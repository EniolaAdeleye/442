//
// Created by Simon Akpoveso on 20/02/2020.
//

#ifndef MSFP_MSNIST_H
#define MSFP_MSNIST_H

#ifdef __cplusplus
extern "C" {
#endif

int ms_imagequality(char *imagedata);
int ms_detect_finger(int FGP, char *imagedata);
int ms_convertToJpegL(char *ifile);
int ms_convertToJpegB(char *ifile);

#ifdef __cplusplus
};
#endif //Extern "C"

#endif //MSFP_MSNIST_H
