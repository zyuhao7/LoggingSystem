#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H
#include "logger.hpp"
#include <mutex>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
using namespace yhlog;
class LoggerManager
{
private:
    std::mutex _mutex;
    Logger::ptr _root_logger;
    std::unordered_map<std::string, Logger::ptr> _loggers;

private:
    LoggerManager()
    {
        std::unique_ptr<LocalLoggerBuilder> slb(new LocalLoggerBuilder());
        slb->buildLoggerName("root");
        slb->buildLoggerType(Logger::Type::LOGGER_SYNC);
        _root_logger = slb->build();
        _loggers.insert(std::make_pair("root", _root_logger));
    }
    LoggerManager(const LoggerManager &) = delete;
    LoggerManager &operator=(const LoggerManager &) = delete;

public:
    // 获取loggerManager实例的静态方法
    static LoggerManager &getInstance()
    {
        static LoggerManager lm;
        return lm;
    }
    bool hasLogger(const std::string &name)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto it = _loggers.find(name);
        if (it == _loggers.end())
        {
            return false;
        }
        return true;
    }
    void addLogger(const std::string &name, const Logger::ptr logger)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _loggers.insert(std::make_pair(name, logger));
    }
    Logger::ptr getLogger(const std::string &name)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto it = _loggers.find(name);
        if (it == _loggers.end())
        {
            return Logger::ptr();
        }
        return it->second;
    }
    Logger::ptr rootLogger()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _root_logger;
    }
};

#endif // LOGGERMANAGER_H