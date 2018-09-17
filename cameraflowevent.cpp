#include "cameraflowevent.h"
#include "framedata.h"

CameraFLowEvent::CameraFLowEvent() : QEvent(camera_flow_event)
{

}

void CameraFLowEvent::SetFrameData(const std::shared_ptr<FrameData> &frame)
{
    frame_ = frame;
}

std::shared_ptr<FrameData> CameraFLowEvent::GetFrame() const
{
    return frame_;
}

void CameraFLowEvent::SetShowMat(const cv::Mat &mat)
{
    show_mat_ = mat;
}

cv::Mat CameraFLowEvent::GetShowMat() const
{
    return show_mat_;
}

void CameraFLowEvent::SetFaceMat(const cv::Mat &mat)
{
    face_mat_ = mat;
}

cv::Mat CameraFLowEvent::GetFaceMat() const
{
    return face_mat_;
}
