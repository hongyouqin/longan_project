#ifndef FACEFEATURE_H
#define FACEFEATURE_H

#include <memory>
#include <string>
#include <chrono>
#include <QMetaType>
#include <cv.h>


//人脸特征
typedef void (*deleter) (unsigned char*); //删除器

void auto_deleter(unsigned char* p);

struct FaceFeature {
    int user_id_;

    std::string name = "";

    std::string face_photo = "";

    unsigned int timestamp = 0;

    std::unique_ptr<unsigned char[], deleter> feature_ = {nullptr, auto_deleter};//特征

    int feature_size_ = 0;

    unsigned long frame_number_ = 0;

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> expiry_time_;//人脸特征有效时间

    bool is_employee_ = false; //是否注册人员

    cv::Mat mat_;

    FaceFeature();

    FaceFeature(const FaceFeature& lh);

    FaceFeature& operator=(const FaceFeature& lh);
};
Q_DECLARE_METATYPE(FaceFeature);

#endif // FACEFEATURE_H
