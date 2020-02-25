#include <cstdio>
#include <cstdlib>
#include <raspicam/raspicam_cv.h>
#include "msCam.h"
#include <vector>
#include "base64.h"
#include "thread"
#include <atomic>
#include <opencv2/opencv.hpp>

const int SCORE_LIMIT = 4;
const int WIDTH = 3248;
const int HEIGHT = 2464;



//Initialize Objects
raspicam::RaspiCam_Cv Camera;
cv::Mat image;

void msInit (raspicam::RaspiCam_Cv &Camera) {

    Camera.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
    if(!Camera.open())
    {
        puts("Camera not Opened");
        exit(1);
    }

}

void takePicture(raspicam::RaspiCam_Cv &Camera, cv::Mat &image) {
        Camera.grab();
        Camera.retrieve(image);
}
void adjPerspective(cv::Mat &image, std::string name) {
    if (!image.data) {
        puts("no data");
        exit(0);
    }

    //Might need to check if image object is in good condition
    cv::Point2f topLeft(496, 52);
    cv::Point2f topRight(2868,212);
    cv::Point2f bottomRight(2732,1852);
    cv::Point2f bottomLeft(400,1692);
    cv::Point2f quad1[4] = {topLeft, topRight, bottomLeft, bottomRight};

    cv::Point2f topLeftNew(0, 0);
    cv::Point2f topRightNew(1600,0);
    cv::Point2f bottomRightNew(1600,1500);
    cv::Point2f bottomLeftNew(0,1600);
    cv::Point2f quad2[4] = {topLeftNew,topRightNew,bottomLeftNew,bottomRightNew};

    cv::Mat matrix = cv::getPerspectiveTransform(quad1,quad2);

    cv::warpPerspective(image,image,matrix,cv::Size(1600,1500));

    cv::GaussianBlur(image,image,cv::Size(),1.3);

    cv::adaptiveThreshold(image,image,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY,11,2.5);
    cv::imwrite(name, image);

}
bool qualityTest(std::vector<int> v1) {

    bool value = true;
    for (int i : v1) {

        bool a = i < SCORE_LIMIT && i > 0;
        if (a) {
            std::cout << "quality value is " << i << std::endl;
            //printf("quality value is %d\n", i);
            continue;
        } else {
            value = false;
            std::cout << "quality value is " << i << std::endl;
            //printf("quality value is %d\n", i);
        }
    }
    printf("\n");
    return value;
}

void convertToBase64(cv::Mat &image) {

    std::vector<uchar> buf;
    cv::imencode(".jpg", image , buf);
    auto *enc_msg = reinterpret_cast<unsigned char*>(buf.data());
    std::string encoded = base64_encode(enc_msg, buf.size());
//
//    std::string dec_jpg =  base64_decode(encoded);
//    std::vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
//    cv::Mat img = cv::imdecode(cv::Mat(data), 1);
//    cv::imwrite("dec.bmp", img);

    std::cout <<"image "<<encoded<<std::endl;

}

int isImage(const char *filename) {
    FILE * pfile;
    long lSize;
    char *buffer;
    size_t result;

    pfile = fopen(filename, "r");
    if (pfile == NULL) {//puts("File Error");
    throw "FILE ERROR";}

    fseek(pfile, 0, SEEK_END);
    lSize = ftell(pfile);
    rewind(pfile);
    buffer = (char* )malloc(sizeof(char)*lSize);
    if (buffer == NULL) {//puts("Memory error");
    throw "MEMORY ERROR";}

    result = fread(buffer,1,lSize,pfile);
    if (result != lSize) {//puts("Reading Error");
    throw "READING ERROR";}

    //printf("it is %zu\n", result);

    //terminate
    fclose(pfile);
    free(buffer);

    return result;
}

void printImage(std::string im) {
    std::cout << im << std::endl;
}


