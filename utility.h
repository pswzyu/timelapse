#ifndef UTILITY_H
#define UTILITY_H

#include <QImage>
#include <opencv2/opencv.hpp>


class Utility
{
public:
    Utility();

    static QImage convertImage(const cv::Mat& mat);
};

#endif // UTILITY_H
