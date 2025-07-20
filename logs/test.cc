#include "util.hpp"
#include "level.hpp"
#include "formatter.hpp"
#include "sink.hpp"
#include "logger.hpp"
#include "loggermanager.h"
#include "logMacro.h"
#include "bench.h"
#include <iostream>
#include <thread>
#include <string>
using namespace yhlog;
using namespace yhlog::util;
void loggerTest(const std::string &logger_name)
{
    yhlog::Logger::ptr lp = yhlog::getLogger(logger_name);
    assert(lp.get());
    LOGF("------------example--------------------");
    lp->debug("%s", "logger->debug");
    lp->info("%s", "logger->info");
    lp->warn("%s", "logger->warn");
    lp->error("%s", "logger->error");
    lp->fatal("%s", "logger->fatal");
    LOG_DEBUG(lp, "%s", "LOG_DEBUG");
    LOG_INFO(lp, "%s", "LOG_INFO");
    LOG_WARN(lp, "%s", "LOG_WARN");
    LOG_ERROR(lp, "%s", "LOG_ERROR");
    LOG_FATAL(lp, "%s", "LOG_FATAL");
    LOGF("---------------------------------------");

    std::string log_msg = "hello world-";
    size_t fsize = 0;
    size_t count = 0;
    while (count < 10000)
    {
        std::string msg = log_msg + std::to_string(count++);
        lp->error("%s", msg.c_str());
    }
}

void sync_bench_thread_log(size_t thread_count, size_t msg_count, size_t msglen)
{
    static int num = 1;
    std::string logger_name = "sync_bench_logger" + std::to_string(num++);
    LOGI("************************************************");
    LOGI("同步⽇志测试: %d threads, %d messages", thread_count, msg_count);
    yhlog::GlobalLoggerBuilder::ptr lbp(new yhlog::GlobalLoggerBuilder);
    lbp->buildLoggerName(logger_name);
    lbp->buildFormatter("%m%n");
    lbp->buildSink<yhlog::FileSink>("./logs/sync.log");
    lbp->buildLoggerType(yhlog::Logger::Type::LOGGER_SYNC);
    lbp->build();
    yhlog::bench(logger_name, thread_count, msglen, msg_count);

    LOGI("************************************************");
}

void async_bench_thread_log(size_t thread_count, size_t msg_count, size_t msglen)
{
    static int num = 1;
    std::string logger_name = "async_bench_logger" + std::to_string(num++);
    LOGI("************************************************");
    LOGI("异步⽇志测试: %d threads, %d messages", thread_count, msg_count);
    yhlog::GlobalLoggerBuilder::ptr lbp(new yhlog::GlobalLoggerBuilder);
    lbp->buildLoggerName(logger_name);
    lbp->buildFormatter("%m");
    lbp->buildSink<yhlog::FileSink>("./logs/async.log");
    lbp->buildLoggerType(yhlog::Logger::Type::LOGGER_ASYNC);
    lbp->build();
    yhlog::bench(logger_name, thread_count, msglen, msg_count);
    LOGI("************************************************");
}

void bench_test()
{
    // 同步写⽇志
    sync_bench_thread_log(1, 1000000, 100);
    sync_bench_thread_log(5, 1000000, 100);
    /*异步⽇志输出，为了避免因为等待落地影响时间所以⽇志数量降低为⼩于缓冲区⼤⼩进⾏测试*/
    async_bench_thread_log(1, 100000, 100);
    async_bench_thread_log(5, 100000, 100);
}
/*
    当前⽇志器：root 未检测到⽇志格式,默认设置为:  %d{%H:%M:%S}%T%t%T[%p]%T[%c]%T%f:%l%T%m%n
    当前⽇志器：root 未检测到落地⽅向，默认为标准输出!
    DEBUG 同步⽇志器: root创建成功...
    [16:39:43][140032152483648][INFO][root][test.cc:45] ************************************************
    [16:39:43][140032152483648][INFO][root][test.cc:46] 同步⽇志测试: 1 threads, 1000000 messages
    DEBUG 同步⽇志器: sync_bench_logger1创建成功...
    输⼊线程数量: 1
    输出⽇志数量: 1000000
    输出⽇志⼤⼩: 97656KB
    线程0耗时: 2.14785s 平均：465581/s
    总消耗时间: 2.14785
    平均每秒输出: 465581
    [16:39:46][140032152483648][INFO][root][test.cc:55] ************************************************
    [16:39:46][140032152483648][INFO][root][test.cc:45] ************************************************
    [16:39:46][140032152483648][INFO][root][test.cc:46] 同步⽇志测试: 5 threads, 1000000 messages
    DEBUG 同步⽇志器: sync_bench_logger2创建成功...
    输⼊线程数量: 5
    输出⽇志数量: 1000000
    输出⽇志⼤⼩: 97656KB
    线程3耗时: 2.19478s 平均：91125/s
    线程0耗时: 2.24779s 平均：88976/s
    线程1耗时: 2.24875s 平均：88938/s
    线程2耗时: 2.25254s 平均：88788/s
    线程4耗时: 2.25247s 平均：88791/s
    总消耗时间: 2.25254
    平均每秒输出: 443943
    [16:39:48][140032152483648][INFO][root][test.cc:55] ************************************************
    [16:39:48][140032152483648][INFO][root][test.cc:62] ************************************************
    [16:39:48][140032152483648][INFO][root][test.cc:63] 异步⽇志测试: 1 threads, 100000 messages
    DEBUG异步⽇志器: async_bench_logger1创建成功...
    输⼊线程数量: 1
    输出⽇志数量: 100000
    输出⽇志⼤⼩: 9765KB
    线程0耗时: 0.192663s 平均：519041/s
    总消耗时间: 0.192663
    平均每秒输出: 519041
    [16:39:48][140032152483648][INFO][root][test.cc:71] ************************************************
    [16:39:48][140032152483648][INFO][root][test.cc:62] ************************************************
    [16:39:48][140032152483648][INFO][root][test.cc:63] 异步⽇志测试: 5 threads, 100000 messages
    DEBUG异步⽇志器: async_bench_logger2创建成功...
    输⼊线程数量: 5
    输出⽇志数量: 100000
    输出⽇志⼤⼩: 9765KB
    线程2耗时: 0.0830193s 平均：240907/s
    线程1耗时: 0.0922329s 平均：216842/s
    线程0耗时: 0.0950633s 平均：210386/s
    线程3耗时: 0.0982815s 平均：203497/s
    线程4耗时: 0.0998292s 平均：200342/s
    总消耗时间: 0.0998292
    平均每秒输出: 1001710
    [16:39:48][140032152483648][INFO][root][test.cc:71] ************************************************
*/

