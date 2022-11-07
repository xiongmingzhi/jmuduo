//
// Created by 熊志耀 on 2022/11/5.
//
#include <iostream>
#include <muduo/base/ThreadLocal.h>
#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <cstring>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Thread.h>
class Test:boost::noncopyable {
public:
    Test(){
        printf("tid=%d, construction %n\n", muduo::CurrentThread::tid());
    }
    ~Test(){
        printf("tid=%d, destruction %p %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
    }

    const std::string& name(){return name_;};
    void setName(const std::string& n){name_=n;};

private:
    std::string name_;
};

muduo::ThreadLocal<Test> testObj1;
muduo::ThreadLocal<Test> testObj2;

void print(){
    printf("tid=%d, obj1%p name=%s\n", muduo::CurrentThread::tid(), &testObj1.value(), testObj1.value().name().c_str());
    printf("tid=%d, obj2%p name=%s\n", muduo::CurrentThread::tid(), &testObj2.value(), testObj2.value().name().c_str());
}


void threadFunc(){
    testObj1.value().setName("changed 1");
    testObj2.value().setName("changed 42");
    print();
}

int main(){
    testObj1.value().setName("main one");
    print();
    muduo::Thread t1(threadFunc);
    t1.start();
    t1.join();
    testObj2.value().setName("main two");
    print();

    pthread_exit(0);
}
