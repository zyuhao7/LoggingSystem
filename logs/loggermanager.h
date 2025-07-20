#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H
#include "logger.hpp"
#include <mutex>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
namespace yhlog
{
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
            assert(_root_logger && "Failed to initialize root logger");
            _loggers.insert({"root", _root_logger});
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
            if (_loggers.find(name) != _loggers.end())
            {
                throw std::runtime_error("Logger with the same name already exists");
            }
            _loggers.insert({name, logger});
        }
        Logger::ptr getLogger(const std::string &name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _loggers.find(name);
            if (it == _loggers.end())
            {
                throw std::runtime_error("Logger not found: " + name);
            }
            return it->second;
        }
        Logger::ptr rootLogger()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _root_logger;
        }
    };

    class GlobalLoggerBuilder : public Logger::Builder
    {
    public:
        virtual Logger::ptr build()
        {
            if (_logger_name.empty())
            {
                std::cout << "⽇志器名称不能为空！！";
                abort();
            }
            assert(LoggerManager::getInstance().hasLogger(_logger_name) == false);
            if (_formatter.get() == nullptr)
            {
                std::cout << "当前⽇志器：" << _logger_name;
                std::cout << " 未检测到⽇志格式，默认设置为";
                std::cout << "[ %d{%H:%M:%S}%T%t%T[%p]%T[%c]%T%f:%l%T%m%n ]!\n";
                _formatter = std::make_shared<Formatter>();
            }
            if (_sinks.empty())
            {
                std::cout << "当前⽇志器：" << _logger_name;
                std::cout << " 未检测到落地⽅向，默认设置为标准输出!\n";
                _sinks.push_back(std::make_shared<StdoutSink>());
            }
            Logger::ptr lp;
            if (_logger_type == Logger::Type::LOGGER_ASYNC)
            {
                lp = std::make_shared<AsyncLogger>(_logger_name, _formatter, _sinks, _level);
            }
            else
            {
                lp = std::make_shared<SyncLogger>(_logger_name, _formatter, _sinks, _level);
            }
            LoggerManager::getInstance().addLogger(_logger_name, lp);
            return lp;
        }
    };
}

#endif // LOGGERMANAGER_H