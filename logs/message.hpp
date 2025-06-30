#ifndef __M_MSG_H__
#define __M_MSG_H__
#include "util.hpp"
#include "level.hpp"
#include <thread>
#include <memory>
// 日志消息类
namespace yhlog
{
    struct LogMsg
    {
        using ptr = std::shared_ptr<LogMsg>;
        size_t _line;           // 行号
        size_t _ctime;           // 时间
        std::thread::id _tid;   // 线程 id
        std::string _name;      // 日志器名称
        std::string _file;      // 日志文件名
        std::string _payload;   // 日志消息
        LogLevel::Value _level; // 日志等级
        
        LogMsg(std::string& name, std::string file, size_t line, std::string&& payload, LogLevel::Value level)
            :_name(name),
            _file(file),
            _payload(payload),
            _level(level),
            _line(line),
            _ctime(util::data::now()),
            _tid(std::this_thread::get_id())
            {
                std::cout << "LogMsg construct success "<< std::endl;
            }
        ~LogMsg()
        {
            std::cout << "LogMsg destruct success " << std::endl;
        }
    };
}
#endif