cmake_minimum_required(VERSION 3.13)
project(MSFP)


set(CMAKE_STANDARD_CXX 14)
set(SOURCE main.cpp msCam.cpp msNist.c base64.cpp)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
#Set the directories to find cmake config files for libraries
set(OpenCV_DIR "/usr/share/OpenCV/")
set(raspicam_DIR "/usr/local/lib/cmake/")
set(NIST_MAIN_LIB "/home/pi/Main/lib")
#Find Packages needed
find_package(OpenCV REQUIRED)
find_package(raspicam REQUIRED)
find_library(WIRINGPI_LIBRARIES NAMES wiringPi REQUIRED)

#Test
if(OpenCV_FOUND AND raspicam_CV_FOUND)

    MESSAGE(STATUS "COMPILING OPENCV TESTS")

endif(OpenCV_FOUND AND raspicam_CV_FOUND)


add_executable(MSFP ${SOURCE})

target_include_directories(MSFP PRIVATE ~/Main/include/)

target_link_libraries(MSFP ${raspicam_CV_LIBS} ${WIRINGPI_LIBRARIES} ${OpenCV_LIB}
        ${NIST_MAIN_LIB}/libnfseg.a
        ${NIST_MAIN_LIB}/libimage.a
        ${NIST_MAIN_LIB}/libwsq.a
        ${NIST_MAIN_LIB}/libfet.a
        ${NIST_MAIN_LIB}/liban2k.a
        ${NIST_MAIN_LIB}/libjpegl.a
        ${NIST_MAIN_LIB}/libjpegb.a
        ${NIST_MAIN_LIB}/libihead.a
        #${CMAKE_SOURCE_DIR}/libm.so.6
        ${NIST_MAIN_LIB}/libmlp.a
        ${NIST_MAIN_LIB}/libpca.a
        ${NIST_MAIN_LIB}/libutil.a
        ${NIST_MAIN_LIB}/libioutil.a
        ${NIST_MAIN_LIB}/libnfiq.a
        ${NIST_MAIN_LIB}/libpng.a
        ${NIST_MAIN_LIB}/libf2c.a
        ${NIST_MAIN_LIB}/libmindtct.a
        ${NIST_MAIN_LIB}/libmlp.a
        ${NIST_MAIN_LIB}/libcblas.a
        ${NIST_MAIN_LIB}/libmindtct.a
        ${NIST_MAIN_LIB}/libopenjp2.a
        ${NIST_MAIN_LIB}/libz.a
        ${NIST_MAIN_LIB}/libclapck.a
        ${NIST_MAIN_LIB}/libfft.a
        )