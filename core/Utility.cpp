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

cv::Mat core::ReadImage(int argc, char** argv, int flags)
{
    if (argc < 2)
    {
        std::ostringstream oss;
        oss << "Usage: " << argv[0] << " <path to image>\n";
        throw GrafikaException(oss.str());
    }

    cv::Mat image = cv::imread(argv[1], flags);

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

