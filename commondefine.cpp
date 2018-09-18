#include "commondefine.h"

namespace common {
    //人脸跟踪内存buf大小
    extern const int kFTMemBufSize = 40*1024*1024;

    //人脸识别内存buf大小
    const int kFRMemBufSize = 40*1024*1024;

    //引擎最多能检测出的人脸数
    extern const int kMaxFaceNum = 20;
}

