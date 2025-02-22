/*
 * @file: main.cpp
 * @brief:
 *
 * @author: liuye
 * @date: 2024.12.10
 * @copyright (c) 2013-2024 Honghu Yuntu Corporation
 */

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include "absl/container/flat_hash_set.h"

using namespace std;
using namespace cv;

static void help(const char* progName)
{
    cout << endl
         << "This program shows how to filter images with mask: the write it yourself and the"
         << "filter2d way. " << endl
         << "Usage:" << endl
         << progName << " [image_path -- default lena.jpg] [G -- grayscale] " << endl
         << endl;
}

void Sharpen(const Mat& myImage, Mat& Result);

int main(const int argc, char* argv[])
{
    absl::flat_hash_set<std::string> uniqueSet;
    std::vector<std::string> order;  // 用于存储插入顺序

    // 插入数据
    std::string event = "Event 1";
    if (uniqueSet.insert(event).second) {
        order.push_back(event);  // 插入成功时记录顺序
    }

    event = "Event 2";
    if (uniqueSet.insert(event).second) {
        order.push_back(event);
    }

    event = "Event 1"; // 重复事件，不插入
    if (uniqueSet.insert(event).second) {
        order.push_back(event);
    }

    // 输出插入顺序
    for (const auto& event : order) {
        std::cout << "Event: " << event << "\n";
    }
    help(argv[0]);
    const char* filename = argc >= 2 ? argv[1] : "lena.jpg";

    Mat src, dst0, dst1;

    if (argc >= 3 && !strcmp("G", argv[2]))
        src = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    else
        src = imread(samples::findFile(filename), IMREAD_COLOR);

    if (src.empty())
    {
        cerr << "Can't open image [" << filename << "]" << endl;
        return EXIT_FAILURE;
    }

    namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Output", WINDOW_AUTOSIZE);

    imshow("Input", src);
    double t = static_cast<double>(getTickCount());

    Sharpen(src, dst0);

    t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
    cout << "Hand written function time passed in seconds: " << t << endl;

    imshow("Output", dst0);
    waitKey();

    const Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

    t = static_cast<double>(getTickCount());

    filter2D(src, dst1, src.depth(), kernel);
    t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
    cout << "Built-in filter2D time passed in seconds:     " << t << endl;

    imshow("Output", dst1);

    waitKey();
    return EXIT_SUCCESS;
}

void Sharpen(const Mat& myImage, Mat& Result)
{
    CV_Assert(myImage.depth() == CV_8U); // accept only uchar images

    const int nChannels = myImage.channels();
    Result.create(myImage.size(), myImage.type());

    for (int j = 1; j < myImage.rows - 1; ++j)
    {
        const uchar* previous = myImage.ptr<uchar>(j - 1);
        const uchar* current = myImage.ptr<uchar>(j);
        const uchar* next = myImage.ptr<uchar>(j + 1);

        uchar* output = Result.ptr<uchar>(j);

        for (int i = nChannels; i < nChannels * (myImage.cols - 1); ++i)
        {
            output[i] = saturate_cast<uchar>(5 * current[i] - current[i - nChannels] - current[i + nChannels] -
                                             previous[i] - next[i]);
        }
    }

    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows - 1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols - 1).setTo(Scalar(0));
}
