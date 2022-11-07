#include <iostream>
#include <vector>
#include <muduo/base/Timestamp.h>

using namespace muduo;

void passByValue(Timestamp x){
    printf("%s\n", x.toString().c_str());
}

void passByConstReference(Timestamp& x){
    printf("%s\n", x.toString().c_str());
}

void benchmark(){
    const int kNumber = 1000*1000;
    std::vector<Timestamp>stamps;
    stamps.reserve(kNumber);
    for(int i = 0 ; i < kNumber; ++i){
        stamps.push_back(Timestamp::now());
    }
    printf("%s\n", stamps.front().toString().c_str());
    printf("%s\n", stamps.back().toString().c_str());
    printf("%f\n", timeDifference(stamps.back(), stamps.front()));

    //查看每个点的间隔
    int increments[100] = {0};
    int64_t start = stamps.front().microSecondsSinceEpoches();
    for(int i = 1; i < kNumber; ++i){
        int64_t next = stamps[i].microSecondsSinceEpoches();//增加了强制转换
        int64_t inc = next -start;
        start = next;
        if(inc < 0){
            printf("reverse!\n");
        }
        else if ( inc < 100){
            ++ increments[inc];
        } else{
            printf("big gap %d\n", static_cast<int>(inc));
        }
    }
    for (int i = 0; i < 100; ++i)
    {
        printf("%2d: %d\n", i, increments[i]);
    }
}

int main(){
    Timestamp now(Timestamp::now());//构建一个对象
    printf("%s\n", now.toString().c_str());
    passByValue(now); //测试值传递
    passByConstReference(now);//测试引用传递
    benchmark();
    return 0;
}
