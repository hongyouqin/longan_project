#include "face_detection.h"
#include "commondefine.h"
#include "logger.h"
#include "printexectime.h"

FaceDetection::FaceDetection()
{

}

FaceDetection::~FaceDetection()
{

}

bool FaceDetection::Install()
{
    ft_work_memory_ = new MByte[common::kFTMemBufSize];
    if(ft_work_memory_ == nullptr){
        return false;
    }

    int ret = AFD_FSDK_InitialFaceEngine((MPChar)APPID, (MPChar)FD_SDKKEY, ft_work_memory_,  common::kFTMemBufSize,
                                         &fd_engine_, AFD_FSDK_OPF_0_HIGHER_EXT, 24, common::kMaxFaceNum);
    if (ret != 0) {
        LogE("AFT_FSDK_InitialFaceEngine fail code: %d", ret);
        return false;
    }

    const AFD_FSDK_Version*pVersionInfo = AFD_FSDK_GetVersion(fd_engine_);
    LogI("FT version: %d %d %d %d %s", pVersionInfo->lCodebase, pVersionInfo->lMajor,
         pVersionInfo->lMinor, pVersionInfo->lBuild, pVersionInfo->Version);

    return true;
}

bool FaceDetection::UnInstall()
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        //卸载ft engine
        if (fd_engine_) {
            AFD_FSDK_UninitialFaceEngine(fd_engine_);
            fd_engine_ = nullptr;
        }
    }

    if (ft_work_memory_) {
        delete[] ft_work_memory_;
        ft_work_memory_ = nullptr;
    }

    return true;
}

long FaceDetection::StillImageFaceDetection(LPASVLOFFSCREEN data, LPAFD_FSDK_FACERES *result)
{
    std::lock_guard<std::mutex> lock(mtx_);
    return AFD_FSDK_StillImageFaceDetection(fd_engine_, data, result);
}
