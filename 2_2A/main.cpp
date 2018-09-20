#include <stdio.h>
#include <exception>
#include <sstream>
#include <assert.h>

#include <opencv2/opencv.hpp>
#include "core/Utility.h"

const float PI2  = 2 * static_cast<float>(3.141592653589793238463);
const float mPI2  = -1 * PI2;

cv::Mat Dft(const cv::Mat& img) {
    assert(img.type() == CV_8U);
    std::cout << "Calculating DFT" << std::endl;
    cv::Mat output = cv::Mat(img.rows, img.cols, CV_32FC2);

    for (int k = 0; k < img.rows; k++)
    {
        std::cout << "Calculating " << k << " / " << img.rows << "\n";
        for (int l = 0; l < img.cols; l++)
        {
            float tmp1 = 0, tmp2 = 0;

            for (int i = 0; i < img.rows; i++)
            {
                // Couple of optimizations for less divisions, more addition
                const unsigned char* val = img.ptr<unsigned char>(i);
                float sum1 = (k * (float)i) / img.rows;
                float sum_part2 = l / (float)img.cols;

                for (int j = 0; j < img.cols; ++j, ++val)
                {
                    float x = mPI2 * sum1;
                    sum1 += sum_part2;
                    // e^ix = cos x + i sin x
                    tmp1 += *val * cos(x);
                    tmp2 += *val * sin(x);
                }
            }

            float* r = output.ptr<float>(k, l);
            r[0] = tmp1, r[1] = tmp2;
        }
    }
    std::cout << "DFT calculated" << std::endl;
    return output;
}

cv::Mat convertFtToImage(const cv::Mat& inp)
{
    assert(inp.type() == CV_32FC2);

    cv::Mat rmat(inp.rows, inp.cols, CV_32F);
    cv::Mat imat(inp.rows, inp.cols, CV_32F);
    cv::Mat arr[2] = {rmat, imat};
    cv::split(inp, arr);
    cv::Mat output(inp.rows, inp.cols, CV_32F);
    cv::magnitude(rmat, imat, output);
    output += 1;
    cv::log(output, output);
    cv::normalize(output, output, 0, 1, CV_MINMAX);
    return output;
}

void ShowImage(const std::string& title, const cv::Mat& mat)
{
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, 1024, 768);
    cv::imshow(title, mat);
    cv::waitKeyEx(0);
    cv::destroyWindow(title);
}

int safe_main(int argc, char** argv)
{
    cv::Mat image = core::ReadImage(argc, argv, CV_LOAD_IMAGE_GRAYSCALE);
    ShowImage("Grayscale window", image);

    auto fimg = Dft(image);
    ShowImage("Fourier transform", convertFtToImage(fimg));

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
