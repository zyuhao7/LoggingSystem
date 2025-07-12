#ifndef __M_LOG_H__
#define __M_LOG_H__
#include "util.hpp"
#include "level.hpp"
#include "message.hpp"
#include "formatter.hpp"
#include "sink.hpp"
#include <vector>
#include <list>
#include <atomic>
#include <unordered_map>
#include <cstdarg>
#include <type_traits>

namespace yhlog
{
    class Logger
    {
     public:
        enum class Type{
            LOGGER_SYNC = 0,
            LOGGER_ASYNC
        };
        using ptr = std::shared_ptr<Logger>;

        Logger(const std::string& name,
        Formatter::ptr formatter,
        std::vector<LogSink::ptr>& sinks,
        LogLevel::Value level = LogLevel::Value::DEBUG)
        : _name(name),
        _level(level),
        _formatter(formatter),
        _sinks(sinks.begin(), sinks.end()){}

        std::string loggerName(){return _name;}
        LogLevel::Value loggerLevel(){return _level;}

        void debug(const char* file, size_t line, const char* fmt, ...)
        {
            if(shouldLog(LogLevel::Value::DEBUG) == false) return;

            va_list al;
            va_start(al, fmt);
            log(LogLevel::Value::DEBUG, file, line, fmt, al);
            va_end(al);
        }

         void info(const char* file, size_t line, const char* fmt, ...)
        {
            if(shouldLog(LogLevel::Value::INFO) == false) return;

            va_list al;
            va_start(al, fmt);
            log(LogLevel::Value::INFO, file, line, fmt, al);
            va_end(al);
        }

         void warn(const char* file, size_t line, const char* fmt, ...)
        {
            if(shouldLog(LogLevel::Value::WARN) == false) return;

            va_list al;
            va_start(al, fmt);
            log(LogLevel::Value::WARN, file, line, fmt, al);
            va_end(al);
        }

         void fatal(const char* file, size_t line, const char* fmt, ...)
        {
            if(shouldLog(LogLevel::Value::FATAL) == false) return;

            va_list al;
            va_start(al, fmt);
            log(LogLevel::Value::FATAL, file, line, fmt, al);
            va_end(al);
        }

         void error(const char* file, size_t line, const char* fmt, ...)
        {
            if(shouldLog(LogLevel::Value::ERROR) == false) return;

            va_list al;
            va_start(al, fmt);
            log(LogLevel::Value::ERROR, file, line, fmt, al);
            va_end(al);
        }


    public:
        class Builder
        {
         public:
                using ptr = std::shared_ptr<Builder>;
                Builder()
                :_level(LogLevel::Value::DEBUG), 
                _logger_type(Logger::Type::LOGGER_SYNC) {}
                void buildLoggerName(const std::string &name) 
                {
                     _logger_name = name; 
                }
                void buildLoggerLevel(LogLevel::Value level) 
                {
                     _level = level; 
                }
                void buildLoggerType(Logger::Type type) 
                {
                     _logger_type = type; 
                }
                void buildFormatter(const std::string pattern) 
                { 
                    _formatter = std::make_shared<Formatter>(pattern); 
                }
                void buildFormatter(const Formatter::ptr &formatter) 
                { 
                       _formatter = formatter; 
                }

                template<typename SinkType, typename ...Args>
                void buildSink(Args &&...args) 
                { 
                    auto sink = SinkFactory::create<SinkType>(std::forward<Args>(args)...);
                    _sinks.push_back(sink); 
                }

                virtual Logger::ptr build() = 0;

            protected:
                Logger::Type _logger_type;
                std::string _logger_name;
                LogLevel::Value _level;
                Formatter::ptr _formatter;
                std::vector<LogSink::ptr> _sinks;
        };

    protected:
        bool shouldLog(LogLevel::Value level) { return level >= _level; }
        void log(LogLevel::Value level, const char* file, size_t line, const char* fmt, va_list al)
        {
            char* buf;
            std::string msg;
            int len = vasprintf(&buf,fmt, al);
            if(len < 0)
            {
                msg = "格式化日志消息失败!";
            }
            else
            {
                msg.assign(buf, len);
                free(buf);
            }
            LogMsg lm(_name, file, line, std::move(msg), level);
            std::stringstream ss;
            _formatter->format(ss, lm);
            LogIt(std::move(ss.str()));
        }

        virtual void LogIt(const std::string& msg) = 0;

        std::mutex _mutex;
        std::string _name;
        Formatter::ptr _formatter;
        std::atomic<LogLevel::Value> _level;
        std::vector<LogSink::ptr> _sinks;

    };

    class SyncLogger : public Logger 
    {
        public:
            using ptr = std::shared_ptr<SyncLogger>;
            SyncLogger(const std::string &name, 
            Formatter::ptr formatter,
            std::vector<LogSink::ptr> &sinks, 
            LogLevel::Value level = LogLevel::Value::DEBUG)
            : Logger(name, formatter, sinks, level)
            { 
                std::cout << LogLevel::toString(level)<<" 同步⽇志器: "<< name << "创建成功...\n";
            }
        private:
            virtual void logIt(const std::string &msg) 
            {
                std::unique_lock<std::mutex> lock(_mutex);
                if (_sinks.empty()) { return ; }
                for (auto &it : _sinks) 
                {
                    it->log(msg.c_str(), msg.size());
                }
            }
    };



class LocalLoggerBuilder: public Logger::Builder {
    public:
        virtual Logger::ptr build() 
        {
            if (_logger_name.empty())
            {
                std::cout << "⽇志器名称不能为空！！";
                abort();
            }
        if (_formatter.get() == nullptr) 
        {
            std::cout<<"当前⽇志器：" << _logger_name;
            std::cout<<" 未检测到⽇志格式,默认设置为: ";
            std::cout<<" %d{%H:%M:%S}%T%t%T[%p]%T[%c]%T%f:%l%T%m%n\n";
            _formatter = std::make_shared<Formatter>();
        }
        if (_sinks.empty()) 
        {
            std::cout<<"当前⽇志器："<<_logger_name<<" 未检测到落地⽅向，默认为标准输出!\n";
            _sinks.push_back(std::make_shared<StdoutSink>());
        }
        Logger::ptr lp;

        if(_logger_type == Logger::Type::LOGGER_ASYNC) 
        {
            // lp = std::make_shared<AsyncLogger>(_logger_name,_formatter, 
            // _sinks, _level);
        }
        else
        {
            lp = std::make_shared<SyncLogger>(_logger_name, _formatter, _sinks, _level);
        }
        return lp;
        }
    };
}




#endif