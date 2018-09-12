#include <core/Utility.h>

int safe_main(int argc, char** argv )
{
    cv::Mat image = core::ReadImage(argc, argv);
    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
