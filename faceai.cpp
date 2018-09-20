#include "faceai.h"
#include "face_recognition.h"
#include "facesdata.h"
#include "logger.h"

FaceAi::FaceAi(QObject *parent) : QObject(parent)
{
    fr_engine_ = std::make_shared<FaceRecognition>();
    if (fr_engine_) {
        fr_engine_->Install();
    }
}

FaceAi::~FaceAi()
{
    if (fr_engine_) {
        fr_engine_->UnInstall();
    }
}

bool FaceAi::AiAnalysis(const FacesData &data)
{
    ASVLOFFSCREEN inputImg = { 0 };
    inputImg.u32PixelArrayFormat = data.GetFormat();
    inputImg.i32Width = data.FrameWidth();
    inputImg.i32Height = data.FrameHeight();
    inputImg.ppu8Plane[0] = (MUInt8*)data.GetMat().data;
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
        return false;
    }


    return true;
}

void FaceAi::RecvDetectedData(const FacesData &data)
{

}
