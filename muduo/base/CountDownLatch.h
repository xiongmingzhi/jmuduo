// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>

namespace muduo
{

class CountDownLatch : boost::noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait(); //等待线程

  void countDown(); //计数-1

  int getCount() const;//获取当前cout

 private:
  mutable MutexLock mutex_; //互斥锁mutex
  Condition condition_; //条件变量
  int count_; //执行线程计数
};

}
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
