#include "face_recognition.h"
#include "commondefine.h"
#include "logger.h"

FaceRecognition::FaceRecognition() {

}

FaceRecognition::~FaceRecognition() {

}

bool FaceRecognition::Install() {
    
    fr_work_memory_ = new MByte[common::kFRMemBufSize];
    if(fr_work_memory_ == nullptr){
        return false;
    }

    int ret = AFR_FSDK_InitialEngine((MPChar)APPID, (MPChar)FR_SDKKEY, fr_work_memory_, common::kFRMemBufSize, &fr_engine_);
    if (ret != 0) {
        LogE("AFR_FSDK_InitialEngine fail code: %d", ret);
        return false;
    }
    const AFR_FSDK_Version*pVersionInfo = AFR_FSDK_GetVersion(fr_engine_);
    LogI("FR version: %d %d %d %d %s", pVersionInfo->lCodebase, pVersionInfo->lMajor,
                                 pVersionInfo->lMinor, pVersionInfo->lBuild, pVersionInfo->Version);

    return true;
}

bool FaceRecognition::UnInstall() {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (fr_engine_) {
            AFR_FSDK_UninitialEngine(fr_engine_);
            fr_engine_ = nullptr;
        }
    }

    if (fr_work_memory_) {
        delete[] fr_work_memory_;
        fr_work_memory_ = nullptr;
    }

    return true;
}

long FaceRecognition::ExtractFRFeature(LPASVLOFFSCREEN input_image, LPAFR_FSDK_FACEINPUT face_result, LPAFR_FSDK_FACEMODEL face_feature) {
    std::lock_guard<std::mutex> lock(mtx_);
    return AFR_FSDK_ExtractFRFeature(fr_engine_, input_image, face_result, face_feature);
}

long FaceRecognition::FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore) {
    std::lock_guard<std::mutex> lock(mtx_);
    return AFR_FSDK_FacePairMatching(fr_engine_,reffeature, probefeature, pfSimilScore);
}
