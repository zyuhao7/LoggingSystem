#include "util.hpp"
#include <iostream>

int main() {
    using namespace yhlog::util;

    std::string fullPath = "./test_dir1/test_dir2/test_file.txt";
    std::string dirPath = file::path(fullPath);  // 提取目录部分 ./test_dir1/test_dir2/
    std::cout << "Creating directory: " << dirPath << std::endl;

    file::create_directory(dirPath);

    if (file::exists(dirPath)) 
    {
        std::cout << "Directory created successfully: " << dirPath << std::endl;
    } 
    else
     {
        std::cout << "Failed to create directory: " << dirPath << std::endl;
    }

    return 0;
}
