#pragma once

#include <stddef.h>
#include <string.h>

namespace Ultramat {

template<class T, size_t N>
class RingBuffer
{
public:

    RingBuffer<T, N>()
        : size_{N}, elemSize_{sizeof(T)},
            head_{0}, tail_{0}
    {
        clear();
    }

    bool isFull() const
    {
        return ((head_ + 1) % size_) == tail_;
    }

    bool isEmpty() const
    {
        return head_ == tail_;
    }

    bool append(const T item)
    {
        if(isFull())
            return false;

        // copy item, ignore result
        memcpy(&buffer_[head_], &item, elemSize_);
        head_ = (head_ + 1) % size_;
        return true;
    }

    bool get(T &item)
    {
        if(isEmpty())
        {
            return false;
        }

        item = buffer_[tail_];
        tail_ = (tail_ + 1) % size_;
        return true;
    }

    void clear()
    {
        memset(buffer_, 0, elemSize_ * size_);
        head_ = tail_ = 0;
    }

    size_t getSize() const
    {
        return size_;
    }

private:
    // buffer size
    const size_t size_;
    // buffer element size
    const size_t elemSize_;
    // head and tail (write and read) positions
    size_t head_, tail_;

    T buffer_[N];
};

} // namespace Ultramat
