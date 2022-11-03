#include <ros/ros.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "3.hpp"
#include "yaml-cpp/yaml.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    ros::init(argc, argv, "exp3");
    ros::NodeHandle nh;
    YAML::Node config = YAML::LoadFile("/home/raymond/dip_ws/src/dip_exp3/config/config.yaml");

    Expr3Node expr3Node(nh);

    Mat frame = imread(config["pictures"].as<string>());
    Mat write(560,1280,CV_8UC1,Scalar(255));
    imwrite("/home/raymond/1280x560.jpg",write);
    waitKey(0);
    while (ros::ok()) {
        // capture.read(frame);

//         if (frame.empty()) break;
// #if USE_COMPUTER_CAMERA
//         Mat frIn = frame.clone();
// #else
//         Mat frIn = frame() cv::Rect(0, 0, frame.cols / 2, frame.rows);
// #endif
//         // src frame
//         imshow("origin", frame);
//         ROS_INFO("%d %d",frame.rows,frame.cols);
//         // canny
//         Mat edge(frame.rows, frame.cols, CV_8UC1, Scalar(0));
//         Mat grey(frame.rows, frame.cols, CV_8UC1, Scalar(0));
//         cvtColor(frame, grey, COLOR_BGR2GRAY);
//         expr3Node.EdgeDetector(grey, edge, config["sigma"].as<double>(), config["high_th"].as<double>(), config["low_th"].as<double>());
//         imshow("canny edge", edge);
//         // hough line
//         Mat line = frame.clone();
//         expr3Node.HoughLine(frame, line, config["line_threth"].as<int>());
//         imshow("line", line);
//         // hough hough_circle
//         Mat hough_circle = frame.clone();
//         expr3Node.HoughCircle(frame, hough_circle, config["max_r"].as<int>(), config["min_r"].as<int>());
//         imshow("hough_circle", hough_circle);
//         waitKey(0);
    }
    return 0;
}