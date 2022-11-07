//
// Created by 熊志耀 on 2022/10/31.
//

#include "ThreadPool.h"
#include <assert.h>
#include <boost/bind.hpp>
#include <muduo/base/Exception.h>
using namespace muduo;
ThreadPool::ThreadPool(const std::string &name)
    :mutex_(),
     cond_(mutex_),
     name_(name),
     running_(false)
{
}
ThreadPool::~ThreadPool() {
    if(running_){
        stop();
    }
}

void ThreadPool::start(int numThreads) {
    assert(thread_.empty());
    running_ = true;
    thread_.reserve(numThreads);
    for (int i=0; i<numThreads;++i){
        char id[32];
        snprintf(id, sizeof(id), "%d", i);
        thread_.push_back(new muduo::Thread(boost::bind(&ThreadPool::runInThrad, this), name_+id));
        thread_[i].start();//创建线程
    }
}

void ThreadPool::stop() {
    {
        MutexLockGuard lock(mutex_);
        running_ = false;
        cond_.notifyAll();
    }
    std::for_each(thread_.begin(), thread_.end(), boost::bind(&muduo::Thread::join, _1));
}


void ThreadPool::run(const Task &task) {
    if (thread_.empty()){
        task();
    } else{
        MutexLockGuard lock(mutex_);
        queue_.push_back(task);
        cond_.notify();
    }
}

ThreadPool::Task ThreadPool::take(){
    MutexLockGuard lock(mutex_);
    while (queue_.empty() && running_){
        cond_.wait();
    }
    Task task;
    if(!queue_.empty()){
        task = queue_.front();
        queue_.pop_front();
    }
    return task;
}




void ThreadPool::runInThrad()
{
    try
    {
        while (running_)
        {
            Task task(take());
            if(task)
            {
                task();
            }
        }
    }
    catch (const Exception& ex) {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reasion: %s\n", ex.what());
        fprintf(stderr, "stack trace:%\n", ex.stackTrace());
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...) {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
        throw ;//rethrow
    }
}