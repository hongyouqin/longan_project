#include "facesdata.h"

FacesData::FacesData()
{

}

void FacesData::SetFacesRect(const std::vector<QRect> &rect_faces)
{
    rect_faces_ = rect_faces;
}

std::vector<QRect> FacesData::GetFacesRect() const
{
    return rect_faces_;
}

void FacesData::SetFrameWidth(int width)
{
    frame_width_ = width;
}

int FacesData::FrameWidth() const
{
    return frame_width_;
}

void FacesData::SetFrameHeight(int height)
{
    frame_height_ = height;
}

int FacesData::FrameHeight() const
{
    return frame_height_;
}

void FacesData::SetMat(const cv::Mat &mat)
{
    mat_ = mat;
}

cv::Mat FacesData::GetMat() const
{
    return mat_;
}

void FacesData::SetFormat(unsigned int format)
{
    format_ = format;
}

unsigned int FacesData::GetFormat() const
{
    return format_;
}

void FacesData::SetFaceOrient(int orient)
{
    face_orient_ = orient;
}

int FacesData::GetFaceOrient() const
{
    return face_orient_;
}

void FacesData::SetIndex(int index)
{
    index_ = index;
}

int FacesData::GetIndex() const
{
    return index_;
}

void FacesData::SetFrameTime(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time)
{
    frame_time_ = time;
}

std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> FacesData::FrameTime() const
{
    return frame_time_;
}
