#ifndef FACEFEATURE_H
#define FACEFEATURE_H

#include <memory>
#include <string>
#include <QMetaType>


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

    FaceFeature();

    FaceFeature(const FaceFeature& lh);

    FaceFeature& operator=(const FaceFeature& lh);
};
Q_DECLARE_METATYPE(FaceFeature);

#endif // FACEFEATURE_H
