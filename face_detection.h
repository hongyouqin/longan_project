#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include "arc_face/arcsoft_fsdk_face_detection.h"
#include <mutex>

class FaceDetection
{
public:
    FaceDetection();

    ~FaceDetection();

    bool Install();

    bool UnInstall();

    long StillImageFaceDetection(LPASVLOFFSCREEN data, LPAFD_FSDK_FACERES *result);

private:
    void *fd_engine_ = nullptr;

    unsigned char* ft_work_memory_ = nullptr;

    std::mutex mtx_;
};

#endif // FACE_DETECTION_H
