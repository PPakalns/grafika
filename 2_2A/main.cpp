#include <stdio.h>
#include <exception>
#include <sstream>
#include <assert.h>

#include <opencv2/opencv.hpp>
#include "core/Utility.h"


const double PI  = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

cv::Mat Dft(const cv::Mat& img, bool inv = false) {
    assert(img.type() == CV_32FC2);
    std::cout << "Calculating DFT" << std::endl;

    cv::Mat output = cv::Mat(img.rows, img.cols, CV_32FC2);
    std::vector<double[2]> P(static_cast<size_t>(output.cols));

    const double u2PI = (inv ? 2 : -2) * PI;
    double normal = (inv ? output.rows * output.cols : 1);

    for (int k = 0; k < output.rows; k++)
    {
        std::cout << "Calculating " << k << " / " << img.rows << "\n";

        // Calculating P(k, b)
        for (int b = 0; b < output.cols; b++)
        {
            double r0 = 0, r1 = 0;
            for (int a = 0; a < output.rows; a++)
            {
                double val = u2PI * (k * (double)a / output.rows);
                double c = cos(val), d = sin(val);
                const float* inVal = img.ptr<float>(a, b);
                r0 += inVal[0] * c - inVal[1] * d;
                r1 += inVal[0] * d + inVal[1] * c;
            }
            P[b][0] = r0;
            P[b][1] = r1;
        }

        // Calculating output(k, l)
        float* res = output.ptr<float>(k);
        for (int l = 0; l < output.cols; l++, res += 2)
        {
            double r0 = 0, r1 = 0;
            for (int b = 0; b < output.cols; b++)
            {
                // e^ix = cos x + i sin x
                double val = u2PI * (l * static_cast<double>(b) / output.cols);
                double c = cos(val), d = sin(val);
                r0 += P[b][0] * c - P[b][1] * d;
                r1 += P[b][0] * d + P[b][1] * c;
            }
            res[0] = r0 / normal;
            res[1] = r1 / normal;
        }
    }
    std::cout << "DFT calculated" << std::endl;
    return output;
}

cv::Mat AddZeroLayer(const cv::Mat& img)
{
    cv::Mat imageI;
    cv::Mat mat[] = {img, cv::Mat::zeros(img.size(), CV_32F)};
    cv::merge(mat, 2, imageI);
    return imageI;
}

cv::Mat IDft(const cv::Mat& img)
{
    return Dft(img, true);
}

std::tuple<cv::Mat, cv::Mat> GetComplexParts(const cv::Mat& inp)
{
    cv::Mat rmat(inp.rows, inp.cols, CV_32F);
    cv::Mat imat(inp.rows, inp.cols, CV_32F);
    cv::Mat arr[2] = {rmat, imat};
    cv::split(inp, arr);
    return std::make_tuple(rmat, imat);
}

cv::Mat convertFtToImage(const cv::Mat& inp)
{
    assert(inp.type() == CV_32FC2);
    cv::Mat rmat, imat;
    std::tie(rmat, imat) = GetComplexParts(inp);
    cv::Mat output(inp.rows, inp.cols, CV_32F);
    cv::magnitude(rmat, imat, output);
    output += 1;
    cv::log(output, output);
    cv::normalize(output, output, 0, 1, CV_MINMAX);
    return output;
}

int safe_main(int argc, char** argv)
{
    cv::Mat image;
    core::ReadImage(argc, argv, CV_LOAD_IMAGE_GRAYSCALE).convertTo(image, CV_32F);
    core::ImageWindow("Grayscale window", image / 255);

    // DFT
    auto fimg = Dft(AddZeroLayer(image));
    core::ImageWindow("My fourier transform", convertFtToImage(fimg));

    /*
    // OPENCV fourier transform for comparision
    cv::Mat imageI = AddZeroLayer(image);
    cv::dft(imageI, imageI);
    ShowImage("OPENCV Fourier transform", convertFtToImage(imageI));

    cv::dft(imageI, imageI, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    cv::Mat cvrealp;
    tie(cvrealp, std::ignore) = GetComplexParts((imageI));
    cv::normalize(cvrealp, cvrealp, 0, 1, CV_MINMAX);
    ShowImage("OPENCV Inverse Fourier transform", cvrealp);
    // Testing purpose end
    */

    // Inverse DFT
    cv::Mat realp;
    tie(realp, std::ignore) = GetComplexParts(IDft(fimg));
    cv::normalize(realp, realp, 0, 1, CV_MINMAX);
    core::ImageWindow("My inverse discrete fourier transform", realp);
    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
