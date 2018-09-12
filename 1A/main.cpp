#include <stdio.h>
#include <exception>
#include "core/Utility.h"

int safe_main(int argc, char** argv)
{
    cv::Mat image = core::ReadImage(argc, argv);

    // TODO

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
