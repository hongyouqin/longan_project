#include "framedataprocess.h"
#include "cameraflowevent.h"
#include "convertimageformat.h"
#include "configs.h"
#include "framedata.h"
#include <QApplication>
#include <thread>
#include "mainwindow.h"

FrameDataProcess::FrameDataProcess(QObject *parent) : QObject(parent)
{

}

void FrameDataProcess::RecvCameraData(FrameData &data)
{
    std::thread process([](const FrameData& data){

        auto time = std::chrono::system_clock::now();

        int w = data.Width();
        int h = data.Height();
        IplImage* pImgYCrCb = cvCreateImage(cvSize(w, h), 8, 3);//得到图像的Y分量
        ConvertImageFormat::YU12toYUV(pImgYCrCb->imageData, data.Data(), w, h, pImgYCrCb->widthStep);//得到所有RGB图像

        cv::Mat temp_frame = cv::cvarrToMat(pImgYCrCb, false);
        cv::Mat camera_frame = temp_frame.clone();
        cvReleaseImage(&pImgYCrCb);
        cv::Mat show_frame;
        cv::Mat face_frame;


        CameraFLowEvent *event = new CameraFLowEvent();
        auto system = Configs::GetSystemConfig();
        if (system->show_camera == 1) {
            //CV_YUV2BGR
            cv ::cvtColor(camera_frame, show_frame, CV_YUV2BGR);
            event->SetShowMat(show_frame);
        }

        //to CV_YUV2RGB
        cv ::cvtColor(camera_frame, face_frame, CV_YUV2RGB);
        event->SetFaceMat(face_frame);
        event->frame_time_ = time;
        QApplication::postEvent(GetMainWindow(), event);
    }, data);

    process.detach();
}
