#ifndef FACESDATA_H
#define FACESDATA_H

#include <vector>
#include <chrono>
#include <QRect>
#include <QMetaType>
#include <cv.h>

//检测到的人脸数据
class FacesData
{
public:
    FacesData();

    void SetFacesRect(const std::vector<QRect>& rect_faces);

    std::vector<QRect> GetFacesRect() const;

    void SetFrameWidth(int width);

    int FrameWidth() const;

    void SetFrameHeight(int height);

    int FrameHeight() const;

    void SetMat(const cv::Mat& mat);

    cv::Mat GetMat() const;

    void SetFormat(unsigned int format);

    unsigned int GetFormat() const;

    void SetFaceOrient(int orient);

    int GetFaceOrient() const;

    void SetIndex(int index);

    int GetIndex() const;

    void SetFrameTime(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time);

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> FrameTime() const;
private:
    std::vector<QRect> rect_faces_; //检测到的人脸数据

    int index_ = 0; //人脸vector索引

    int frame_width_ = 0;

    int frame_height_ = 0;

    unsigned int format_ = 0;

    int face_orient_ = 0;

    cv::Mat mat_;

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> frame_time_;
};
Q_DECLARE_METATYPE(FacesData);

#endif // FACESDATA_H
