#ifndef __M_FMT_H__
#define __M_FMT_H__

#include "util.hpp"
#include "message.hpp"
#include "level.hpp"
#include <memory>
#include <vector>
#include <tuple>

namespace yhlog
{
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, const LogMsg& msg) = 0;
    };

    class MsgFormatItem : public FormatItem
    {
    public:
        MsgFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << msg._payload;
        }
    };

    class LevelFormatItem : public FormatItem
    {
    public:
        LevelFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << LogLevel::toString(msg._level);
        }
    };

    class NameFormatItem : public FormatItem
    {
    public:
        NameFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << msg._name;
        }
    };

    class ThreadFormatItem : public FormatItem
    {
    public:
        ThreadFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << msg._tid;
        }
    };

    class TimeFormatItem : public FormatItem
    {
    private:
        std::string _format;

    public:
        TimeFormatItem(const std::string &format = "%H:%M:%S") // 默认是 时:分:秒
            : _format(format)
        {
            if (format.empty())
                _format = "%H:%M:%S";
        }
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            time_t t = msg._ctime;
            struct tm lt;
            localtime_r(&t, &lt);
            char tmp[128];
            // 根据 _format 格式, 将 &lt format to tmp.
            strftime(tmp, 127, _format.c_str(), &lt);
            os << tmp;
        }
    };

     class CFileFormatItem : public FormatItem
    {
    public:
        CFileFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << msg._file;
        }
    };

    class CLineFormatItem : public FormatItem
    {
    public:
        CLineFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << msg._line;
        }
    };

    class TabFormatItem : public FormatItem
    {
    public:
        TabFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << "\t";
        }
    };

    class NLineFormatItem : public FormatItem
    {
    public:
        NLineFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << "\n";
        }
    };

    class OtherFormatItem : public FormatItem
    {
    private:
        std::string _str;

    public:
        OtherFormatItem(const std::string &str = "")
            : _str(str)
        {
        }
        virtual void format(std::ostream &os, const LogMsg &msg)
        {
            os << _str;
        }
    };

    class Formatter
    {
    public:
      using ptr = std::shared_ptr<Formatter>;
        /*
            %d 日期
            %T 缩进
            %t 线程id
            %p 日志级别
            %c 日志器名称
            %f 文件名
            %m 日志消息
            %n 换行
        */
       Formatter(const std::string& pattern =  "[%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n")
        :_pattern(pattern)
       {
            assert(parsePattern());
       }

       const std::string pattern() {return _pattern;}
       std::string format(const LogMsg& msg)
       {
            std::stringstream ss;
            for(auto& it : _items)
            {
                it->format(ss, msg);
            }
            return ss.str();
       }

       std::ostream& format(std::ostream& os, const LogMsg& msg)
       {
            for(auto& it : _items)
            {
                it->format(os, msg);
            }
            return os;
       }

        FormatItem::ptr createItem(const std::string &fc, const std::string &subfmt)
        {
            if (fc == "m")
                return FormatItem::ptr(new MsgFormatItem(subfmt));
            if (fc == "p")
                return FormatItem::ptr(new LevelFormatItem(subfmt));
            if (fc == "c")
                return FormatItem::ptr(new NameFormatItem(subfmt));
            if (fc == "t")
                return FormatItem::ptr(new ThreadFormatItem(subfmt));
            if (fc == "n")
                return FormatItem::ptr(new NLineFormatItem(subfmt));
            if (fc == "d")
                return FormatItem::ptr(new TimeFormatItem(subfmt));
            if (fc == "f")
                return FormatItem::ptr(new CFileFormatItem(subfmt));
            if (fc == "l")
                return FormatItem::ptr(new CLineFormatItem(subfmt));
            if (fc == "T")
                return FormatItem::ptr(new TabFormatItem(subfmt));

            return FormatItem::ptr();
        }

         // pattern 解析
        // std::string _pattern
        // std::cout << _pattern << std::endl;

        // 每个要素分为三部分：
        //  格式化字符 : %d %T %p...
        //  对应的输出⼦格式 ： {%H:%M:%S}
        //  对应数据的类型 ： 0-表⽰原始字符串，也就是⾮格式化字符，1-表⽰格式化数据类型
        // 默认格式 "%d{%H:%M:%S}%T%t%T[%p]%T[%c]%T%f:%l%T%m%n"
        // 
        bool parsePattern()
        {
            std::vector<std::tuple<std::string, std::string, int>> array;
            std::string format_key; // 存放 % 后的格式字符
            std::string format_val; // 存放格式化字符后边 {} 中的子格式字符串
            std::string string_row; // 存放原始的非格式化字符
            bool sub_format_error = false;
            int pos = 0;
            int n = _pattern.size();
            while(pos < n)
            {
                // 如果不是 % 添加到原始字符串.
                if(_pattern[pos] != '%')
                {
                    string_row += _pattern[pos++];
                    continue;
                }
                // Pos位置是%,判断是否是 %%,如果是的话只需要添加一个%即可.
                if(pos + 1 < n &&_pattern[pos + 1] == '%')
                {
                    string_row += _pattern[pos];
                    pos += 2;
                    continue;
                }
                // 如果原始字符串不为空, 添加到数组
                if(!string_row.empty())
                {
                    array.push_back(std::make_tuple(string_row, "", 0));
                    string_row.clear();
                }
                // 当前位置是%，并且下一个字符是 key.
                pos += 1;
                // 防止 % 是最后一个字符导致出错
                if(pos < n && isalpha(_pattern[pos]))
                {
                    format_key = _pattern[pos];
                }
                else
                {
                    std::cout << &_pattern[pos - 1] << "位置附近格式错误!" << std::endl;
                    return false;
                }
                // pos 指向格式化字符的下一个位置, 判断是否含有子格式 %d{%H%M%S}
                pos += 1;
                if(pos < n && _pattern[pos] == '{')
                {
                    sub_format_error = true;
                    pos += 1;
                    while(pos < n)
                    {
                        if(_pattern[pos] == '}')
                        {
                            sub_format_error = false;
                            pos += 1;
                            break;
                        }
                        format_val += _pattern[pos++];
                    }
                }
                // 找到一个需要的格式化 k,v
                array.push_back(std::make_tuple(format_key, format_val, 1));
                format_key.clear();
                format_val.clear();
            }

            if(sub_format_error)
            {
                std::cout << "{} 对应出错!" << std::endl;
                return false;
            }
            if (string_row.empty() == false)
                array.push_back(std::make_tuple(string_row, "", 0));
            if (format_key.empty() == false)
                array.push_back(std::make_tuple(format_key, format_val, 1));

             for (auto &it : array)
            {
                if (std::get<2>(it) == 0)
                {
                    FormatItem::ptr fi(new OtherFormatItem(std::get<0>(it)));
                    _items.push_back(fi);
                }
                else
                {
                    FormatItem::ptr fi = createItem(std::get<0>(it), std::get<1>(it));
                    if (fi.get() == nullptr)
                    {
                        std::cout << "没有对应的格式化字符: %" << std::get<0>(it) << std::endl;
                        return false;
                    }
                    _items.push_back(fi);
                }
            }
            return true;
        }

    private:
        std::string _pattern;
        std::vector<FormatItem::ptr> _items;

    };
}



#endif