#ifndef __M_LEVEL_H__
#define __M_LEVEL_H__

// 日志等级类
namespace yhlog
{
    class LogLevel
    {
    public:
        // 定义日志级别枚举
        enum class Value
        {
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        // 将日志级别转换为字符串
        static const char *toString(LogLevel::Value v)
        {
            switch (v)
            {
#define TOSTRING(name) #name
            // 根据日志级别返回对应的字符串
            case LogLevel::Value::DEBUG:
                return TOSTRING(DEBUG);
            case LogLevel::Value::INFO:
                return TOSTRING(INFO);
            case LogLevel::Value::WARN:
                return TOSTRING(WARN);
            case LogLevel::Value::ERROR:
                return TOSTRING(ERROR);
            case LogLevel::Value::FATAL:
                return TOSTRING(FATAL);
            case LogLevel::Value::OFF:
                return TOSTRING(OFF);
#undef TOSTRING
            default:
                return "UNKNOWN";
            }
        }
    };
}

#endif