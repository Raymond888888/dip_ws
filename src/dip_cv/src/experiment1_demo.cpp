#include <ros/ros.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <drawImageHistogram.hpp>
#define READIMAGE_ONLY
#ifndef READIMAGE_ONLY
#include <geometry_msgs/Twist.h>
#endif

int main(int argc, char **argv) {
    ROS_WARN("*****START*****");
    ros::init(argc, argv, "trafficLaneTrack");  //初始化ROS节点
    ros::NodeHandle n;

    cv::VideoCapture capture;
    capture.open(0);  //打开相机
    cv::waitKey(100);
    if (!capture.isOpened()) {
        printf("Caemra NOT Opend!\n");
        return 0;
    }

#ifndef READIMAGE_ONLY
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/smoother_cmd_vel", 5);  //定义dashgo机器人的速度发布器
#endif
    cv::Mat src_frame;
    cv::Mat src_frame_gray;
    cv::Mat dst;
    while (ros::ok()) {
        capture.read(src_frame);
        if (src_frame.empty()) {
            break;
        }
        cv::imshow("src_image", src_frame);
        ROS_INFO("%d %d %d\n", src_frame.channels(), src_frame.cols, src_frame.rows);

        // 此处为实验部分，请自行增加直方图均衡化的代码
        cv::cvtColor(src_frame, src_frame_gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(src_frame_gray, dst);
        cv::imshow("Equalized Image", dst);
        cv::imshow("GrayImage", src_frame_gray);
		openCVHist(src_frame_gray);

#ifndef READIMAGE_ONLY
        //以下代码可设置机器人的速度值，从而控制机器人运动
        geometry_msgs::Twist cmd_red;
        cmd_red.linear.x = 0;
        cmd_red.linear.y = 0;
        cmd_red.linear.z = 0;
        cmd_red.angular.x = 0;
        cmd_red.angular.y = 0;
        cmd_red.angular.z = 0.2;
        pub.publish(cmd_red);
#endif
        ros::spinOnce();
        cv::waitKey(5);
    }
    return 0;
}
