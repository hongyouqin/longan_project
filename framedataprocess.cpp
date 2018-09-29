#include "framedataprocess.h"
#include "cameraflowevent.h"
#include "convertimageformat.h"
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
        int w = data.Width();
        int h = data.Height();
        static IplImage* pImgYCrCb = cvCreateImage(cvSize(w, h), 8, 3);//得到图像的Y分量
        ConvertImageFormat::YU12toYUV(pImgYCrCb->imageData, data.Data(), w, h, pImgYCrCb->widthStep);//得到所有RGB图像
        cv::Mat camera_frame = cv::cvarrToMat(pImgYCrCb, false);
        cv::Mat show_frame;
        cv::Mat face_frame;
        //CV_YUV2BGR
        cv ::cvtColor(camera_frame, show_frame, CV_YUV2BGR);
        //to CV_YUV2RGB
        cv ::cvtColor(camera_frame, face_frame, CV_YUV2RGB);

        CameraFLowEvent *event = new CameraFLowEvent();
    //    auto frame_data(std::make_shared<FrameData>());
    //    frame_data->SetFrameData(pFrameInfo->nWidth, pFrameInfo->nHeight, frame_type, pBuf, nSize);
    //    event->SetFrameData(frame_data);
        event->SetShowMat(show_frame);
        event->SetFaceMat(face_frame);
        QApplication::postEvent(GetMainWindow(), event);
    }, data);

    process.detach();
}
