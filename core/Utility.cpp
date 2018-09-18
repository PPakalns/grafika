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

cv::Mat core::ReadImage(int argc, char** argv)
{
    if (argc < 2)
    {
        std::ostringstream oss;
        oss << "Usage: " << argv[0] << " <path to image>\n";
        throw GrafikaException(oss.str());
    }

    cv::Mat image = cv::imread(argv[1]);

    if (image.data == nullptr)
    {
        std::ostringstream oss;
        oss << "Failed to read image " << argv[1] << "\n";
        throw GrafikaException(oss.str());
    }

    return image;
}

std::string core::ReadFile(const std::string path)
{
	std::string content;
	std::ifstream stream(path, std::ios::in);
	if(stream.is_open()){
		std::stringstream sstr;
		sstr << stream.rdbuf();
        return sstr.str();
	}else{
        throw GrafikaException("Failed to read file" + path);
	}
}

