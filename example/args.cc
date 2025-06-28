#include <iostream>
#include <cstdarg>
#include <memory>
#include <functional>

// C 语言风格的不定参
// void printNum(int n, ...)
// {
//     va_list vl;
//     va_start(vl, n); // 让 vl 指向 n 参数之后的第一个可变参数
//     for(int i = 0; i < n; ++i)
//     {
//         int num = va_arg(vl, int);
//         std::cout<<num<<" ";
//     }
//     std::cout<<std::endl;
//     va_end(vl); // 清空可变参数列表.
// }

// void myprintf(const char* fmt, ...)
// {
//     // int vasprintf(char** strp, const char* fmt, va_list ap);

//     char* res;
//     va_list vl;
//     va_start(vl, fmt);
//     int len = vasprintf(&res, fmt, vl);
//     std::cout<<res<<std::endl;
//     free(res);
// }

// int main()
// {
//     printNum(3, 1,3,4);
//     printNum(5, 11,22,33,44,55);
//     myprintf("%s的年龄为: %d\n", "章三年", 27);
//     return 0;
// }

// C++ 风格的不定参函数
void xprintf()
{
    std::cout<< std::endl;
}

template<typename T, typename... Args>
void xprintf(const T& value, Args&&... args)
{
    std::cout << value << " ";
    if((sizeof...(args)) > 0)
    {
        xprintf(std::forward<Args>(args)...);
    }
    else
    {
        xprintf();
    }
}
int main()
{
    xprintf("沫羽皓");
    xprintf("沫羽皓", "Niubility");
    xprintf("沫羽皓", "Niubility", 666);


}
