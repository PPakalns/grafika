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

cv::Mat blurFilter(long long size)
{
    if (size <= 0 || (size & 1) == 0)
    {
        throw GrafikaException("Blur filter size must be an odd positive integer");
    }

    return cv::Mat::ones(size, size, CV_32F) / (size * size);
}

int safe_main(int argc, char** argv)
{
    cv::Mat image{core::ReadImage(argc, argv)};

    core::ImageWindow(argv[1], image);

    cv::Mat test = cv::Mat::ones(3, 3, CV_32F);
    test /= test.rows * test.cols;

    cv::Mat res;
    res = applyFilterOnImage(image, blurFilter(15));
    core::ImageWindow(argv[1], res);

    res = applyFilterOnImage(image, blurFilter(201));
    core::ImageWindow(argv[1], res);

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
