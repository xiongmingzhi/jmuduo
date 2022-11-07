// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <muduo/base/Atomic.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

    class Thread : boost::noncopyable
    {
    public:
        typedef boost::function<void ()> ThreadFunc; //函数容器，回调函数

        explicit Thread(const ThreadFunc&, const std::string& name = std::string()); //name=""
        ~Thread();

        void start();
        int join(); // return pthread_join()

        bool started() const { return started_; }
        // pthread_t pthreadId() const { return pthreadId_; }
        pid_t tid() const { return tid_; }
        const std::string& name() const { return name_; }

        static int numCreated() { return numCreated_.get(); }

    private:
        static void* startThread(void* thread);
        void runInThread();

        bool       started_; //是否开启线程
        pthread_t  pthreadId_; //线程id
        pid_t      tid_; //进程id
        ThreadFunc func_; //回调函数
        std::string     name_; //线程名称

        static AtomicInt32 numCreated_; //原子操作
    };

}
#endif
