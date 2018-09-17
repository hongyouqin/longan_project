#ifndef PRINTEXECTIME_H
#define PRINTEXECTIME_H

#include <chrono>

//记录一段代码处理所花费的时间
//用法
//PrintExecTime time;

class PrintExecTime
{
public:
    PrintExecTime();

    ~PrintExecTime();
private:
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start_;
};

#endif // PRINTEXECTIME_H
