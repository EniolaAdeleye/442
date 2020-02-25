//
// Created by Simon Akpoveso on 20/02/2020.
//

#ifndef MSFP_MSCAM_H
#define MSFP_MSCAM_H

#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include <atomic>

void takePicture(raspicam::RaspiCam_Cv &Camera, cv::Mat &image);
void adjPerspective(cv::Mat &image, std::string name);
bool qualityTest(std::vector<int> v1);
void msInit (raspicam::RaspiCam_Cv &Camera);
void convertToBase64(cv::Mat &image);
int isImage(const char *filename);
void printImage(std::string im);


#endif //MSFP_MSCAM_H
