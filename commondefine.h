#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H

//通用的一些定义

namespace common {
    //红软 appid和key
    #define   APPID          "FAX6N6Xuhom2yJwtxJH57PBhuzY48jQQVPhYrXdfffca"

    //人脸跟踪key
    #define   FT_SDKKEY      "CpD1fuKXcZwgGTCYSRWJzV7jXvrtR7h2S8iz5ewUXKQS"

    //人脸识别key
    #define   FR_SDKKEY      "CpD1fuKXcZwgGTCYSRWJzV8EBXubPpYeTCU9Yt6MVyKL"

    //静态人脸跟蹤
    #define   FD_SDKKEY      "CpD1fuKXcZwgGTCYSRWJzV7rhL85NJymYPKBqhxaHCbi"

    //人脸跟踪内存buf大小
    extern const int kFTMemBufSize;

    //人脸识别内存buf大小
    extern const int kFRMemBufSize;

    //引擎最多能检测出的人脸数
    extern const int kMaxFaceNum;
}


#endif // COMMONDEFINE_H
