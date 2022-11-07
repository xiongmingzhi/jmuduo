//
// Created by 熊志耀 on 2022/10/31.
//
#ifndef MUDUO_THREADPOOL_H
#define MUDUO_THREADPOOL_H
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <muduo/base/Condition.h>
#include <muduo/base/Thread.h>
namespace muduo
{
class ThreadPool: boost::noncopyable{
public:
    typedef boost::function<void()> Task;
    explicit ThreadPool(const std::string& name=std::string());
    ~ThreadPool();
    void start(int numThreads);
    void stop();
    void run(const Task& f);

private:
    void runInThrad();
    Task take();
    MutexLock mutex_;
    Condition cond_;
    std::string name_;
    boost::ptr_vector<muduo::Thread>thread_;
    std::deque<Task>queue_;
    bool running_;
};

}


#endif //MUDUO_THREADPOOL_H
