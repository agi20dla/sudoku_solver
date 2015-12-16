//
// Shamelessly stolen from: https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
// Created by jason on 5/7/15.
//

#ifndef SUDOKU_SOLVER_CONCURRENTQUEUE_H
#define SUDOKU_SOLVER_CONCURRENTQUEUE_H

#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include "IoMessage.h"

//template<class T>
class ConcurrentQueue
{
private:
    std::queue<std::shared_ptr<IoMessage> > queue_;
    mutable boost::mutex the_mutex;
    boost::condition_variable the_condition_variable;

    ulong num_messages_rcvd_;

public:

    ConcurrentQueue()
    : num_messages_rcvd_(0)
    {}

    ConcurrentQueue(const ConcurrentQueue &otherQueue)
            : queue_(otherQueue.queue_)
            , num_messages_rcvd_(0)
    {}

    ~ConcurrentQueue()
    {
        while (!queue_.empty()) {
            auto iom = queue_.front();
            queue_.pop();
            iom = nullptr;
        }
    }

    void push(std::shared_ptr<IoMessage> data) {
        boost::mutex::scoped_lock lock(the_mutex);
        queue_.push(data);
        ++num_messages_rcvd_;
        lock.unlock();
        the_condition_variable.notify_one();
    }

    bool empty() const
    {
        boost::mutex::scoped_lock lock(the_mutex);
        return queue_.empty();
    }


    std::shared_ptr<IoMessage> try_pop()
    {
        boost::mutex::scoped_lock lock(the_mutex);

        if (queue_.empty())
        {
            return nullptr;
        } else {

            std::shared_ptr<IoMessage> popped = queue_.front();
            queue_.pop();
            return popped;
        }
    }

    std::shared_ptr<IoMessage> wait_and_pop()
    {
        boost::mutex::scoped_lock lock(the_mutex);
        while(queue_.empty())
        {
            the_condition_variable.wait(lock);
        }

        std::shared_ptr<IoMessage> popped_value = queue_.front();
        queue_.pop();
        return popped_value;
    }


    ulong size()
    {
        boost::mutex::scoped_lock lock(the_mutex);
        ulong size = queue_.size();
        lock.unlock();

        return size;
    }

    ulong get_num_messages_rcvd()
    {
        boost::mutex::scoped_lock lock(the_mutex);
        ulong rcvd = num_messages_rcvd_;
        lock.unlock();

        return rcvd;
    }

    void clear() {
        std::queue<std::shared_ptr<IoMessage> > emptyQueue;
        std::swap(queue_, emptyQueue);
    }

};


#endif //SUDOKU_SOLVER_CONCURRENTQUEUE_H
