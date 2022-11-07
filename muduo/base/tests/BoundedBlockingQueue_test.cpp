//
// Created by 熊志耀 on 2022/10/29.
//
#include <unistd.h>
#include <muduo/base/BoundedBlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Thread.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/bind.hpp>
class Test{
public:
    Test(int numThread)
    : queue_(20),
      latch_(numThread),
      threads_(numThread)
    {
        for(int i=0; i<numThread;++i){
            char name[32];
            snprintf(name, sizeof(name), "work thread %d", i);
            threads_.push_back(new muduo::Thread(boost::bind(&Test::threadFunc, this), std::string(name)));
        }
        std::for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::start, _1));
    }

    void run(int times){
        printf("waiting for cout down latch\n");
        latch_.wait();
        printf("all threads started\n");
        for(int i=0; i<times;++i){
            char buf[32];
            snprintf(buf, sizeof(buf), "hello %d", i);
            queue_.put(buf);
            printf("tid=%d, put data = %s, size = %zd\n", muduo::CurrentThread::tid(), buf, queue_.size());
        }
    }

    void joinAll(){
        for(size_t i=0; i < threads_.size(); ++i){
            queue_.put("stop");
        }
        std::for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::join, _1));
    }

private:
    void threadFunc(){
        printf("tid=%d, &s started\n", muduo::CurrentThread::tid(), muduo::CurrentThread::name());
        latch_.countDown();
        bool running = true;
        while (running){
            std::string d(queue_.take());
            printf("tid=%d, get data=%s, size = %zd\n", muduo::CurrentThread::tid(), d.c_str(), queue_.size());
            running = (d !="stop");
        }
        printf("tid=%d, %s stoped\n", muduo::CurrentThread::tid(), muduo::CurrentThread::name());
    }
    muduo::BoundedBlockingQueue_test<std::string> queue_;
    muduo::CountDownLatch latch_;
    boost::ptr_vector<muduo::Thread> threads_;

};

int main(){
    printf("pid=%d, tid=%d", ::getpid(), muduo::CurrentThread::tid());
    Test t(5);
    t.run(100);
    t.joinAll();
    printf("number of created threads %d\n", muduo::Thread::numCreated());
}