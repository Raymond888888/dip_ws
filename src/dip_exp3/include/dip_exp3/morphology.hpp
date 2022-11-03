#ifndef MORPHOLOGY_H_
#define MORPHOLOGY_H_
#include <ros/ros.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "gauss_filter.hpp"
#include "morphology.hpp"

using namespace cv;

void Dilate(Mat &Input, Mat &output, Mat Dst) {
    Mat temp;
    int ker_rows = Dst.rows;
    int ker_cols = Dst.cols;
    output = Input.clone();
    copyMakeBorder(Input, temp, ker_rows / 2, ker_rows / 2, ker_cols / 2, ker_cols / 2, CV_HAL_BORDER_CONSTANT, Scalar(0, 0, 0));
    for (int i = ker_rows / 2; i < temp.rows - ker_rows / 2; i++) {
        for (int j = ker_cols / 2; j < temp.cols - ker_cols / 2; j++) {
            output.at<uchar>(i - ker_rows / 2, j - ker_cols / 2) = 0;
            bool flag = 0;
            for (int n = 0; n < ker_rows; n++) {
                for (int m = 0; m < ker_cols; m++) {
                    if (Dst.at<uchar>(m, n) && temp.at<uchar>(i + n - ker_rows / 2, j + m - ker_cols / 2)) {
                        output.at<uchar>(i - ker_rows / 2, j - ker_cols / 2) = 255;
                        flag = 1;
                        break;
                    }
                    if (flag)
                        break;
                }
            }
        }
    }
}

void Erode(Mat &Input, Mat &output, Mat Dst) {
    Mat temp;
    int ker_rows = Dst.rows;
    int ker_cols = Dst.cols;
    output = Input.clone();
    copyMakeBorder(Input, temp, ker_rows / 2, ker_rows / 2, ker_cols / 2, ker_cols / 2, CV_HAL_BORDER_CONSTANT, Scalar(0, 0, 0));
    for (int i = ker_rows / 2; i < temp.rows - ker_rows / 2; i++) {
        for (int j = ker_cols / 2; j < temp.cols - ker_cols / 2; j++) {
            output.at<uchar>(i - ker_rows / 2, j - ker_cols / 2) = 255;
            bool flag = 0;
            for (int n = 0; n < ker_rows; n++) {
                for (int m = 0; m < ker_cols; m++) {
                    if ((Dst.at<uchar>(m, n) && !temp.at<uchar>(i + n - ker_rows / 2, j + m - ker_cols / 2))) {
                        output.at<uchar>(i - ker_rows / 2, j - ker_cols / 2) = 0;
                        flag = 1;
                        break;
                    }
                    if (flag)
                        break;
                }
            }
        }
    }
}

#endif