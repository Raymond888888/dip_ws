#include <ros/ros.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char **argv) {
    ros::init(argc, argv, "dip_filter");  //初始化ROS节点
    ros::NodeHandle nf;
    ROS_INFO("test\n");
    return 0;
}
