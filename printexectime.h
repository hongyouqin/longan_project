#ifndef PRINTEXECTIME_H
#define PRINTEXECTIME_H

#include <chrono>
#include <string>

//记录一段代码处理所花费的时间
//用法
//PrintExecTime time;

class PrintExecTime
{
public:
    explicit PrintExecTime(const std::string& tag="");

    ~PrintExecTime();
private:
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start_;

    std::string tag_;
};

#endif // PRINTEXECTIME_H
