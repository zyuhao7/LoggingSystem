#include <iostream>
#include <string>

// 租房基类.
class RentHouse
{
public:
    virtual void rentHouse() = 0; // 纯虚函数
};

// 房东类
class Host : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "将房子出租!" << std::endl;
    }
};

// 中介代理类
class Intemediary : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "发布招租启示 \n";
        std::cout << "带人看房\n";
        _host.rentHouse();
        std::cout << "租后维修\n";
    }
private:
    Host _host;
};

int main()
{
    Intemediary im;
    im.rentHouse();
    return 0;
}