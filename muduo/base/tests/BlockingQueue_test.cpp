//
// Created by 熊志耀 on 2022/10/29.
//
#include <stdio.h>
#include <unistd.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/BlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Thread.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/bind.hpp>

class Test{
public:
    Test(int numThread)
    : latch_(numThread),
    thread_(numThread)
    {
        for (int i=0; i<numThread; ++i){
            char name[32];
            snprintf(name, sizeof(name), "work thread %d", i);
            thread_.push_back(new muduo::Thread(boost::bind(&Test::threadFun, this), std::string(name)));
        }
        std::for_each(thread_.begin(), thread_.end(), boost::bind(&muduo::Thread::start, _1)); //将线程一次分发出去
    }

    void run(int times){
        printf("waiting for count down latch\n");
        latch_.wait();
        printf("all thread started\n");
        for(int i=0; i<times;++i){
            char buf[32];
            snprintf(buf, sizeof(buf), "hello %d", i);
            queue_.put(buf);
            printf("tid=%d, put data = %s, size = %zd\n", muduo::CurrentThread::tid(), buf, queue_.size());
        }
    }

    void joinAll(){
        for (size_t i=0; i<thread_.size();++i){
            queue_.put("stop");
        }
        std::for_each(thread_.begin(), thread_.end(), boost::bind(&muduo::Thread::join, _1));
    }

private:
    void threadFun(){
        printf("tid=%d, %s started\n", muduo::CurrentThread::tid(), muduo::CurrentThread::name());
        latch_.countDown();
        bool running = true;
        while (running){
            std::string d(queue_.take());
            printf("tid=%d, get data = %s, size = % zd\n", muduo::CurrentThread::tid(),d.c_str(), queue_.size());
            running = (d !="stop");
        }
        printf("tid=%d, %s stopped\n", muduo::CurrentThread::tid(), muduo::CurrentThread::name());
    }
    muduo::BlockingQuenue<std::string> queue_;//string?
    muduo::CountDownLatch latch_;
    boost::ptr_vector<muduo::Thread> thread_;

};
int main(){
    printf("pid=%d, tid=%d\n", ::getpid(), muduo::CurrentThread::tid());
    //新建5个线程
    Test t(5);
    t.run(100);
    t.joinAll();
    printf("number of created threads %d\n", muduo::Thread::numCreated());
}
