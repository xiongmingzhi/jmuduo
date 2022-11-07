//
// Created by 熊志耀 on 2022/10/27.
//

#ifndef MUDUO_BLOCKINGQUEUE_H
#define MUDUO_BLOCKINGQUEUE_H
#include <boost/noncopyable.hpp>
#include <muduo/base/Mutex.h>
#include <muduo/base/Condition.h>
#include <deque>
#include <assert.h>
namespace muduo
{
template<typename T>
class BlockingQuenue:boost::noncopyable
{
public:
    BlockingQuenue()
    :mutex_(), //初始化互斥量
     notEmpty_(mutex_), //初始化信号
     queue_()
    {
    }
    ~BlockingQuenue(){}

    //生产产品，产品是string
    void put(const T& x){
        MutexLockGuard lock(mutex_);//先加上锁对队列进行保护，构造函数中调用lock,析构函数会自动调用unlock
        queue_.push_back(x);//产品放进队列
        notEmpty_.notify();//每添加一个元素，就通知所有的线程（当前缓冲区不为空，可以来取任务了），队列不为空，通知消费者可以进行消费；通知等待的线程，实现线程同步
    }

    //消费产品
    T take(){
        MutexLockGuard lock(mutex_);//加锁保护队列（线程安全）
        while (queue_.empty()){ //如果队列为空（仓库已空）
            notEmpty_.wait();
        }
        assert(!queue_.empty());//确保队列非空
        T front(queue_.front());//取出队首元素
        queue_.pop_front();
        return front;
    }

    /*可能有多个线程访问所以需要保护*/
    size_t size() const {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }
private:
   mutable MutexLock mutex_; //互斥锁（量）；mutable表示可变的，修饰后可以改变const的特性
   Condition notEmpty_;
   std::deque<T> queue_;

};
}

#endif //MUDUO_BLOCKINGQUEUE_H
