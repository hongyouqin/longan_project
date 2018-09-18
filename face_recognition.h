#ifndef FACE_RECOGNITION_H_
#define FACE_RECOGNITION_H_

#include "arc_face/arcsoft_fsdk_face_recognition.h"
#include <mutex>

class FaceRecognition {
public:
    FaceRecognition();
    
    ~FaceRecognition();

    bool Install();

    bool UnInstall();

    long ExtractFRFeature(LPASVLOFFSCREEN input_image,LPAFR_FSDK_FACEINPUT face_result,LPAFR_FSDK_FACEMODEL face_feature);

    long FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore);
private:
    MHandle fr_engine_ = nullptr;

    MByte *fr_work_memory_ = nullptr;

    std::mutex mtx_;
};


#endif
