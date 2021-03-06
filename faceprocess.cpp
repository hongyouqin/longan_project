#include "faceprocess.h"
#include <QThread>
#include <thread>
#include "facesdata.h"
#include "face_detection.h"
#include "face_analysis_model.h"
#include "framedata.h"
#include "face_tracking.h"
#include "logger.h"
#include "printexectime.h"

FaceProcess::FaceProcess(QObject *parent) : QObject(parent)
{
   // ft_engine_.reset(new FaceTracking());
   // ft_engine_->Install();

    fd_engine_.reset(new FaceDetection());
    fd_engine_->Install();

    face_analysis_ = std::make_shared<FaceAnalysisModel>();
    face_analysis_->Init();
    QThread *thread = new QThread();
    face_analysis_->moveToThread(thread);
    connect(this, SIGNAL(faces_detected_signal(const FacesData&)), face_analysis_.get(), SLOT(RecvDetectedData(const FacesData&)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

FaceProcess::~FaceProcess()
{
    //disconnect(this, SIGNAL(faces_detected_signal(const FacesData&)), face_analysis_.get(), SLOT(RecvDetectedData(const FacesData&)));

    LogI("析构FaceProcess");

    face_analysis_.reset();

    if (ft_engine_) {
        ft_engine_->UnInstall();
    }

    if (fd_engine_) {
        fd_engine_->UnInstall();
    }
}

std::vector<QRect> FaceProcess::Frame(unsigned char *frame_data, int frame_width, int frame_height, unsigned int input_image_format, int &out_face_orient)
{
    std::vector<QRect> temp_rects;

    ASVLOFFSCREEN inputImg = { 0 };
    inputImg.u32PixelArrayFormat = input_image_format;
    inputImg.i32Width = frame_width;
    inputImg.i32Height = frame_height;
    inputImg.ppu8Plane[0] = (MUInt8*)frame_data;
    if (ASVL_PAF_I420 == inputImg.u32PixelArrayFormat) {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width / 2;
        inputImg.pi32Pitch[2] = inputImg.i32Width / 2;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + inputImg.pi32Pitch[0] * inputImg.i32Height;
        inputImg.ppu8Plane[2] = inputImg.ppu8Plane[1] + inputImg.pi32Pitch[1] * inputImg.i32Height / 2;
    } else if (ASVL_PAF_NV12 == inputImg.u32PixelArrayFormat) {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + (inputImg.pi32Pitch[0] * inputImg.i32Height);
    } else if (ASVL_PAF_NV21 == inputImg.u32PixelArrayFormat) {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + (inputImg.pi32Pitch[0] * inputImg.i32Height);
    } else if (ASVL_PAF_YUYV == inputImg.u32PixelArrayFormat) {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 2;
    } else if (ASVL_PAF_RGB32_B8G8R8A8 == inputImg.u32PixelArrayFormat) {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 4;
    } else if (ASVL_PAF_RGB24_B8G8R8 == inputImg.u32PixelArrayFormat) {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 3;
    }
    else {
        LogE("unsupported Image format: %u", inputImg.u32PixelArrayFormat);
        return std::move(temp_rects);;
    }
    //检测人脸处理
//    LPAFT_FSDK_FACERES face_reult = nullptr;
//    long ret = ft_engine_->FaceFeatureDetect(&inputImg, &face_reult);
//    if (ret != 0) {
//        LogE("fail to AFT_FSDK_FaceFeatureDetect()返回值: %d", ret);
//        return std::move(temp_rects);;
//    }

    LPAFD_FSDK_FACERES face_reult = nullptr;
    long ret = fd_engine_->StillImageFaceDetection(&inputImg, &face_reult);
    if (ret != 0) {
        LogE("fail to StillImageFaceDetection返回值: %d", ret);
        return std::move(temp_rects);;
    }

    QRect rect;
    for(int i = 0; i < face_reult->nFace; i++) {
      rect.setLeft(face_reult->rcFace[i].left);
      rect.setTop(face_reult->rcFace[i].top);
      rect.setRight(face_reult->rcFace[i].right);
      rect.setBottom(face_reult->rcFace[i].bottom);
      temp_rects.push_back(rect);
    }

    out_face_orient = *face_reult->lfaceOrient;

    return std::move(temp_rects);
}

void FaceProcess::set_serial_number(int serial)
{
    serial_number_ = serial;
}

int FaceProcess::serial_number() const
{
    return serial_number_;
}

void FaceProcess::SendDetectedSignal()
{

}

void FaceProcess::CameraFrame(FrameData &frame)
{

    std::thread process([&](FaceProcess *obj, const FrameData& frame) {
        cv::Mat mat = frame.GetMat();
        int w = mat.cols;
        int h = mat.rows;
        int out_face_orient;
        std::vector<QRect> result =  Frame(mat.data, w, h, ASVL_PAF_RGB24_B8G8R8, out_face_orient);
        if (!result.empty()) {
            //发送检测到的人脸信息
            FacesData data;
            data.SetFrameTime(frame.FrameTime());
            data.SetFacesRect(std::move(result));
            data.SetFrameWidth(w);
            data.SetFrameHeight(h);
            data.SetFormat(ASVL_PAF_RGB24_B8G8R8);
            data.SetMat(mat);
            data.SetFaceOrient(out_face_orient);
            emit faces_detected_signal(data);
        }
    }, this, frame);

    process.detach();

}
