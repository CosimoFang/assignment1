#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2){return -2;}

    Mat picture;
    picture = imread(argv[1], IMREAD_COLOR);

    if (picture.empty()) {
        cout << "Cannnot find the image" << std::endl;
        return -1;
    }

    namedWindow("test", WINDOW_AUTOSIZE); 
    imshow("test", picture); 
    waitKey(0); 
    while (waitKey(100) != 113) {}
    return 0;
}