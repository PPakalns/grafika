#include <opencv2/opencv.hpp>
#include "core/Utility.h"

cv::Mat equalizeHist(cv::Mat image)
{
    assert(image.type() == CV_8U);

    // Count pixels with each unique luminiscence value
    std::vector<unsigned int> pixelCount(256);
    uchar minval = 255, maxval = 0;
    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            uchar lum = image.at<uchar>(y, x);
            ++pixelCount[lum];
            minval = std::min(minval, lum);
            maxval = std::max(maxval, lum);
        }
    }

    // Create mapping from [0; 256) -> [0; 1]
    std::vector<float> mapTo(pixelCount.size());

    unsigned int total = image.rows * image.cols;

    // Keep maximum colors at their maximum
    mapTo[minval] = 0;
    mapTo[maxval] = 1;

    unsigned int prefixSum = pixelCount[minval];
    for (size_t i = minval + 1; i < maxval; i++)
    {
        mapTo[i] = (prefixSum + pixelCount[i] / 2.0) / total;
        prefixSum += pixelCount[i];
    }

    // Create new image with the new distribution in [0; 1]
    cv::Mat result(image.rows, image.cols, CV_32F);
    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            result.at<float>(y, x) = mapTo[image.at<uchar>(y, x)];
        }
    }

    // Convert back to [0; 255]
    result = result * 255;
    result.convertTo(result, CV_8U);

    return result;
}

int safe_main(int argc, char** argv)
{
    cv::Mat image{core::ReadImage(argc, argv)};

    core::ImageWindow(argv[1], image);

    cv::cvtColor(image, image, CV_BGR2YCrCb);

    std::vector<cv::Mat> ch;
    cv::split(image, ch);

    // Copy luminiscence layer
    cv::Mat tmpChannel;
    ch[0].copyTo(tmpChannel);

    // CV equalization for comparision
    cv::equalizeHist(tmpChannel, ch[0]);
    cv::Mat cvResult;
    cv::merge(ch, cvResult);
    cv::cvtColor(cvResult, cvResult, CV_YCrCb2BGR);
    core::ImageWindow("Opencv result", cvResult);

    // My equalization
    ch[0] = equalizeHist(tmpChannel);
    cv::Mat myResult;
    cv::merge(ch, myResult);
    cv::cvtColor(myResult, myResult, CV_YCrCb2BGR);
    core::ImageWindow("My result", myResult);

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
