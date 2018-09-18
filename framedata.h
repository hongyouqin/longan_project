#ifndef FRAMEDATA_H
#define FRAMEDATA_H

#include <QMetaType>
#include <cv.h>

class FrameData
{
public:
    FrameData();

    ~FrameData();

    FrameData(const FrameData& lh);

    FrameData& operator=(const FrameData& lh);

    bool SetFrameData(int width, int height, long type, char* data, int data_size);

    void SetMat(const cv::Mat& mat);

    char *Data() const;

    int Width() const;

    int Height() const;

    long Type() const;

    cv::Mat GetMat() const;

private:
    char *data_ = nullptr; //一帧数据

    int data_size_ = 0;

    int width_ = 0;

    int height_ = 0;

    long frame_type_ = 0; //帧的类型，比如yv12类型

    cv::Mat mat_;
};

Q_DECLARE_METATYPE(FrameData);

#endif // FRAMEDATA_H
