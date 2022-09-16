#ifndef DRAWIMAGEHISTOGRAM_H_
#define DRAWIMAGEHISTOGRAM_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void openCVHist(const Mat src,string str);
void drawHist(vector<int> nums);
void calHist(const string img);

#endif