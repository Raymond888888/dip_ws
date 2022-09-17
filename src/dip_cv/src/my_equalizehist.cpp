#include "my_equalizehist.hpp"
using namespace cv;

// //==========================补充说明一=========================//
// calcBody(heightRange);  //类的（）重载
// void operator()(const cv::Range& rowRange) const {
//     int localHistogram[HIST_SZ] = {
//         0,
//     };

//     const size_t sstep = src_.step;  //每行的字节数

//     int width = src_.cols;
//     int height = rowRange.end - rowRange.start;

//     if (src_.isContinuous())  //图像是否连续
//     {
//         width *= height;
//         height = 1;
//     }

//     for (const uchar* ptr = src_.ptr<uchar>(rowRange.start); height--; ptr += sstep)  //指向Mat矩阵行的首地址，ptr完成一次循环向后移动整行
//     {
//         int x = 0;
//         for (; x <= width - 4; x += 4)  //每4列进行一次处理
//         {
//             int t0 = ptr[x], t1 = ptr[x + 1];
//             localHistogram[t0]++;
//             localHistogram[t1]++;  // int localHistogram[256] = { 0, };int数组；直方图每一维存储数据
//             t0 = ptr[x + 2];
//             t1 = ptr[x + 3];
//             localHistogram[t0]++;
//             localHistogram[t1]++;
//         }

//         for (; x < width; ++x)
//             localHistogram[ptr[x]]++;
//     }

//     cv::AutoLock lock(*histogramLock_);

//     for (int i = 0; i < HIST_SZ; i++)
//         globalHistogram_[i] += localHistogram[i];
// }
// //========================补充说明二======================//
// // lutBody(heightRange);  //类的（）重载
// void operator()(const cv::Range& rowRange) const {
//     const size_t sstep = src_.step;  //每行的字节数
//     const size_t dstep = dst_.step;

//     int width = src_.cols;
//     int height = rowRange.end - rowRange.start;
//     int* lut = lut_;

//     if (src_.isContinuous() && dst_.isContinuous()) {
//         width *= height;
//         height = 1;
//     }

//     const uchar* sptr = src_.ptr<uchar>(rowRange.start);
//     uchar* dptr = dst_.ptr<uchar>(rowRange.start);

//     for (; height--; sptr += sstep, dptr += dstep) {
//         int x = 0;
//         for (; x <= width - 4; x += 4) {
//             int v0 = sptr[x];
//             int v1 = sptr[x + 1];
//             int x0 = lut[v0];
//             int x1 = lut[v1];
//             dptr[x] = (uchar)x0;
//             dptr[x + 1] = (uchar)x1;

//             v0 = sptr[x + 2];
//             v1 = sptr[x + 3];
//             x0 = lut[v0];
//             x1 = lut[v1];
//             dptr[x + 2] = (uchar)x0;
//             dptr[x + 3] = (uchar)x1;
//         }

//         for (; x < width; ++x)
//             dptr[x] = (uchar)lut[sptr[x]];
//     }
// }

// void my_equalizeHist(InputArray _src, OutputArray _dst)  // InputArray接口类,只能作为函数的形参参数使用
// {
//     Mat src = _src.getMat();           //将传入的参数转换为Mat的结构
//     CV_Assert(src.type() == CV_8UC1);  //检查运行情况,如果出现错误，则显示错误信息。

//     _dst.create(src.size(), src.type());  // OutputArray是InputArray的派生类,需要注意在使用_OutputArray：：getMat（）之前一定要调用_OutputArray：：create（）为矩阵分配空间。
//     Mat dst = _dst.getMat();

//     if (src.empty())
//         return;

//     Mutex histogramLockInstance;

//     const int hist_sz = 256;  // HIST_SZ = 256
//     int hist[hist_sz] = {
//         0,
//     };
//     int lut[hist_sz];

//     calcBody(src, hist, &histogramLockInstance);
// lutBody(src, dst, lut);
// cv::Range heightRange(0, src.rows);  //高度范围

//     if (EqualizeHistCalcHist_Invoker::isWorthParallel(src))  //判断原始图像是否大于等于640*480；大于时
//         parallel_for_(heightRange, calcBody);
//     else
//         calcBody(heightRange);  //见补充一

//     int i = 0;
//     while (!hist[i]) ++i;

//     int total = (int)src.total();  //图像中元素总数
//     if (hist[i] == total){
//             dst.setTo(i);  //将全部元素设置为i
//             return;
//         }

//     float scale = (hist_sz - 1.f) / (total - hist[i]);  //?
//     int sum = 0;

//     for (lut[i++] = 0; i < hist_sz; ++i) {
//         sum += hist[i];
//         lut[i] = saturate_cast<uchar>(sum * scale);  //直方图均衡
//     }

//     if (EqualizeHistLut_Invoker::isWorthParallel(src))
//         parallel_for_(heightRange, lutBody);  //?
//     else
//         lutBody(heightRange);
// }

void my_equalizeHist(InputArray _src, OutputArray _dst)  // InputArray接口类,只能作为函数的形参参数使用
{
    Mat src = _src.getMat();  //将传入的参数转换为Mat的结构
    ROS_INFO("mat: %d %d",src.rows,src.cols);
    ROS_INFO("mat(0,0): %d ",src.at<uchar>(719, 1279));
    // for(long int i=0;i<)
}