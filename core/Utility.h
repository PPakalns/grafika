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

    cv::Mat ReadImage(int argc, char** argv);
}
