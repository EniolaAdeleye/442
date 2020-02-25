//#include <iostream>
//#include "msNist.h"
//#include "msCam.h"
//#include <vector>
//#include <string>
//#include <opencv2/opencv.hpp>
//#include <raspicam/raspicam_cv.h>
//#include <atomic>
//#include <thread>
//#include <unistd.h>
//#include <cstdlib>
//
//int debug = 0,fgp = -1;
//const size_t IMG_SIZE = 2400000;
//
//void t_takePicture(std::atomic<bool>&stop, raspicam::RaspiCam_Cv &Camera, cv::Mat &image) {
//    while (!stop) {
//        takePicture(Camera,image);
//        adjPerspective(image, "image.bmp");
//        system("./cjpegb 60 jpeg image.bmp -raw 1600,1500,8,-1");
//        std::cout << "Thread" << std::endl;
//    }
//
//}
//
//void usage () {
//    std::cout << "Only One argument is required for this program\n" <<
//              "./MSFP RIGHT " << "or\n"
//                                 "./MSFP LEFT " << "or\n"
//                                                   "./MSFP THUMB" <<std:: endl;
//}
//
//
//
//
//int main(int argc, char *argv[]) {
//
//    if (argc != 2) {
//        usage();
//        exit(0);
//    }
//
//    std::atomic<bool>stop {false};
//    raspicam::RaspiCam_Cv Camera;
//    cv::Mat image;
//
//    msInit(Camera);
//
//    std::thread t1(t_takePicture, std::ref(stop), std::ref(Camera), std::ref(image));
//
//    std::map<std::string, char>args = {
//            {"LEFT", 'L'},
//            {"RIGHT", 'R'},
//            {"THUMB", 'T'},
//    };
//
//    char hand = args[argv[1]];
//    std::cout << hand << std::endl;
//    switch (hand) {
//        case 'L':
//            //do something
//            while (1) {
//                std::vector<int>finger_Score{0,0,0,0};
//                fgp = 13;
//                try {
//                    if (isImage("image.bmp") < IMG_SIZE) {
//                        //puts("something wrong");
//                        continue;
//                    }
//                }catch(...) {
//                    continue;
//                }
//                if(ms_convertToJpegL("image.bmp") == -10) {
//                    continue;
//                }
//
//                ms_detect_finger(fgp, "image.jpegl");
//                finger_Score[0] = ms_imagequality("image_02.wsq");
//                finger_Score[1] = ms_imagequality("image_03.wsq");
//                finger_Score[2] = ms_imagequality("image_04.wsq");
//                finger_Score[3] = ms_imagequality("image_05.wsq");
//
//                bool correctScore = qualityTest(finger_Score);
//                if (correctScore) {
//                    stop = true;
//                    break;
//                }
//
//            }
//            break;
//        case 'R':
//            while (1) {
//                std::vector<int>finger_Score{0,0,0,0};
//                fgp = 14;
//                try {
//                    if (isImage("image.bmp") < IMG_SIZE) {
//                        //puts("something wrong");
//                        continue;
//                    }
//                }catch(...) {
//                    continue;
//                }
//                if(ms_convertToJpegL("image.bmp") == -10) {
//                    continue;
//                }
//                ms_detect_finger(fgp, "image.jpegl");
//                finger_Score[0] = ms_imagequality("image_07.wsq");
//                finger_Score[1] = ms_imagequality("image_08.wsq");
//                finger_Score[2] = ms_imagequality("image_09.wsq");
//                finger_Score[3] = ms_imagequality("image_10.wsq");
//
//                bool correctScore = qualityTest(finger_Score);
//                if (correctScore) {
//                    stop = true;
//                    break;
//                }
//
//            }
//            break;
//        case 'T':
//            while (1) {
//                std::vector<int>finger_Score{0,0};
//                fgp = 15;
//                try {
//                    if (isImage("image.bmp") < IMG_SIZE) {
//                        //puts("something wrong");
//                        continue;
//                    }
//                }catch(...) {
//                    continue;
//                }
//                if(ms_convertToJpegL("image.bmp") == -10) {
//                    continue;
//                }
//                ms_detect_finger(fgp, "image.jpegl");
//                finger_Score[0] = ms_imagequality("image_11.wsq");
//                finger_Score[1] = ms_imagequality("image_12.wsq");
//
//                bool correctScore = qualityTest(finger_Score);
//                if (correctScore) {
//                    stop = true;
//                    break;
//                }
//
//            }
//            break;
//        default:
//            std::cout << "Invalid Argument" << std::endl;
//            //do something
//            break;
//    }
//
//    t1.join();
//
//    return 0;
//}

#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <atomic>
#include "msCam.h"
#include "msNist.h"

int main(int argc , char* argv[]) {

//    if (argc != 2) {
//        std::cout << "Arguments should be only one" << std::endl;
//        exit(0);
//    }

    std::atomic<bool>stop {false};
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    int debug = 0;
    const int IMG_SIZE = 2400000;


    const char * command = argv[1];
    std::cout << "Hello" << std::endl;

    int switcher = 0;

    if (strcmp(command,"LEFT") == 0){
        switcher = 1;
    }else if ( strcmp(command,"RIGHT") == 0){
        switcher = 2;
    }else if ( strcmp(command,"THUMB") == 0) {
        switcher = 3;
    }else {
        std::cout << "Invalid Argument" << std::endl;
        exit(1);
        switcher = 0;
    }

    std::cout << "Switcher is " << switcher << std::endl;
    msInit(Camera);
    int count = 1;
    int fgp = -1;

    switch (switcher) {
        case 1:
            while (1) {
                //std::vector<int>fingerScore{0,0,0,0};
                //fgp = 13;
                takePicture(Camera, image);
                std::cout << "Picture taken" << std::endl;
                convertToBase64(image);

//                adjPerspective(image, "image.bmp");
//                try {
//                    if (isImage("image.bmp") < IMG_SIZE) {
//                        std::cout << "Something wrong" << std::endl;
//                        continue;
//                    }
//                }catch(...) {
//                    continue;
//                }
//                --count;
//                if(ms_convertToJpegL("image.bmp") == -10) {
//                    std::cout << "still printing Image" <<std::endl;
//                    continue;
//                }
//                ms_detect_finger(fgp, "image.jpegl");
//                fingerScore[0] = ms_imagequality("image_02.wsq");
//                fingerScore[1] = ms_imagequality("image_03.wsq");
//                fingerScore[2] = ms_imagequality("image_04.wsq");
//                fingerScore[3] = ms_imagequality("image_05.wsq");
//
//                bool valid = qualityTest(fingerScore);
//                std::cout << valid << std::endl;
//                if (!valid) {
//                    std::cout << "Image not good enough" << std::endl;
//                    continue;
//                }else {
//                    break;
//                }

            }
            std::cout << "Done" << std::endl;
            //do something
            break;
        case 2:
            //do something
            break;
        case 3:
            //do something
            break;
        default:
            //do nothing
            break;
    }


    return 0;
}