#include <iostream>
#include <memory>

/*抽象电脑类*/
class Computer
{
public:
    using ptr = std::shared_ptr<Computer>;
    Computer() {}
    void setBoard(const std::string &board) { _board = board; }
    void setDisplay(const std::string &display) { _display = display; }
    virtual void setOs() = 0;
    std::string toString()
    {
        std::string computer = "Computer:{ \n";
        computer += "\tboard=" + _board + ",\n";
        computer += "\tdisplay=" + _display + ",\n";
        computer += "\tOs=" + _os + ",\n";
        computer += "}\n";
        return computer;
    }

protected:
    std::string _board; // 主板
    std::string _display; // 显示器
    std::string _os; // 操作系统
};

// 具体产品类
class MacBook : public Computer
{
public:
    using ptr = std::shared_ptr<MacBook>;
    MacBook(){}
    virtual void setOs()
    {
        _os = "Max Os X13";
    }
};

// 抽象建造者类
class Builder
{
public:
    using ptr = std::shared_ptr<Builder>;
    virtual void buildBoard(const std::string& board) = 0;
    virtual void buildDisplay(const std::string& display) = 0;
    virtual void buildOs() = 0;
    virtual Computer::ptr build() = 0;
};

// 具体产品建造者类
class MacBookBuilder : public Builder
{
public:
    using ptr = std::shared_ptr<MacBookBuilder>;
    MacBookBuilder()
    :_computer(new MacBook())
    {}
     void buildBoard(const std::string& board)
     {
        _computer->setBoard(board);
     }
     void buildDisplay(const std::string& display) 
     {
        _computer->setDisplay(display);
     }
     void buildOs() 
     {
        _computer->setOs();
     }
     Computer::ptr build()
     {
        return _computer;
     }

private:
    Computer::ptr _computer;
};

// 指挥者类，提供给调⽤者使⽤，通过指挥者来构造复杂产品
class Director
{
public:
    Director(Builder *builder)
        : _builder(builder) {}

    void construct(const std::string &board, const std::string &display)
    {
        _builder->buildBoard(board);
        _builder->buildDisplay(display);
        _builder->buildOs();
    }

private:
    Builder::ptr _builder;
};

int main()
{
    Builder *builder = new MacBookBuilder();
    std::unique_ptr<Director> pd(new Director(builder));

    pd->construct("因特尔主板", "VOC 显示器");

    Computer::ptr computer = builder->build();
    std::cout << computer->toString() << std::endl;
    return 0;
}