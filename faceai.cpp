#include "faceai.h"
#include "face_recognition.h"
#include "facesdata.h"
#include "facefeature.h"
#include "logger.h"

FaceAi::FaceAi(QObject *parent) : QObject(parent)
{
    face_model_.reset(new FACEMODEL());

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

bool FaceAi::ExtractFeature(const FacesData &data)
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

    //提取脸部特征
    auto faces = data.GetFacesRect();
    int index = data.GetIndex();
    AFR_FSDK_FACEINPUT face_result;
    face_result.rcFace.left = faces[index].left();
    face_result.rcFace.top = faces[index].top();
    face_result.rcFace.right = faces[index].right();
    face_result.rcFace.bottom = faces[index].bottom();
    face_result.lOrient =data.GetFaceOrient();
    long ret = fr_engine_->ExtractFRFeature(&inputImg, &face_result, (LPAFR_FSDK_FACEMODEL)face_model_.get());
    if (ret != 0) {
        LogE("人脸特征提取失败: %d", ret);
        return false;
    }


    return true;
}

float FaceAi::FaceComparison(const std::shared_ptr<FaceFeature> &feature)
{
    AFR_FSDK_FACEMODEL ref_feature;
    ref_feature.pbFeature = face_model_->pbFeature;
    ref_feature.lFeatureSize = face_model_->lFeatureSize;

    AFR_FSDK_FACEMODEL probe_feature;
    probe_feature.pbFeature = feature->feature_.get();
    probe_feature.lFeatureSize = feature->feature_size_;
    float simil_score = 0.0f;
    long ret = fr_engine_->FacePairMatching(&ref_feature, &probe_feature, &simil_score);
    if (ret != 0) {
        LogE("人脸比对失败: %d", ret);
        return simil_score;
    }

    return simil_score;
}

void FaceAi::RecvDetectedData(const FacesData &data)
{

}
