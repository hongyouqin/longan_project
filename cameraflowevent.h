#ifndef CAMERAFLOWEVENT_H
#define CAMERAFLOWEVENT_H

#include <QEvent>
#include <memory>
#include <chrono>
#include <cv.h>
#include <opencv/highgui.h>
#include <opencv.hpp>

const QEvent::Type camera_flow_event = (QEvent::Type)1000; //視頻劉事件

class FrameData;

class CameraFLowEvent : public QEvent
{
public:
    CameraFLowEvent();

    void SetFrameData(const std::shared_ptr<FrameData>& frame);

    std::shared_ptr<FrameData> GetFrame() const;

    void SetShowMat(const cv::Mat& mat);

    cv::Mat GetShowMat() const;

    void SetFaceMat(const cv::Mat& mat);

    cv::Mat GetFaceMat() const;

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> frame_time_; //帧时间

private:
    std::shared_ptr<FrameData> frame_ = nullptr;

    cv::Mat show_mat_; //用于窗口显示的mat

    cv::Mat face_mat_; //用于人脸识别的mat
};

#endif // CAMERAFLOWEVENT_H
