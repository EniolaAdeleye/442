#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include "msCam.h"

int main() {

    cv::Mat image = cv::imread("image.bmp");

    convertToBase64(image);

    return 0;

}
