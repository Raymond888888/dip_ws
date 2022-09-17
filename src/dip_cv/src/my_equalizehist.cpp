#include "my_equalizehist.hpp"
using namespace cv;

void my_equalizeHist(InputArray _src, OutputArray _dst)  // InputArray接口类,只能作为函数的形参参数使用
{
    Mat src = _src.getMat();  //将传入的参数转换为Mat的结构
    _dst.create(src.size(), src.type());
    Mat dst = _dst.getMat();
    long int imgsize = src.rows * src.cols;
    long int pix_sum[255] = {0};
    ROS_INFO("mat: %ld %d", src.size, src.cols);  // rows:720 cols:1280
    ROS_INFO("mat(0,0): %d ", src.at<uchar>(719, 1279));
    for (long int r = 0; r < src.rows; r++) {
        for (long int c = 0; c < src.cols; c++) {
            pix_sum[src.at<uchar>(r, c)]++;
        }
    }

    double pro = 0, pro_sum = 0;
    uint8_t s[256] = {0};
    for (int r = 0; r < 256; r++) {
        pro = (double)pix_sum[r] / (double)imgsize;
        pro_sum += pro;
        s[r] = 255.0 * pro_sum;
    }

    for (long int r = 0; r < src.rows; r++) {
        for (long int c = 0; c < src.cols; c++) {
            dst.at<uchar>(r, c) = s[src.at<uchar>(r, c)];
        }
    }
}