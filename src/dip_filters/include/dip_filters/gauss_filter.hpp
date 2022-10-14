#ifndef GAUSS_FILTER_H_
#define GAUSS_FILTER_H_
#include <ros/ros.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "gauss_filter.hpp"
#include "morphology.hpp"
using namespace cv;
class Gauss_Filter_Mine {
   public:
    void Gaussian_Kernerl(Mat &kernel, double sigma);

    void Gaussian(Mat &Input, Mat &output, double sigma);
};

void Gaussian_Kernerl(Mat &kernel, double sigma) {
    int dim = ceil(6 * sigma);
    if (!(dim % 2)) {
        dim += 1;
    }
    kernel.create(dim, dim, CV_64FC1);
    double sum = 0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            double distance = pow(i - (dim / 2), 2) + pow(j - (dim / 2), 2);
            kernel.at<_Float64>(i, j) = pow(2.718281828459, -distance / (2 * sigma * sigma));
            sum += kernel.at<_Float64>(i, j);
        }
    }
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            kernel.at<_Float64>(i, j) /= sum;
        }
    }
}

void Gaussian(Mat &Input, Mat &output, double sigma) {
    Mat temp;
    Mat kernerl;
    output = Input.clone();
    Gaussian_Kernerl(kernerl, sigma);
    int dim = kernerl.rows;
    copyMakeBorder(Input, temp, dim / 2, dim / 2, dim / 2, dim / 2, CV_HAL_BORDER_CONSTANT, Scalar(0, 0, 0));
    for (int i = dim / 2; i < temp.rows - dim / 2; i++) {
        for (int j = dim / 2; j < temp.cols - dim / 2; j++) {
            double sum = 0;
            for (int n = 0; n < dim; n++) {
                for (int m = 0; m < dim; m++) {
                    sum += (double)temp.at<uchar>(i + n - dim / 2, j + m - dim / 2) * kernerl.at<_Float64>(n, m);
                }
                output.at<uchar>(i - dim / 2, j - dim / 2) = sum;
            }
        }
    }
}
#endif