#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <cassert>

namespace yhlog
{
#define BUFFER_DEFAULT_SIZE (1 * 1024 * 1024)
#define BUFFER_INCREMENT_SIZE (1 * 1024 * 1024)
#define BUFFER_THRESHOLD_SIZE (10 * 1024 * 1024)
    class Buffer
    {
        Buffer()
            : _buffer(BUFFER_DEFAULT_SIZE),
              _read_idx(0),
              _write_idx(0)
        {
        }
        bool empty() { return _read_idx == _write_idx; }
        size_t ReadableSize() { return _write_idx - _read_idx; }
        size_t WritableSize() { return _buffer.size() - _write_idx; }
        void reset() { _read_idx = _write_idx = 0; }
        void swap(Buffer &other)
        {
            _buffer.swap(other._buffer);
            std::swap(_read_idx, other._read_idx);
            std::swap(_write_idx, other._write_idx);
        }
        void push(const char *data, size_t len)
        {
            assert(len <= WritableSize());
            EnsureEnoughSpace(len);
            std::copy(data, data + len, &_buffer[_write_idx]);
            _write_idx += len;
        }
        const char *begin() { return &_buffer[_read_idx]; }
        void pop()
        {
            _read_idx += ReadableSize();
            assert(_read_idx <= _write_idx);
        }

    protected:
        // 确保缓冲区有足够的空间
        void EnsureEnoughSpace(size_t len)
        {
            // 如果需要写入的长度小于等于可写大小，则不需要扩容
            if (len <= WritableSize())
                return;
            // 计算新的容量
            size_t new_capacity;
            // 如果缓冲区大小小于阈值，则新的容量为当前容量乘以2加上需要写入的长度
            if (_buffer.size() < BUFFER_THRESHOLD_SIZE)
                new_capacity = _buffer.size() * 2 + len;
            // 否则，新的容量为当前容量加上增量加上需要写入的长度
            else
                new_capacity = _buffer.size() + BUFFER_INCREMENT_SIZE + len;
            // 调整缓冲区大小
            _buffer.resize(new_capacity);
        }

    private:
        std::vector<char> _buffer;
        size_t _read_idx;
        size_t _write_idx;
    };

}
