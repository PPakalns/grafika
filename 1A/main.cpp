#include <stdio.h>
#include <exception>
#include "core/Utility.h"
#include "third_party/gnuplot.h"
#include <sstream>

const char* BGR[3] = {"Blue", "Green", "Red"};

void OutputChannel(GnuplotPipe& gnuPlot, cv::Mat& mat, int channel)
{
    for (int i = 0; i < mat.rows; i++)
    {
        cv::Vec3b *cRow = mat.ptr<cv::Vec3b>(i);;
        for (int j = 0; j < mat.cols; j++, cRow++)
        {
            gnuPlot << i << " " << j << " " << static_cast<int>((*cRow)[channel]) << "\n";
        }
    }
    gnuPlot.sendEndOfData();
}

int safe_main(int argc, char** argv)
{
    cv::Mat image = core::ReadImage(argc, argv);

    core::ImageWindow(argv[1], image);

    GnuplotPipe gnuPlot[3];
    for (int i = 0; i < 3; i++)
    {
        gnuPlot[i] << "set title \"" + std::string(BGR[i]) + " channel \"" << "\n";
        gnuPlot[i] << "set xyplane at 0\n"
                   << "splot '-' with points palette\n";
        OutputChannel(gnuPlot[i], image, i);
    }

    std::cout << "Press enter to quit" << std::endl;
    getchar();
    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
