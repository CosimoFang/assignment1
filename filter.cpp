#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core/core.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <filter.h>
#include <algorithm>
#include <iostream>


int greyscale(cv::Mat& src, cv::Mat& dst) {
    dst.create(src.size(), src.type());

    for (int i = 0; i < src.rows; i++) {
        cv::Vec3b* row_src = src.ptr<cv::Vec3b>(i);

        cv::Vec3b* row_dst = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {

            int blue = row_src[j][0];
            row_dst[j][0] = blue;
            row_dst[j][1] = blue;
            row_dst[j][2] = blue;
        }
    }
    return 0;
}


int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst) {
    dst.create(sx.size(), sx.type());
    int n = sx.rows;
    int m = sx.cols;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            double pix[3] = { 0, 0, 0 };
            cv::Vec3b p_1 = sx.at<cv::Vec3b>(i, j);
            cv::Vec3b p_2 = sy.at<cv::Vec3b>(i, j);
            for (int k = 0; k < 3; k++) {
                dst.at<cv::Vec3b>(i, j)[k] = sqrt(p_1[k] * p_1[k] + p_2[k] * p_2[k]);
            }

        }
    }
    return 0;

}


int sobelY3x3(cv::Mat& src, cv::Mat& dst) {
    dst.create(src.size(), src.type());
    float kernely[3][3] = 
    { { 1, 2, 1 },
    { 0,  0,  0 },
    { -1,  -2,  -1 } };
    int n = src.rows;
    int m = src.cols;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            double pix[3] = { 0, 0, 0 };
            
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    cv::Vec3b p = src.at<cv::Vec3b>(i - 1 + a, j - 1 + b);
                    for (int k = 0; k < 3; k++) {
                        pix[k] += p[k] * kernely[a][b] / 8;
                    }
                }
            }
            for (int k = 0; k < 3; k++) {
                
                dst.at<cv::Vec3b>(i, j)[k] = abs(pix[k]);
            }
            
        }
    }
    return 0;
}


int sobelX3x3(cv::Mat& src, cv::Mat& dst) {
    dst.create(src.size(), src.type());
    float kernelx[3][3] = 
    { { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 } };
    int n = src.rows;
    int m = src.cols;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            double pix[3] = { 0, 0, 0 };

            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    cv::Vec3b p = src.at<cv::Vec3b>(i - 1 + a, j - 1 + b);
                    for (int k = 0; k < 3; k++) {
                        pix[k] += p[k] * kernelx[a][b] / 8;
                    }
                }
            }
            for (int k = 0; k < 3; k++) {
                
                dst.at<cv::Vec3b>(i, j)[k] = abs(pix[k]);
            }

        }
    }


    return 0;
}


int blur5x5(cv::Mat& src, cv::Mat& dst) {
    dst.create(src.size(), src.type());
    cv::Mat temp;
    temp = dst.clone();
    int f[5] = {1, 2, 4, 2, 1 };

    int n = src.rows;
    int m = src.cols;
    for (int i = 0; i < n; i++) {
        cv::Vec3b* row = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* row_dst = temp.ptr<cv::Vec3b>(i);
        for (int j = 2; j < m-2; j++) {
            double pix[3] = {0, 0, 0};
            for (int k = -2; k < 3; k++) {
                for (int t = 0; t < 3; t++) {
                    pix[t] += f[2 + k] * row[j + k][t] / 10;
                }
            }
            for (int k = 0; k < 3; k++) {
                if (pix[k] > 255){
                    pix[k] = 255;
                }
                if (pix[k] < 0) {
                    pix[k] = 0;
                }
                row_dst[j][k] = pix[k];
            }
        }
    }

    
    for (int j = 0; j < m; j++) {
        for (int i = 2; i < n - 2; i++) {

            double pix[3] = { 0, 0, 0 };
            for (int k = -2; k < 3; k++) {
                cv::Vec3b p = temp.at< cv::Vec3b>(i + k, j);
                pix[0] += f[2 + k] * p[0] / 10;
                pix[1] += f[2 + k] * p[1] / 10;
                pix[2] += f[2 + k] * p[2] / 10;
            }
            for (int k = 0; k < 3; k++) {
                if (pix[k] > 255) {
                    pix[k] = 255;
                }
                if (pix[k] < 0) {
                    pix[k] = 0;
                }
                dst.at<cv::Vec3b>(i, j)[k] = pix[k];
            }
        }
    }

    return 0;
}



int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels) {

    dst.create(src.size(), src.type());
    blur5x5(src, dst);
    int n = dst.rows;
    int m = dst.cols;
    int x = 255 / levels;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < 3; k++) {
                dst.at<cv::Vec3b>(i, j)[k] = (dst.at<cv::Vec3b>(i, j)[k] / x);
                dst.at<cv::Vec3b>(i, j)[k] = (dst.at<cv::Vec3b>(i, j)[k] * x);
            }
        }
    }
    return 0;
}
int reverseColor(cv::Mat& src, cv::Mat& dst) {

    dst = src.clone();
    
    int n = dst.rows;
    int m = dst.cols;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < 3; k++) {
                dst.at<cv::Vec3b>(i, j)[k] = (255 - dst.at<cv::Vec3b>(i, j)[k]);
            }
        }
    }
    return 0;




}



int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold) {
    
    cv::Mat x;
    x.create(src.size(), src.type());
    sobelX3x3(src, x);

    cv::Mat y;
    y.create(src.size(), src.type());
    sobelY3x3(src, y);


    cv::Mat mag;
    magnitude(x, y, mag);
    mag.create(src.size(), src.type());

    blurQuantize(src, dst, levels);

    int n = src.rows;
    int m = src.cols;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cv::Vec3b p = mag.at<cv::Vec3b>(i, j);
            if ((p[0] + p[1] + p[2]) > 3 * magThreshold) {
                dst.at<cv::Vec3b>(i, j) = { 0, 0, 0 };
            }
        }
    }

    return 0;
}


