#ifndef EXP04_HPP_
#define EXP04_HPP_

#include "cmath"
#include "cstdlib"
#include "geometry_msgs/Twist.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "ros/ros.h"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace cv;

#define USE_COMPUTER_CAMERA 1

class Exp4 {
   public:
    Exp4(ros::NodeHandle &);

   private:
    void ColorSplitManual(const Mat &hsv_input, Mat &grey_output, const string window);

    void ColorSplitAuto(const Mat &hsv_input, Mat &bgr_output, vector<vector<Point>> &contours, int hmin, int hmax,
                        int smin, int smax, int vmin, int vmax);

    void GetROI(const Mat &input, Mat &output, const vector<vector<Point>> &contour);

    void ColorHist(const Mat &roi, Mat &hist);

    void Color2Velocity(const Mat &input, Mat &output);

    int CountROIPixel(const Mat &input);

    VideoCapture capture;

    int colorSizeThresh = 10000;

    int exp4part = 1;

    //{hmin, hmax, smin, smax, vmin, vmax}
    int red_thresh[6] = {245, 23, 143, 255, 102, 255};
    int green_thresh[6] = {66, 113, 0, 255, 67, 255};
    int blue_thresh[6] = {115, 168, 52, 255, 39, 223};
    int yellow_thresh[6] = {19, 47, 143, 255, 102, 255};

    ros::Publisher vel_pub;
};

Exp4::Exp4(ros::NodeHandle &nh) {
    YAML::Node config = YAML::LoadFile("/home/raymond/dip_ws/src/dip_exp4/config/config.yaml");

    capture.open(!USE_COMPUTER_CAMERA);
    vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
    this->exp4part = config["exp4part"].as<int>();

    if (!capture.isOpened())
        ROS_ERROR("Failed to open camera. Replug the camera.");
    else {
        Mat frame_raw;
        while (ros::ok()) {
            // Mat frame_raw = imread(config["pictures"].as<string>());
            capture.read(frame_raw);
            if (frame_raw.empty()) break;
#if USE_COMPUTER_CAMERA
                // Mat frame = frame_raw.clone();
#else
            Mat frIn = frame() Rect(0, 0, frame.cols / 2, frame.rows);
#endif
            // src frame
            double factor = 0.5;
            Mat frame((int)(factor * frame_raw.rows), (int)(factor * frame_raw.cols), CV_8UC3);
            resize(frame_raw, frame, Size((int)(factor * frame_raw.cols), (int)(factor * frame_raw.rows)));
            imshow("origin", frame);

            if (this->exp4part == 1) {
                Mat filtered(frame.rows, frame.cols, CV_8UC3);
                cv::GaussianBlur(frame, filtered, Size(3, 3), 3);
                // imshow("filtered", filtered);
                Mat hsv(frame.rows, frame.cols, CV_8UC3);
                cv::cvtColor(filtered, hsv, cv::COLOR_BGR2HSV);
                imshow("hsv", hsv);
                Mat grey(frame.rows, frame.cols, CV_8UC1);
                ColorSplitManual(hsv, grey, "hsv");
                imshow("split", grey);
                Mat target = frame.clone();
                vector<vector<Point>> contours;
                ColorSplitAuto(hsv, target, contours, 153, 197, 82, 255, 74, 172);
                imshow("target", target);
                Mat roi;
                GetROI(frame, roi, contours);
                imshow("roi", roi);
                Mat hist = Mat::zeros(200, 200, CV_8UC3);
                ColorHist(roi, hist);
                imshow("hist", hist);
            } else if (this->exp4part == 2) {
                Mat velOutput(frame.size(), CV_8UC3);
                Color2Velocity(frame, velOutput);
                imshow("vel output", velOutput);
            }
            waitKey(10);
            ros::spinOnce();
        }
    }
}

