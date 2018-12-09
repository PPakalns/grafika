#include "Utility.h"

int core::CatchExceptions(MainFunc func, int argc, char** argv)
{
    try {
        func(argc, argv);
        return 0;
    } catch(const GrafikaException& e) {
        std::cout << "Execution failed with the following error:" << std::endl;
        std::cout << e.GetMessage() << std::endl;
        return -1;
    } catch(const std::exception& e) {
        std::cout << "Unexpected exception:" << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }
}

cv::Mat core::ReadImage(int argc, char** argv, int flags, int argpos)
{
    if (argc <= argpos)
    {
        std::ostringstream oss;
        oss << "Usage: Argument" << argpos << " must provide path to image\n";
        throw GrafikaException(oss.str());
    }

    cv::Mat image = cv::imread(argv[argpos], flags);

    if (image.data == nullptr)
    {
        std::ostringstream oss;
        oss << "Failed to read image " << argv[argpos] << "\n";
        throw GrafikaException(oss.str());
    }

    return image;
}

std::string core::ReadFile(const std::string path)
{
	std::ifstream stream(path, std::ios::in);
	if(stream.is_open()){
        stream.seekg(0, stream.end);
        auto length = stream.tellg();
        stream.seekg(0, stream.beg);
        std::vector<char> source(static_cast<size_t>(length));
        stream.read(source.data(), length);
        return std::string(source.begin(), source.end());
	}else{
        throw GrafikaException("Failed to read file" + path);
	}
}

core::ImageWindow::ImageWindow(const std::string& titleArg,
                               const cv::Mat& mat,
                               bool waitKeyArg,
                               bool waitOnStartupArg)
    : title(titleArg)
    , waitKey(waitKeyArg)
    , waitOnStartup(waitOnStartupArg)
{
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, 1024, 768);
    cv::imshow(title, mat);

    if (waitOnStartup && waitKey)
    {
        std::cout << "Press any key when focusing window to close it" << std::endl;
        cv::waitKey();
        this->waitKey = false;
    }
}

core::ImageWindow::~ImageWindow()
{
    if (waitKey)
    {
        std::cout << "Press any key when focusing window to close it" << std::endl;
        cv::waitKey();
    }
    cv::destroyWindow(title);
}

