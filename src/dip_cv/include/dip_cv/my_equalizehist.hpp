#ifndef MY_EQUALIZEHIST_H_
#define MY_EQUALIZEHIST_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


void equalizeHist(cv::InputArray src, cv::OutputArray dst);

#endif 