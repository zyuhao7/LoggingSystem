#ifndef __M_LOOP_H__
#define __M_LOOP_H__
#include "util.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include "buffer.hpp"
namespace yhlog
{
    // 异步工作器. 输出线程
    class AsyncWorker
    {
    public:
        using Functor = std::function<void(Buffer &buffer)>;
        using ptr = std::shared_ptr<AsyncWorker>;
        AsyncWorker(const Functor &cb)
            : _running(true),
              _worker_callback(cb),
              _thread(std::bind(&AsyncWorker::worker, this))
        {
        }
        ~AsyncWorker() { stop(); }
        void stop()
        {
            _running = false;
            _pop_cv.notify_all();
            _thread.join();
        }
        void push(const std::string &msg)
        {
            if (_running == false)
                return;
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _push_cv.wait(lock, [&]()
                              { return _tasks_push.WritableSize() >= msg.size(); });
                _tasks_push.push(msg.c_str(), msg.size());
            }
            _pop_cv.notify_all();
        }

    private:
        void worker()
        {
            // 死循环,保证工作线程一直运行
            while (1)
            {
                {
                    std::unique_lock<std::mutex> lock(_mtx);
                    if (!_running && _tasks_push.empty())
                    {
                        // 如果工作线程停止且任务队列为空，退出循环
                        return;
                    }
                    // 等待任务队列有数据或工作线程停止
                    _pop_cv.wait(lock, [this]()
                                 { return !_running || !_tasks_push.empty(); });
                    // 交换缓冲区
                    _tasks_push.swap(_tasks_pop);
                }
                _push_cv.notify_all();
                _worker_callback(_tasks_pop);
                _tasks_pop.reset();
            }

            return;
        }

    private:
        Functor _worker_callback;

    private:
        std::mutex _mtx;
        std::atomic<bool> _running;
        std::condition_variable _push_cv;
        std::condition_variable _pop_cv;
        Buffer _tasks_push;
        Buffer _tasks_pop;
        std::thread _thread;
    };
}

#endif