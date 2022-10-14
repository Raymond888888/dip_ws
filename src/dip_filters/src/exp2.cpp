#include <ros/ros.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "gauss_filter.hpp"
#include "morphology.hpp"
using namespace cv; 

int main(int argc, char **argv)
{
    ros::init(argc, argv, "exp2");
    ros::NodeHandle nh;
    VideoCapture capture;
    capture.open(0);
    if (!capture.isOpened())
    {
        ROS_WARN("can not open video");
        return 0;
    }
    Mat src;
    while (ros::ok())
    {
        capture.read(src);
        if (src.empty())
        {
            ROS_WARN("src is empty!");
            return 0;
        }
        imshow("src", src);
        Mat gray_Img;
        cvtColor(src, gray_Img, cv::COLOR_BGR2GRAY);
        imshow("gray", gray_Img);
        Mat gaussian_gray_img;
        Gaussian(gray_Img, gaussian_gray_img, 0.5);
        imshow("sigma = 1", gaussian_gray_img);
        Mat b_gray_img = gray_Img.clone();
        for (int i = 0; i < gray_Img.rows; i++)
        {
            for (int j = 0; j < gray_Img.cols; j++)
            {
                if (gray_Img.at<uchar>(i, j) > 128)
                    b_gray_img.at<uchar>(i, j) = 255;
                else
                    b_gray_img.at<uchar>(i, j) = 0;
            }
        }
        Mat dilate_img;
        Mat erode_img;
        Mat dst = (Mat_<uchar>(3, 3) << 0, 1, 0,
                   1, 1, 1,
                   0, 1, 0);
        imshow("binarization", b_gray_img);
        Dilate(b_gray_img, dilate_img, dst);
        imshow("dilate", dilate_img);
        Erode(b_gray_img, erode_img, dst);
        imshow("erode", erode_img);
        waitKey(1);
        ros::spinOnce();
    }
    destroyAllWindows();
    return 0;
}