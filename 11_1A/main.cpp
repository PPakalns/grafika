#include <opencv2/opencv.hpp>
#include "core/Utility.h"
#include <assert.h>

static cv::Mat MakeCorrelation(cv::Mat image, cv::Mat mask)
{
    assert(image.type() == CV_8UC3);
    assert(mask.type() == CV_8UC3);

    image.convertTo(image, CV_BGR2YCrCb);
    mask.convertTo(mask, CV_BGR2YCrCb);

    std::vector<cv::Mat> limage(3), lmask(3);
    {
        cv::Mat fimage, fmask;
        image.convertTo(fimage, CV_32FC3);
        mask.convertTo(fmask, CV_32FC3);
        cv::split(fimage, limage);
        cv::split(fmask, lmask);
    }

    cv::Mat res = cv::Mat::zeros(image.rows, image.cols, CV_32F);
    for (int i = 0; i < 3; i++)
    {
        limage[i] /= 255;
        lmask[i] /= 255;
        auto mean = cv::mean(lmask[i]);
        limage[i] -= mean;
        lmask[i] -= mean;

        cv::Mat ilayer, flayer;
        cv::copyMakeBorder(lmask[i], flayer,
                0, image.rows - mask.rows,
                0, image.cols - mask.cols,
                cv::BORDER_CONSTANT);

        cv::merge(std::vector<cv::Mat>{limage[i], cv::Mat::zeros(limage[i].size(), CV_32F)}, ilayer);
        cv::merge(std::vector<cv::Mat>{flayer, cv::Mat::zeros(flayer.size(), CV_32F)}, flayer);
        cv::dft(ilayer, ilayer);
        cv::dft(flayer, flayer);
        cv::mulSpectrums(ilayer, flayer, ilayer, 0);
        cv::dft(ilayer, ilayer, cv::DFT_INVERSE);
        
        std::vector<cv::Mat> tmp(2);
        cv::split(ilayer, tmp);
        ilayer = tmp[0];

        assert(ilayer.type() == CV_32F);

        // Show output for spewcific layer
        cv::Mat outputForLayer;
        cv::normalize(ilayer, outputForLayer, 0, 1, CV_MINMAX);
        core::ImageWindow("Layer " + std::to_string(i), outputForLayer);

        res += ilayer;
    }

    cv::Mat shiftedOutput(res.size(), CV_32F);
    // Shift image
    int shiftX = mask.cols / 2;
    int shiftY = mask.rows / 2;
    for (int r = 0, tr = res.rows - shiftY; r < res.rows; r++, tr++)
    {
        if (tr >= res.rows)
            tr = 0;

        for (int c = 0, tc = res.cols - shiftX; c < res.cols; c++, tc++)
        {
            if (tc >= res.cols)
                tc = 0;

            shiftedOutput.at<float>(tr, tc) = res.at<float>(r, c);
        }
    }
    cv::normalize(shiftedOutput, shiftedOutput, 0, 1, CV_MINMAX);
    return shiftedOutput;
}

int safe_main(int argc, char** argv)
{
    cv::Mat image{core::ReadImage(argc, argv, CV_LOAD_IMAGE_COLOR, 1)};
    cv::Mat mask{core::ReadImage(argc, argv, CV_LOAD_IMAGE_COLOR, 2)};

    core::ImageWindow(argv[2], mask);
    core::ImageWindow(argv[1], image);

    cv::Mat corr = MakeCorrelation(image, mask);
    core::ImageWindow("Korelācija", corr);
    cv::threshold(corr, corr, 0.8, 0, 3);
    corr.convertTo(mask, CV_8U);
    cv::normalize(corr, corr, 0, 1, CV_MINMAX, -1, mask);
    core::ImageWindow("Korelācija tresholded", corr);
    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
