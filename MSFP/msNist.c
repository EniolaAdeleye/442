#include "msNist.h"
#include <stdio.h>
#include <imgdecod.h>
#include <log.h>
#include <nfiq.h>
#include <nfseg.h>
#include <lfs.h>
#include <libgen.h>
#include <sys/param.h>
#include <jpegl.h>
#include <ihead.h>
#include <img_io.h>
#include <stdio.h>
#include <sys/param.h>
#include <jpegb.h>
#include <intrlv.h>
#include <ihead.h>
#include <img_io.h>
#include <dataio.h>
#include <parsargs.h>
#include <version.h>


/* Declarations needed to make NIST function calls work */
typedef struct opt_flags_s {
    int verbose;
    int old_mode;
    int has_attribs;
} OPT_FLAGS;

MINUTIAE *minutiae;
int *direction_map, *low_contrast_map, *low_flow_map;
int *high_curve_map, *quality_map;
int map_w, map_h;
seg_rec_coords *fing_boxes;
OPT_FLAGS flags = {0, 0, 0};
int bthr_adj = 0, rot_search = 0, rot_seg = 0, comp = 1;

int img_type, ilen, iw,ih, id, ippi, nfiq, bw, bh, w, h, lossyflag =0, ppi;
int dlen, d;
float conf;
unsigned char *idata, *bdata, *data, **pdata;
int debug = 0;


int ms_imagequality(char *imagedata) {
    int ret;
    if((ret = read_and_decode_grayscale_image(imagedata, &img_type,&idata, &ilen, &iw, &ih, &id, &ippi)))
    {
        if(ret == -3) /* UNKNOWN_IMG */
            fprintf(stderr, "Hint: Use -raw for raw images\n");
        exit(ret);
    }

    //Compute NFIQ value
    ret = comp_nfiq(&nfiq, &conf, idata, iw, ih, id, ippi, &flags.verbose);
    if(ret < 0) {
        free(idata);
        exit(ret);
    }

    return nfiq;

}

int ms_detect_finger(int FGP, char *imagedata) {
    char *filename;
    int ret, nf;
    if (FGP < 15 && FGP > 12)
        nf = 4;
    else if (FGP == 15)
        nf = 2;
    else
        nf = 1;

    if ((ret = read_and_decode_grayscale_image(imagedata, &img_type, &data, &dlen, &w,
                                               &h, &d, &ppi)))
        exit(ret);

    if ((ret = lfs_detect_minutiae_V2(&minutiae, &direction_map,
                                      &low_contrast_map, &low_flow_map,
                                      &high_curve_map, &map_w, &map_h, &bdata,
                                      &bw, &bh, data, w, h, &lfsparms))) {
        return (ret);
    }
    //trim_histtails_contrast_boost(data, w, h);
    /* TRY TO SEGMENT FINGER IMAGE */
    if ((ret = segment_fingers(data, w, h, &fing_boxes, nf, FGP, bthr_adj,
                               rot_search)))
        exit(ret);

    /* PARSE FINGERS FROM ORIGINAL FINGER IMAGE */
    if ((ret = parse_segfing(&pdata, data, w, h, fing_boxes, nf, rot_seg)))
        exit(ret);
    //trim_histtails_contrast_boost(data, w, h);
    free(data);
    /* OUTPUT RESULTS TO FILE */
    filename = basename("image");
    if ((ret = write_parsefing(filename, -1, FGP, comp, ppi, lossyflag, pdata,
                               fing_boxes, nf, rot_seg)))
        exit(ret);

    free(fing_boxes);
    for (int i = 0; i < nf; i++)
        free(pdata[i]);
    free(pdata);

    /* If system error ... */
    if (ret < 0) {
        free(bdata);
        exit(ret);
        return 0;
    }
    return 0;
}