void Exp4::ColorSplitManual(const Mat &hsv_input, Mat &grey_output, const string window) {
    static int hmin = 0;
    static int hmax = 255;
    static int smin = 0;
    static int smax = 255;
    static int vmin = 0;
    static int vmax = 255;
    createTrackbar("Hmin", window, &hmin, 255);
    createTrackbar("Hmax", window, &hmax, 255);
    createTrackbar("Smin", window, &smin, 255);
    createTrackbar("Smax", window, &smax, 255);
    createTrackbar("Vmin", window, &vmin, 255);
    createTrackbar("Vmax", window, &vmax, 255);
    int rw = hsv_input.rows;
    int cl = hsv_input.cols;
    for (int i = 0; i < rw; i++) {
        for (int j = 0; j < cl; j++) {
            if (vmin <= hsv_input.at<Vec3b>(i, j)[0] && hsv_input.at<Vec3b>(i, j)[0] <= vmax &&
                smin <= hsv_input.at<Vec3b>(i, j)[1] && hsv_input.at<Vec3b>(i, j)[1] <= smax &&
                hmin <= hsv_input.at<Vec3b>(i, j)[2] && hsv_input.at<Vec3b>(i, j)[2] <= hmax)
                grey_output.at<uchar>(i, j) = 255;
            else
                grey_output.at<uchar>(i, j) = 0;
        }
    }
}

