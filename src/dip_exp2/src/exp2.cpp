#include <ros/ros.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "gauss_filter.hpp"
#include "morphology.hpp"
#include "yaml-cpp/yaml.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    ros::init(argc, argv, "exp2");
    ros::NodeHandle nh;
        VideoCapture capture;
        YAML::Node config = YAML::LoadFile("/home/raymond/dip_ws/src/dip_exp2/config/config.yaml");
        capture.open(0);
        if (!capture.isOpened()) {
            ROS_WARN("can not open video");
            return 0;
        }
    Mat src;
    while (ros::ok()) {
        if (!config["read_camera"].as<int>()) {
            capture.read(src);
            if (src.empty()) {
                ROS_WARN("src is empty!");
                return 0;
            }
        } else {
            src = imread("/home/raymond/dip_ws/src/dip_exp3/config/11.png");
            if (config["read_camera"].as<int>() == 2)
                src = imread("/home/raymond/dip_ws/src/dip_exp3/config/22.png");
        }
        imshow("src_img", src);
        Mat gray_img;
        cvtColor(src, gray_img, cv::COLOR_BGR2GRAY);
        imshow("gray", gray_img);
        Mat gaussian_gray_img;
        Gaussian_Filter(gray_img, gaussian_gray_img, config["sigma"].as<double>());
        imshow("gaussian_filted_img", gaussian_gray_img);
        Mat binarilized_gray_img = gray_img.clone();
        for (int i = 0; i < gray_img.rows; i++) {
            for (int j = 0; j < gray_img.cols; j++) {
                if (gray_img.at<uchar>(i, j) > 128)
                    binarilized_gray_img.at<uchar>(i, j) = 255;
                else
                    binarilized_gray_img.at<uchar>(i, j) = 0;
            }
        }
        Mat dilate_img;
        Mat erode_img;
        Mat dst = (Mat_<uchar>(3, 3) << 0, 1, 0,
                   1, 1, 1,
                   0, 1, 0);
        imshow("binarilized_gray_img", binarilized_gray_img);
        Dilate(binarilized_gray_img, dilate_img, dst);
        imshow("dilate_img", dilate_img);
        Erode(binarilized_gray_img, erode_img, dst);
        imshow("erode_img", erode_img);
        if (!config["read_camera"].as<int>()) {
            waitKey(1);
            ros::spinOnce();
        } else {
            waitKey(0);
        }
    }
    destroyAllWindows();
    return 0;
}