int ms_convertToJpegL(char *ifile) {
    int ret, rawflag = 1, intrlvflag = 0;
    char *outext = "jpegl";/* ouput file extension */
    char *cfile, ofile[MAXPATHLEN]; /* file names */
    int width = 1600, height = 1500, depth = 8, ppi = -1, ilen, olen;/* image parameters */
    unsigned char *idata, *odata;/* image pointers */
    IMG_DAT *img_dat;
    int hor_sampfctr[MAX_CMPNTS], vrt_sampfctr[MAX_CMPNTS];
    int n_cmpnts = 1;
    char *comment_text;


    cfile = (char *) NULL;
    hor_sampfctr[0] = 1;
    vrt_sampfctr[0] = 1;
    hor_sampfctr[1] = 1;
    vrt_sampfctr[1] = 1;
    hor_sampfctr[2] = 1;
    vrt_sampfctr[2] = 1;

    //cfile = 2;
    /* If raw image flagged... */
    if (rawflag) {
        if ((ret = read_raw(ifile, &idata, &width, &height, &depth)))
            return -10;
            //exit(ret);
    }
    /* Otherwise, input image is an IHead image */


    if (debug > 0)
        fprintf(stdout, "File %s read\n", ifile);

    if (cfile == (char *) NULL)
        comment_text = (char *) NULL;


    if ((ret = setup_IMG_DAT_nonintrlv_encode(&img_dat, idata,
                                              width, height, depth, ppi, hor_sampfctr, vrt_sampfctr, n_cmpnts, 0,
                                              PRED4))) {
        free(idata);
        if (comment_text != (char *) NULL)
            free(comment_text);
        return (ret);
    }
    free(idata);


    if (debug > 0) {
        fprintf(stdout, "Image structure initialized\n");
        fflush(stderr);

    }
    if ((ret = jpegl_encode_mem(&odata, &olen, img_dat, comment_text))) {
        free_IMG_DAT(img_dat, FREE_IMAGE);
        if (comment_text != (char *) NULL)
            free(comment_text);
        exit(ret);

    }


    free_IMG_DAT(img_dat, FREE_IMAGE);
    if (comment_text != (char *) NULL)
        free(comment_text);

    if (debug > 0)
        fprintf(stdout, "Image data encoded, compressed byte length = %d\n",
                olen);
    fileroot("image");

    sprintf(ofile, "%s.%s", "image", outext);

    if ((ret = write_raw_from_memsize(ofile, odata, olen))) {
        free(odata);
        exit(ret);

    }

    if (debug > 0)
        fprintf(stdout, "Image data written to file %s\n", ofile);

    free(odata);

}

int ms_convertToJpegB(char *ifile) {




        int ret, i;
        int quality = 60, rawflag = 1, intrlvflag=0;
        char *outext = "jpeg";/* ouput file extension */
        char *cfile, ofile[MAXPATHLEN]; /* file names */
        int width = 1600, height= 1500, depth= 8, ppi=-1;/* image parameters */
        int ilen, olen;
        unsigned char *idata, *odata;/* image pointers */
        char *comment_text;
        int hor_sampfctr[MAX_CMPNTS], vrt_sampfctr[MAX_CMPNTS];
        cfile == (char *)NULL;
        //ifile = "./image1.bmp";




        /* If raw image flagged... */
        if(rawflag) {
           if((ret = read_raw(ifile, &idata, &width, &height, &depth)))
            exit(ret);
           }


       if(debug > 0)
        fprintf(stdout, "File %s read\n", ifile);

       if(cfile == (char *)NULL)
        comment_text = (char *)NULL;
        else{
            if((ret = read_ascii_file(cfile, &comment_text))){
                free(idata);
                exit(ret);
                }
           }


        if((ret = jpegb_encode_mem(&odata, &olen, quality,
        idata, width, height, depth, ppi, comment_text))){
            free(idata);
            if(comment_text != (char *)NULL)
            free(comment_text);
            return(ret);
            }

        free(idata);
        if(comment_text != (char *)NULL)
        free(comment_text);

        if(debug > 0)
        fprintf(stdout, "Image data encoded, compressed byte length = %d\n",
        olen);

        fileroot("image");
        sprintf(ofile, "%s.%s", "image", outext);

       if((ret = write_raw_from_memsize(ofile, odata, olen))){
          free(odata);
            exit(ret);
          }

        if(debug > 0)
        fprintf(stdout, "Image data written to file %s\n", ofile);

        free(odata);

       exit(0);
}
