#ifndef FACE_TRACKING_H_
#define FACE_TRACKING_H_
#include "arc_face/arcsoft_fsdk_face_tracking.h"
#include <mutex>

class FaceTracking {
public:
    FaceTracking();

    ~FaceTracking();

    bool Install();

    bool UnInstall();

    long FaceFeatureDetect(LPASVLOFFSCREEN data, LPAFT_FSDK_FACERES *result);

private:
    void *ft_engine_;

    unsigned char* ft_work_memory_;

    std::mutex mtx_;
};


#endif
