#ifndef __M_BENCH_H__
#define __M_BENCH_H__
#include "logger.hpp"
#include "loggermanager.h"
#include "logMacro.h"
#include <chrono>

namespace yhlog
{

    void bench(const std::string &loger_name, size_t thread_num,
               size_t msglen, size_t msg_count)
    {
        Logger::ptr lp = getLogger(loger_name);
        if (lp.get() == nullptr)
            return;
        std::string msg(msglen, '1');
        size_t msg_count_per_thread = msg_count / thread_num;
        std::vector<double> cost_time(thread_num);
        std::vector<std::thread> threads;
        std::cout << "输⼊线程数量: " << thread_num << std::endl;
        std::cout << "输出⽇志数量: " << msg_count << std::endl;
        std::cout << "输出⽇志⼤⼩: " << msglen * msg_count / 1024 << "KB" << std::endl;
        for (int i = 0; i < thread_num; i++)
        {

            threads.emplace_back([&, i]()
                                 {
            auto start = std::chrono::high_resolution_clock::now();
            for(size_t j = 0; j < msg_count_per_thread; j++) {
            lp->fatal("%s", msg.c_str());
        }
            auto end = std::chrono::high_resolution_clock::now();
            auto cost=std::chrono::duration_cast<std::chrono::duration<double>>(end-start);
            cost_time[i] = cost.count();
            auto avg = msg_count_per_thread / cost_time[i];
            std::cout << "线程" << i << "耗时: " << cost.count() << "s";
            std::cout << " 平均：" << (size_t)avg << "/s\n"; });
        }
        for (auto &thr : threads)
        {
            thr.join();
        }
        double max_cost = 0;
        for (auto cost : cost_time)
            max_cost = max_cost < cost ? cost : max_cost;
        std::cout << "总消耗时间: " << max_cost << std::endl;
        std::cout << "平均每秒输出: " << (size_t)(msg_count / max_cost) << std::endl;
    }
}

#endif
