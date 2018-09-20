#pragma once

#include <opencv2/opencv.hpp>
#include <exception>
#include <ostream>

class GrafikaException : public std::exception
{
    std::string message;
public:
    GrafikaException(const std::string& msg)
        : message(msg)
    { }

    const std::string& GetMessage() const
    {
        return message;
    }

    virtual const char* what() const noexcept override
    {
        return message.c_str();
    }
};

using MainFunc = int(*)(int, char**);

namespace core{
    int CatchExceptions(MainFunc func, int argc, char** argv);

    cv::Mat ReadImage(int argc, char** argv, int flags = CV_LOAD_IMAGE_COLOR);

    class ImageWindow {
        std::string title;
        bool waitKey, waitOnStartup;
    public:
        ImageWindow(const std::string& title,
                    const cv::Mat& mat,
                    bool waitKey = true,
                    bool waitOnStartup = false);
        ~ImageWindow();
    };

    std::string ReadFile(const std::string path);
}
