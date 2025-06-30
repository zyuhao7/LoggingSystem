#ifndef __M_LEVEL_H__
#define __M_LEVEL_H__

namespace yhlog
{
    class LogLevel
    {
        public:
        enum class Value
        {
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        static const char* toString(LogLevel::Value v)
        {
            switch (v)
            {
#define TOSTRING(name) #name
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