#ifndef DRAWIMAGEHISTOGRAM_H_
#define DRAWIMAGEHISTOGRAM_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void openCVHist(const cv::Mat src,std::string str);
void drawHist(std::vector<int> nums);
void calHist(const std::string img);

#endif