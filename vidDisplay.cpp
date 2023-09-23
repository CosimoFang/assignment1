//the video real time funciton


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core/core.hpp"

#include "filter.h"

#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;



int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;
    int cond = 0;
    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        // see what is the keystrok and change the status of the loop
        if (key == 'q') {
            break;
        }
        if (key == 'g') {
            cond = 1;
        }
        if (key == 'h') {
            cond = 2;
        }
        if (key == 'b') {
            cond = 3;
        }
        if (key == 'm') {
            cond = 4;
        }
        if (key == 'l') {
            cond = 5;
        }
        if (key == 'c') {
            cond = 6;
        }
        if (key == 'r') {
            cond = 7;
        }

        //given the status to change the output img
        Mat p_image;
        if (cond == 1) {
            cvtColor(frame, p_image, COLOR_RGBA2GRAY, 0);
        }
        else if (cond == 2) {
            greyscale(frame, p_image);
        }
        else if (cond == 3) {
            blur5x5(frame, p_image);
        }
        else if (cond == 4) {
            Mat p_x;
            Mat p_y;
            sobelX3x3(frame, p_x);
            sobelY3x3(frame, p_y);
            magnitude(p_x, p_y, p_image);
        }
        else if (cond == 5) {
            blurQuantize(frame, p_image, 15);
        }
        else if (cond == 6) {
            cartoon(frame, p_image, 15, 15);
        }
        else if (cond == 7) {
            reverseTheImg(frame, p_image);
        }
        else {
            p_image = frame;
        }


        //save file
        if (key == 's') {
            imwrite("/Users/Mark/Desktop/sampleImage.jpg", p_image);
        }


        cv::imshow("Video", p_image);

    }

    delete capdev;
    return(0);
}


