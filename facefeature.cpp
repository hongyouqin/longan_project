#include "facefeature.h"
#include <memory>
#include "logger.h"

void auto_deleter(unsigned char* p) {
    if (p) {
        delete[] p;
    }
}

FaceFeature::FaceFeature()
{

}

FaceFeature::FaceFeature(const FaceFeature &lh)
{
   // LogI("FaceFeature copy construct");
    this->user_id_ = lh.user_id_;
    this->name = lh.name;
    this->face_photo = lh.face_photo;
    this->timestamp = lh.timestamp;
    this->feature_size_ = lh.feature_size_;
    this->frame_number_ = lh.frame_number_;
    this->mat_ = lh.mat_;
    this->expiry_time_ = lh.expiry_time_;
    this->is_employee_ = lh.is_employee_;
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
    this->frame_number_ = lh.frame_number_;
    this->mat_ = lh.mat_;
    this->expiry_time_ = lh.expiry_time_;
    this->is_employee_ = lh.is_employee_;
    std::unique_ptr<unsigned char[], deleter> temp(new unsigned char[lh.feature_size_](), auto_deleter);
    memcpy(temp.get(), lh.feature_.get(), lh.feature_size_);
    this->feature_ = std::move(temp);
    //LogI("FaceFeature operator=");

    return *this;
}
