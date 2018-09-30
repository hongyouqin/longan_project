#include "printexectime.h"
#include "logger.h"

PrintExecTime::PrintExecTime(const std::string& tag/*=""*/) : tag_(tag)
{
    start_ = std::chrono::system_clock::now();

}

PrintExecTime::~PrintExecTime()
{
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
    double elapsed = double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    LogI("%s处理花费的时间: %f微秒", tag_.c_str(), elapsed);
}
