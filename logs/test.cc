#include "util.hpp"
#include "level.hpp"
#include <iostream>
using namespace yhlog::util;

int main() {
    
    std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::DEBUG) << std::endl;
    std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::INFO) << std::endl;
    std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::WARN) << std::endl;
    std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::ERROR) << std::endl;
    std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::FATAL) << std::endl;
    std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::OFF) << std::endl;



    // std::string fullPath = "./test_dir1/test_dir2/test_file.txt";
    // std::string dirPath = file::path(fullPath);  // 提取目录部分 ./test_dir1/test_dir2/
    // std::cout << "Creating directory: " << dirPath << std::endl;

    // file::create_directory(dirPath);

    // if (file::exists(dirPath)) 
    // {
    //     std::cout << "Directory created successfully: " << dirPath << std::endl;
    // } 
    // else
    // {
    //     std::cout << "Failed to create directory: " << dirPath << std::endl;
    // }

    return 0;
}
