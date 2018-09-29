#include "framedata.h"
#include <string.h>

FrameData::FrameData()
{

}

FrameData::~FrameData()
{
    if (nullptr != data_) {
        delete[] data_;
        data_ = nullptr;
    }
}

FrameData::FrameData(const FrameData &lh)
{
    this->data_ = new char[lh.data_size_];
    memcpy(data_, lh.data_, sizeof(char) * lh.data_size_);

    this->width_ = lh.width_;
    this->height_ = lh.height_;
    this->frame_type_ = lh.frame_type_;
    this->data_size_ = lh.data_size_;
    this->mat_ = lh.mat_;
    this->serial_ = lh.serial_;
}

FrameData &FrameData::operator=(const FrameData &lh)
{
    if (this == &lh) {
        return *this;
    }
    this->data_ = new char[lh.data_size_];
    memcpy(data_, lh.data_, sizeof(char) * lh.data_size_);

    this->width_ = lh.width_;
    this->height_ = lh.height_;
    this->frame_type_ = lh.frame_type_;
    this->data_size_ = lh.data_size_;
    this->mat_ = lh.mat_;
    this->serial_ = lh.serial_;
    return *this;
}

bool FrameData::SetFrameData(int width, int height, long type, char *data, int data_size)
{
    if (data == nullptr || data_size == 0) {
        return false;
    }

    width_ = width;
    height_ = height;
    frame_type_ = type;
    data_size_ = data_size;

    data_ = new char[data_size_];
    memset(data_, 0, sizeof(data_));
    memcpy(data_, data, sizeof(char) * data_size_);

    return true;
}

void FrameData::SetMat(const cv::Mat &mat)
{
    mat_ = mat;
}

char *FrameData::Data() const
{
    return data_;
}

int FrameData::Width() const
{
    return width_;
}

int FrameData::Height() const
{
    return height_;
}

long FrameData::Type() const
{
    return frame_type_;
}

cv::Mat FrameData::GetMat() const
{
    return mat_;
}

int FrameData::GetSerial() const
{
    return serial_;
}

void FrameData::SetSerial(int serial)
{
    serial_ = serial;
}
