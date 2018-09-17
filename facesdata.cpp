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
