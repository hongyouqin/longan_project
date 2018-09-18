#include "printexectime.h"
#include "logger.h"

PrintExecTime::PrintExecTime()
{
    start_ = std::chrono::system_clock::now();
}

PrintExecTime::~PrintExecTime()
{
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
    double elapsed = double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    LogI("处理花费的时间: %f秒", elapsed);
}
