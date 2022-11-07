//
// Created by 熊志耀 on 2022/11/2.
//
#include <iostream>
#include <muduo/base/Singleton.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Thread.h>
class Test: boost::noncopyable {
public:
    Test(){
        printf("tid=%d, constucting %p\n", muduo::CurrentThread::tid(), this);
    }
    ~Test(){
        printf("tid=%d, destructing %p %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
    }
    const std::string& name()const{return name_;};
    void setName(const std::string& n){name_=n;};
private:
    std::string name_;

};

void threadFunc(){
    printf("tid=%d, %p name=%s\n",
           muduo::CurrentThread::tid(),
            &muduo::Singleton<Test>::instance);
    muduo::Singleton<Test>::instance().setName("only one, changed")
}
int main(){
    muduo::Singleton<Test>::instance().setName("only one");
    muduo::Thread t1(threadFunc);
    t1.start();
    t1.join();
    printf("tid=%d, %p name=%s\n",
           muduo::CurrentThread::tid(),
           &muduo::Singleton<Test>::instance(),
           muduo::Singleton<Test>::instance().name().c_str());
}