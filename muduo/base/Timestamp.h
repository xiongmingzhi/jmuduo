#ifndef PROJECT_TIMESTAMP_H
#define PROJECT_TIMESTAMP_H
#include <iostream>
#include <boost/operators.hpp>
#include <muduo/base/copyable.h>

namespace muduo{

    class Timestamp: public muduo::copyable, public boost::less_than_comparable<Timestamp>{
    public:
        Timestamp():microSecondsSinceEpoch_(0){};
        explicit Timestamp(int64_t microSecondsSinceEpoch):microSecondsSinceEpoch_(microSecondsSinceEpoch){};
        void swap(Timestamp& that){
            std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
        };
        std::string toString() const;
        std::string toFormattedString() const;
        bool valid() const{return microSecondsSinceEpoch_ > 0;};
        int64_t microSecondsSinceEpoches(){return microSecondsSinceEpoch_;};
        time_t secondsSinceEpoches(){
            return static_cast<time_t>(microSecondsSinceEpoch_/kMicroSecondsPerSeconds);
        }
        //get time of now
        static Timestamp now();
        static Timestamp invalid(){return Timestamp();};
        static const int kMicroSecondsPerSeconds = 1000 * 1000;
    private:
        int64_t microSecondsSinceEpoch_;
    };

    inline bool operator< (Timestamp lhs, Timestamp rhs){
        return lhs.microSecondsSinceEpoches() < rhs.microSecondsSinceEpoches();
    }

    inline bool operator== (Timestamp lhs, Timestamp rhs){
        return lhs.microSecondsSinceEpoches() == rhs.microSecondsSinceEpoches();
    }

//substract
    inline double timeDifference(Timestamp high, Timestamp low){
        int64_t diff = high.microSecondsSinceEpoches() -low.microSecondsSinceEpoches();
        return static_cast<double>(diff)/Timestamp::kMicroSecondsPerSeconds;
    }

//add
    inline Timestamp addTime(Timestamp timestamp, double seconds){
        int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSeconds);
        return Timestamp(timestamp.microSecondsSinceEpoches() + delta); //构造函数
    }

}


#endif //PROJECT_TIMESTAMP_H