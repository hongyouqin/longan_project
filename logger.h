#ifndef LOGGER_H_
#define LOGGER_H_
#include "easylogger/elog.h"


//线程安全的日志类,使用了easylogger日志开源库
// log_a("Hello EasyLogger!");
// log_e("Hello EasyLogger!");
// log_w("Hello EasyLogger!");
// log_i("Hello EasyLogger!");
// log_d("Hello EasyLogger!");
// log_v("Hello EasyLogger!");


#define LogA(...) log_a(__VA_ARGS__)

#define LogE(...) log_e(__VA_ARGS__)

#define LogW(...) log_w(__VA_ARGS__)

#define LogI(...) log_i(__VA_ARGS__)

#define LogD(...) log_d(__VA_ARGS__)

#define LogV(...) log_v(__VA_ARGS__)



class Logger {
public:
    static void InitLog();

    static void StartLog();
};


#endif 

