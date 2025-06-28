#include <iostream>
using namespace std;

// 懒汉和饿汉模式

// 饿汉模式
class Singleton
{
private:
    static Singleton ins;
    Singleton()
    {
        std::cout<<"Singleton()"<< std::endl;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(Singleton&) = delete;

public:
    static Singleton& GetInstance()
    {
        return ins;
    }
    ~Singleton()
    {
        std::cout <<"~Singleton()"<< std::endl;
    }
};

Singleton Singleton::ins;

// 懒汉模式
// class Singleton
// {
// private:
//     Singleton()
//     {
//         std::cout << "Singleton() "<< std::endl;
//     }
//     Singleton(const Singleton&) = delete;
//     Singleton& operator=(Singleton&) = delete;

// public:
//     static Singleton& GetInstance()
//     {
//         static Singleton ins;
//         return ins;
//     }

//     ~Singleton()
//     {
//         std::cout << "~Singleton() "<< std::endl;
//     }
// };
int main()
{
}