int main()
{
    bench_test();

    // 实例化全局⽇志器建造者
    // yhlog::GlobalLoggerBuilder::ptr lbp(new yhlog::GlobalLoggerBuilder);
    // lbp->buildLoggerName("stdout_and_file_logger");                    // 设置⽇志器名称
    // lbp->buildFormatter("[%d][%c][%f:%l][%p] %m%n");                   // 设置⽇志输出格式
    // lbp->buildLoggerLevel(yhlog::LogLevel::Value::DEBUG);              // 设置⽇志限制输出等级
    // lbp->buildSink<yhlog::StdoutSink>();                               // 创建⼀个标准输出的落地⽅向
    // lbp->buildSink<yhlog::FileSink>("./logs/sync.log");                // 创建⼀个⽂件落地⽅向
    // lbp->buildSink<yhlog::RollSink>("./logs/roll-", 10 * 1024 * 1024); // 创建滚动⽇志落地⽅向
    // lbp->buildLoggerType(yhlog::Logger::Type::LOGGER_SYNC);            // 设置⽇志器类型为同步⽇志
    // lbp->build();                                                      // 建造⽇志器
    // loggerTest("stdout_and_file_logger");

    // std::string logger_name = "myh";
    // yhlog::LogMsg msg(logger_name, "main.cc", 27, "Formatted log message test!", yhlog::LogLevel::Value::INFO);

    // {
    //     yhlog::Formatter fmt;
    //     std::string str = fmt.format(msg);
    //     yhlog::LogSink::ptr stdout_sink = yhlog::SinkFactory::create<yhlog::StdoutSink>();
    //     stdout_sink->log(str.c_str(), str.size()); // Write to stdout
    // }

    // {
    //     yhlog::Formatter fmt("[%d{%Y-%m-%d %H:%M:%S}][%t][%p][%c] %m%n");
    //     std::string str = fmt.format(msg);
    //     yhlog::LogSink::ptr file_sink = yhlog::SinkFactory::create<yhlog::FileSink>("test_log.log");
    //     file_sink->log(str.c_str(), str.size());
    // }

    // {
    //     yhlog::Formatter fmt("[%d{%H:%M:%S}][%p] %m%n");
    //     std::string str = fmt.format(msg);
    //     yhlog::LogSink::ptr roll_sink = yhlog::SinkFactory::create<yhlog::RollSink>("./logfile/roll_log_", 1024 * 1024);
    //     for (int i = 0; i < 10086; ++i)
    //     {
    //         std::string test_str = "RollSink File #" + std::to_string(i) + ": " + str;
    //         roll_sink->log(test_str.c_str(), test_str.size());
    //     }
    // }

    // std::string logger_name = "myh";
    // yhlog::LogMsg msg(logger_name, "main.cc", 27, "Formatted log message test!", yhlog::LogLevel::Value::INFO);
    // yhlog::Formatter fmt("[%d{%Y-%m-%d %H:%M:%S}][%t][%p][%c] %m%n");
    // std::string str = fmt.format(msg);
    // std::cout << str << std::endl;

    // std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::DEBUG) << std::endl;
    // std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::INFO) << std::endl;
    // std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::WARN) << std::endl;
    // std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::ERROR) << std::endl;
    // std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::FATAL) << std::endl;
    // std::cout<< yhlog::LogLevel::toString(yhlog::LogLevel::Value::OFF) << std::endl;

    // std::string fullPath = "./test_dir1/test_dir2/test_file.txt";
    // std::string dirPath = file::path(fullPath);  // 提取目录部分 ./test_dir1/test_dir2/
    // std::cout << "Creating directory: " << dirPath << std::endl;

    // file::create_directory(dirPath);

    // if (file::exists(dirPath))
    // {
    //     std::cout << "Directory created successfully: " << dirPath << std::endl;
    // }
    // else
    // {
    //     std::cout << "Failed to create directory: " << dirPath << std::endl;
    // }

    return 0;
}
