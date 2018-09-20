#include "facefeature.h"
#include <memory>
#include <QDebug>

void auto_deleter(unsigned char* p) {
    if (p) {
        delete[] p;
        qDebug() << "删除人脸特征数据";
    }
}

FaceFeature::FaceFeature()
{

}

FaceFeature::FaceFeature(const FaceFeature &lh)
{
    qDebug() << "FaceFeature copy construct";
    this->user_id_ = lh.user_id_;
    this->name = lh.name;
    this->face_photo = lh.face_photo;
    this->timestamp = lh.timestamp;
    this->feature_size_ = lh.feature_size_;
    std::unique_ptr<unsigned char[], deleter> temp(new unsigned char[lh.feature_size_](), auto_deleter);
    memcpy(temp.get(), lh.feature_.get(), lh.feature_size_);
    this->feature_ = std::move(temp);
}

FaceFeature &FaceFeature::operator=(const FaceFeature &lh)
{
    if (this == &lh) {
        return *this;
    }

    this->user_id_ = lh.user_id_;
    this->name = lh.name;
    this->face_photo = lh.face_photo;
    this->timestamp = lh.timestamp;
    this->feature_size_ = lh.feature_size_;
    std::unique_ptr<unsigned char[], deleter> temp(new unsigned char[lh.feature_size_](), auto_deleter);
    memcpy(temp.get(), lh.feature_.get(), lh.feature_size_);
    this->feature_ = std::move(temp);
    qDebug() << "FaceFeature operator=";

    return *this;
}