void Exp4::ColorSplitAuto(const Mat &hsv_input, Mat &bgr_output, vector<vector<Point>> &contours, int hmin, int hmax, int smin, int smax, int vmin, int vmax) {
    int rw = hsv_input.rows;
    int cl = hsv_input.cols;
    Mat color_region(rw, cl, CV_8UC1);
    for (int i = 0; i < rw; i++) {
        for (int j = 0; j < cl; j++) {
            if (hmax >= hmin) {
                if (vmin <= hsv_input.at<Vec3b>(i, j)[0] && hsv_input.at<Vec3b>(i, j)[0] <= vmax &&
                    smin <= hsv_input.at<Vec3b>(i, j)[1] && hsv_input.at<Vec3b>(i, j)[1] <= smax &&
                    hmin <= hsv_input.at<Vec3b>(i, j)[2] && hsv_input.at<Vec3b>(i, j)[2] <= hmax)
                    color_region.at<uchar>(i, j) = 255;
                else
                    color_region.at<uchar>(i, j) = 0;
            } else {
                if (vmin <= hsv_input.at<Vec3b>(i, j)[0] && hsv_input.at<Vec3b>(i, j)[0] <= vmax &&
                    smin <= hsv_input.at<Vec3b>(i, j)[1] && hsv_input.at<Vec3b>(i, j)[1] <= smax &&
                    (hmax >= hsv_input.at<Vec3b>(i, j)[2] || hsv_input.at<Vec3b>(i, j)[2] >= hmin))
                    color_region.at<uchar>(i, j) = 255;
                else
                    color_region.at<uchar>(i, j) = 0;
            }
        }
    }
    vector<Vec4i> hierarchy;
    findContours(color_region, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    vector<vector<Point>> lines(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        approxPolyDP(contours[i], lines[i], 9, true);
    }
    drawContours(bgr_output, lines, -1, Scalar(0, 0, 255), 2, 8);
}

void Exp4::GetROI(const Mat &input, Mat &output, const vector<vector<Point>> &contour) {
    Mat roi = Mat::zeros(input.size(), CV_8U);

    drawContours(roi, contour, -1, Scalar(255, 255, 255), -1);

    input.copyTo(output, roi);
}

void Exp4::ColorHist(const Mat &roi, Mat &hist) {
    int rw = roi.rows;
    int cl = roi.cols;
    int color_num[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < rw; i++) {
        for (int j = 0; j < cl; j++) {
            if (roi.at<Vec3b>(i, j)[0] || roi.at<Vec3b>(i, j)[1] || roi.at<Vec3b>(i, j)[2]) {
                Mat tmp_bgr(1, 1, CV_8UC3);
                tmp_bgr.at<Vec3b>(0, 0)[0] = roi.at<Vec3b>(i, j)[0];
                tmp_bgr.at<Vec3b>(0, 0)[1] = roi.at<Vec3b>(i, j)[1];
                tmp_bgr.at<Vec3b>(0, 0)[2] = roi.at<Vec3b>(i, j)[2];
                Mat tmp_hsv(1, 1, CV_8UC3);
                cv::cvtColor(tmp_bgr, tmp_hsv, cv::COLOR_BGR2HSV);
                if (tmp_hsv.at<Vec3b>(0, 0)[0] >= 80 && tmp_hsv.at<Vec3b>(0, 0)[1] >= 50) {
                    if (21 <= tmp_hsv.at<Vec3b>(0, 0)[2] && tmp_hsv.at<Vec3b>(0, 0)[2] <= 64)
                        color_num[1] += 1;
                    else if (64 < tmp_hsv.at<Vec3b>(0, 0)[2] && tmp_hsv.at<Vec3b>(0, 0)[2] <= 106)
                        color_num[2] += 1;
                    else if (106 < tmp_hsv.at<Vec3b>(0, 0)[2] && tmp_hsv.at<Vec3b>(0, 0)[2] <= 148)
                        color_num[3] += 1;
                    else if (148 < tmp_hsv.at<Vec3b>(0, 0)[2] && tmp_hsv.at<Vec3b>(0, 0)[2] <= 191)
                        color_num[4] += 1;
                    else if (191 < tmp_hsv.at<Vec3b>(0, 0)[2] && tmp_hsv.at<Vec3b>(0, 0)[2] <= 234)
                        color_num[5] += 1;
                    else
                        color_num[0] += 1;
                }
            }
        }
    }
    ROS_INFO("red:%d, yellow:%d, green:%d, light blue:%d, blue:%d, pink:%d",
             color_num[0], color_num[1], color_num[2], color_num[3], color_num[4], color_num[5]);
    int max_num = 0;
    for (int i = 0; i < 6; i++) {
        if (max_num < color_num[i]) max_num = color_num[i];
    }
    rw = hist.rows;
    cl = hist.cols;
    for (int i = 0; i < rw; i++) {
        for (int j = 0; j < cl; j++) {
            if (0 <= j && j < (int)(cl / 6.0)) {
                if (((double)(rw - i) / rw) <= ((double)color_num[0] / max_num)) {
                    hist.at<Vec3b>(i, j)[0] = 0;
                    hist.at<Vec3b>(i, j)[1] = 0;
                    hist.at<Vec3b>(i, j)[2] = 255;
                }
            }
            if ((int)(cl / 6.0) <= j && j < (int)(2.0 * cl / 6.0)) {
                if (((double)(rw - i) / rw) <= ((double)color_num[1] / max_num)) {
                    hist.at<Vec3b>(i, j)[0] = 0;
                    hist.at<Vec3b>(i, j)[1] = 255;
                    hist.at<Vec3b>(i, j)[2] = 255;
                }
            }
            if ((int)(2.0 * cl / 6.0) <= j && j < (int)(3.0 * cl / 6.0)) {
                if (((double)(rw - i) / rw) <= ((double)color_num[2] / max_num)) {
                    hist.at<Vec3b>(i, j)[0] = 0;
                    hist.at<Vec3b>(i, j)[1] = 255;
                    hist.at<Vec3b>(i, j)[2] = 0;
                }
            }
            if ((int)(3.0 * cl / 6.0) <= j && j < (int)(4.0 * cl / 6.0)) {
                if (((double)(rw - i) / rw) <= ((double)color_num[3] / max_num)) {
                    hist.at<Vec3b>(i, j)[0] = 255;
                    hist.at<Vec3b>(i, j)[1] = 255;
                    hist.at<Vec3b>(i, j)[2] = 0;
                }
            }
            if ((int)(4.0 * cl / 6.0) <= j && j < (int)(5.0 * cl / 6.0)) {
                if (((double)(rw - i) / rw) <= ((double)color_num[4] / max_num)) {
                    hist.at<Vec3b>(i, j)[0] = 255;
                    hist.at<Vec3b>(i, j)[1] = 0;
                    hist.at<Vec3b>(i, j)[2] = 0;
                }
            }
            if ((int)(5.0 * cl / 6.0) <= j && j < cl) {
                if (((double)(rw - i) / rw) <= ((double)color_num[5] / max_num)) {
                    hist.at<Vec3b>(i, j)[0] = 255;
                    hist.at<Vec3b>(i, j)[1] = 0;
                    hist.at<Vec3b>(i, j)[2] = 255;
                }
            }
        }
    }
}

void Exp4::Color2Velocity(const Mat &input, Mat &output) {
    Mat tmp_line = input.clone();
    Mat tmp_roi(input.size(), CV_8UC3);
    Mat filter(input.size(), CV_8UC3);
    Mat hsv(input.size(), CV_8UC3);
    cv::GaussianBlur(input, filter, Size(3, 3), 3);
    cv::cvtColor(filter, hsv, cv::COLOR_BGR2HSV);
    vector<vector<Point>> contours_r;
    vector<vector<Point>> contours_g;
    vector<vector<Point>> contours_b;
    vector<vector<Point>> contours_y;
    int colors = -1;    // qualified color: red, green, blue, yellow
    int max_color = 0;  // pixel numbers of the color that has most pixels
    int color_num = 0;  // pixel numbers of each color
    output = input.clone();

    ColorSplitAuto(hsv, tmp_line, contours_r, red_thresh[0], red_thresh[1], red_thresh[2],
                   red_thresh[3], red_thresh[4], red_thresh[5]);
    GetROI(input, tmp_roi, contours_r);
    color_num = CountROIPixel(tmp_roi);
    if (max_color < color_num && color_num >= colorSizeThresh) {
        max_color = color_num;
        output = tmp_line.clone();
        colors = 0;
    }
    tmp_line = input.clone();
    tmp_roi = Mat::zeros(input.size(), CV_8UC3);

    ColorSplitAuto(hsv, tmp_line, contours_g, green_thresh[0], green_thresh[1], green_thresh[2],
                   green_thresh[3], green_thresh[4], green_thresh[5]);
    GetROI(input, tmp_roi, contours_g);
    color_num = CountROIPixel(tmp_roi);
    if (max_color < color_num && color_num >= colorSizeThresh) {
        max_color = color_num;
        output = tmp_line.clone();
        colors = 1;
    }
    tmp_line = input.clone();
    tmp_roi = Mat::zeros(input.size(), CV_8UC3);

    ColorSplitAuto(hsv, tmp_line, contours_b, blue_thresh[0], blue_thresh[1], blue_thresh[2],
                   blue_thresh[3], blue_thresh[4], blue_thresh[5]);
    GetROI(input, tmp_roi, contours_b);
    color_num = CountROIPixel(tmp_roi);
    if (max_color < color_num && color_num >= colorSizeThresh) {
        max_color = color_num;
        output = tmp_line.clone();
        colors = 2;
    }
    tmp_line = input.clone();
    tmp_roi = Mat::zeros(input.size(), CV_8UC3);

    ColorSplitAuto(hsv, tmp_line, contours_y, yellow_thresh[0], yellow_thresh[1], yellow_thresh[2],
                   yellow_thresh[3], yellow_thresh[4], yellow_thresh[5]);
    GetROI(input, tmp_roi, contours_y);
    color_num = CountROIPixel(tmp_roi);
    if (max_color < color_num && color_num >= colorSizeThresh) {
        max_color = color_num;
        output = tmp_line.clone();
        colors = 3;
    }

    geometry_msgs::Twist vel;
    vel.linear.x = 0;
    vel.linear.y = 0;
    vel.linear.z = 0;
    vel.angular.x = 0;
    vel.angular.y = 0;
    vel.angular.z = 0;
    if (max_color) {
        switch (colors) {
            case 0:
                vel.linear.x = 0.5;
                break;
            case 1:
                vel.linear.x = -0.5;
                break;
            case 2:
                vel.angular.z = 0.4;
                break;
            case 3:
                vel.angular.z = -0.4;
                break;
        }
    }
    vel_pub.publish(vel);
}

int Exp4::CountROIPixel(const Mat &input) {
    int cnt = 0;
    int rw = input.rows;
    int cl = input.cols;
    for (int i = 0; i < rw; i++) {
        for (int j = 0; j < cl; j++) {
            if (input.at<Vec3b>(i, j)[0] != 0 || input.at<Vec3b>(i, j)[1] != 0 || input.at<Vec3b>(i, j)[2] != 0)
                cnt++;
        }
    }
    return cnt;
}

#endif  // EXP04_HPP_
