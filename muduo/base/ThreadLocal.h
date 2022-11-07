#ifndef MUDUO_THREADLOCAL_H
#define MUDUO_THREADLOCAL_H
#include <boost/noncopyable.hpp>
#include <pthread.h>
namespace muduo
{
template<typename T>
class ThreadLocal: boost::noncopyable
{
public:
    ThreadLocal(){
        pthread_key_create(&pkey_, ThreadLocal::destructor);
    }
    ~ThreadLocal(){
    }

    T& value(){
        T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
        if(!perThreadValue){
            T* newobj = new T();
            pthread_setspecific(pkey_, newobj);
            perThreadValue = newobj;
        }
        return *perThreadValue;
    }

private:
    static void destructor(void *x){
        T*obj = static_cast<T*>(x);
        typedef char T_must_be_complete_typei[sizeof(T) == 0 ?-1:1];
        delete obj;
    }
    pthread_key_t pkey_;
};
}

#endif //MUDUO_THREADLOCAL_H
