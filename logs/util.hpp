/*
    通用功能类，与业务无关的功能实现
        1. 获取系统时间
        2. 获取文件大小
        3. 创建目录
        4. 获取文件所在目录
*/
#ifndef __M_UTIL_H__
#define __M_UTIL_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cassert>
#include <sys/stat.h>

namespace yhlog
{
    namespace util
    {
        class data
        {
        public:
            static size_t now() { return (size_t)time(nullptr); }
        };
        class file
        {
        public:
            static bool exists(const std::string &name)
            {
                struct stat st;
                return stat(name.c_str(), &st) == 0;
            }
            // 根据文件名获取文件路径
            static std::string path(const std::string &name)
            {
                // 如果文件名为空，则返回当前目录
                if (name.empty())
                    return ".";
                // 获取文件名中最后一个斜杠或反斜杠的位置
                size_t pos = name.find_last_of("/\\");
                // 如果没有找到斜杠或反斜杠，则返回当前目录
                if (pos == std::string::npos)
                    return ".";
                // 返回文件路径
                return name.substr(0, pos + 1);
            }
            static void create_directory(const std::string &path)
            {
                // ./abc/bcd/a.txt
                if (path.empty())
                    return;
                if (exists(path))
                    return;
                size_t pos, idx = 0;
                while (idx < path.size())
                {
                    pos = path.find_first_of("/\\", idx);
                    // abc
                    if (pos == std::string::npos)
                    {
                        mkdir(path.c_str(), 0755);
                        return;
                    }
                    // 当出现 //
                    if (pos == idx)
                    {
                        idx = pos + 1;
                        continue;
                    }
                    // ./abc/bcd/a.txt
                    std::string subdir = path.substr(0, pos);
                    if (subdir == "." || subdir == ".." || exists(subdir))
                    {
                        idx = pos + 1;
                        continue;
                    }
                    mkdir(subdir.c_str(), 0755);
                    idx = pos + 1;
                }
            }
        };
    }
}


#endif