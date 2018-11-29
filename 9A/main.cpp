#include <opencv2/opencv.hpp>
#include "core/Utility.h"
#include <assert.h>

cv::Mat applyFilterOnImage(cv::Mat image, cv::Mat filter)
{
    assert(image.type() == CV_8UC3);
    assert(filter.type() == CV_32F);

    if (filter.rows > image.rows || filter.cols > image.cols)
    {
        throw GrafikaException("Filter larger than image");
    }

    // Prepare filter
    cv::Mat filterIm;
    cv::copyMakeBorder(filter, filterIm,
                       0, image.rows - filter.rows,
                       0, image.cols - filter.cols,
                       cv::BORDER_CONSTANT);
    cv::merge(std::vector<cv::Mat>{filterIm, cv::Mat::zeros(filterIm.size(), CV_32F)}, filterIm);
    cv::dft(filterIm, filterIm);


    // Prepare image
    cv::Mat fimage;
    std::vector<cv::Mat> layers(3);
    image.convertTo(fimage, CV_32FC3);
    cv::split(fimage, layers);

    // Apply filter to each channel
    for (size_t i = 0; i < layers.size(); i++)
    {
        cv::merge(std::vector<cv::Mat>{layers[i], cv::Mat::zeros(layers[i].size(), CV_32F)}, layers[i]);
        cv::dft(layers[i], layers[i]);
        cv::mulSpectrums(layers[i], filterIm, layers[i], 0);
        cv::dft(layers[i], layers[i], cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
        cv::normalize(layers[i], layers[i], 0, 255, CV_MINMAX);
    }

    // Merge channels
    cv::Mat res;
    cv::merge(layers, res);
    res.convertTo(res, CV_8UC3);
    cv::Mat shiftedOutput(res.size(), CV_8UC3);

    // Shift image
    int shiftX = filter.cols / 2;
    int shiftY = filter.rows / 2;
    for (int r = 0, tr = res.rows - shiftY; r < res.rows; r++, tr++)
    {
        if (tr >= res.rows)
            tr = 0;

        for (int c = 0, tc = res.cols - shiftX; c < res.cols; c++, tc++)
        {
            if (tc >= res.cols)
                tc = 0;

            memcpy(shiftedOutput.at<char[3]>(tr, tc), res.at<char[3]>(r, c), sizeof(char[3]));
        }
    }

    return shiftedOutput;
}

cv::Mat avgBlurFilter(long long r)
{
    assert(r >= 0);
    auto size = 2 * r + 1;

    return cv::Mat::ones(size, size, CV_32F) / (size * size);
}

cv::Mat gaussianBlurFilter(long long radius, double sigma)
{
    assert(r >= 0);
    long long size = 2 * radius + 1;
    double bmult = 2 * sigma * sigma;

    cv::Mat tmp(size, size, CV_32F);
    for (int r = 0; r < tmp.rows; r++)
    {
        for (int c = 0; c < tmp.cols; c++)
        {
            auto dx = radius - r;
            auto dy = radius - c;
            auto dd = dx * dx + dy * dy;
            tmp.at<float>(r, c) = exp(-dd/bmult);
        }
    }
    double sum = cv::sum(tmp)[0];
    return tmp / sum;
}

cv::Mat testOffsetFilter(long long r)
{
    assert(r >= 0);
    auto size = 2 * r + 1;

    cv::Mat test = cv::Mat::zeros(size, size, CV_32F);
    test.at<float>(size / 2, size / 2) = 1.0;
    return test;
}

int safe_main(int argc, char** argv)
{
    cv::Mat image{core::ReadImage(argc, argv)};

    core::ImageWindow(argv[1], image);

    int avgR = std::atoi(argv[2]);
    int gaussianR = std::atoi(argv[3]);
    int sigma = std::atoi(argv[4]);
    // int sharpR = std::atoi(argv[4]);

    cv::Mat res;
    res = applyFilterOnImage(image, avgBlurFilter(avgR));
    core::ImageWindow("Videjas vertibas izpludināšanas filtrs", res);

    res = applyFilterOnImage(image, gaussianBlurFilter(gaussianR, sigma));
    core::ImageWindow("Gausa izpludināšanas filtrs", res);

    // res = applyFilterOnImage(image, sharpBlurFilter(sharpR));
    // core::ImageWindow("Assināšanas filtrs", res);

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
