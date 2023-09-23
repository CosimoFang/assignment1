//the different filter header file



#ifndef __filter_h
#define __filter_h

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

int greyscale(cv::Mat& src, cv::Mat& dst);
int blur5x5(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);
int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);
int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels);
int cartoon(cv::Mat &src, cv::Mat&dst, int levels, int magThreshold);
int reverseTheImg(cv::Mat& src, cv::Mat& dst);

#endif#pragma once
