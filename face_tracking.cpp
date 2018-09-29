#include "face_tracking.h"
#include "commondefine.h"
#include "logger.h"


FaceTracking::FaceTracking() : ft_engine_(nullptr), ft_work_memory_(nullptr) {

}

FaceTracking::~FaceTracking() {

}

bool FaceTracking::Install() {
    ft_work_memory_ = new MByte[common::kFTMemBufSize];
     if(ft_work_memory_ == nullptr){
         return false;
     }

     int ret = AFT_FSDK_InitialFaceEngine((MPChar)APPID, (MPChar)FT_SDKKEY, ft_work_memory_, common::kFTMemBufSize,
                                          &ft_engine_, AFT_FSDK_OPF_0_HIGHER_EXT, 16, common::kMaxFaceNum);
     if (ret != 0) {
         LogE("AFT_FSDK_InitialFaceEngine fail code: %d", ret);
         return false;
     }

     const AFT_FSDK_Version*pVersionInfo = AFT_FSDK_GetVersion(ft_engine_);
     LogI("FT version: %d %d %d %d %s", pVersionInfo->lCodebase, pVersionInfo->lMajor,
                                 pVersionInfo->lMinor, pVersionInfo->lBuild, pVersionInfo->Version);

    return true;
}

bool FaceTracking::UnInstall() {
    //卸载ft engine
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (ft_engine_) {
            AFT_FSDK_UninitialFaceEngine(ft_engine_);
            ft_engine_ = nullptr;
        }
    }

    if (ft_work_memory_) {
        delete[] ft_work_memory_;
        ft_work_memory_ = nullptr;
    }
    return true;
}

long FaceTracking::FaceFeatureDetect(LPASVLOFFSCREEN data, LPAFT_FSDK_FACERES *result) {
    std::lock_guard<std::mutex> lock(mtx_);
    return AFT_FSDK_FaceFeatureDetect(ft_engine_, data, result);
}
