#include "faceai.h"
#include "airesult.h"
#include "aithreadsmanage.h"
#include "configs.h"
#include "face_recognition.h"
#include "facesdata.h"
#include "facefeature.h"
#include "face_feature_library.h"
#include "logger.h"

FaceAi::FaceAi(QObject *parent) : QObject(parent)
{
    segment_numer_.start = segment_numer_.end = 0;

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

bool FaceAi::ExtractFeature(const FacesData &data, FACEMODEL* face_model/* = nullptr*/)
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

    auto faces = data.GetFacesRect();
    int index = data.GetIndex();
    AFR_FSDK_FACEINPUT face_result;
    face_result.rcFace.left = faces[index].left();
    face_result.rcFace.top = faces[index].top();
    face_result.rcFace.right = faces[index].right();
    face_result.rcFace.bottom = faces[index].bottom();
    face_result.lOrient =data.GetFaceOrient();
    long ret = fr_engine_->ExtractFRFeature(&inputImg, &face_result, (LPAFR_FSDK_FACEMODEL)face_model);
    if (ret != 0) {
        LogE("人脸特征提取失败: %d", ret);
        return false;
    }

    return true;
}

float FaceAi::FaceComparison(const std::shared_ptr<FaceFeature> &feature, const FaceFeature& ref_feature)
{
    AFR_FSDK_FACEMODEL ref_face_model;
    ref_face_model.pbFeature = ref_feature.feature_.get();
    ref_face_model.lFeatureSize = ref_feature.feature_size_;

    AFR_FSDK_FACEMODEL probe_feature;
    probe_feature.pbFeature = feature->feature_.get();
    probe_feature.lFeatureSize = feature->feature_size_;
    float simil_score = 0.0f;
    long ret = fr_engine_->FacePairMatching(&ref_face_model, &probe_feature, &simil_score);
    if (ret != 0) {
        LogE("人脸比对失败: %d", ret);
        return simil_score;
    }

    return simil_score;
}

void FaceAi::SetSegmentNumber(const SEGMENTNUMER &segment)
{
    segment_numer_ = segment;
}

void FaceAi::set_serial_number(int number)
{
    serial_number_ = number;
}

int FaceAi::serial_number() const
{
    return serial_number_;
}

void FaceAi::set_type(AiTypeEnum type)
{
    type_ = type;
}

AiTypeEnum FaceAi::type() const
{
    return type_;
}

void FaceAi::set_stop_signal(bool stop)
{
    stop_signal_ = stop;
}

bool FaceAi::stop_signal() const
{
    return stop_signal_;
}

void FaceAi::RecvEmployeeData(const FaceFeature& face)
{
    auto face_lib = GetFeatureLib()->GetRegFaceLib();
    int count = static_cast<int>(face_lib.size());
    int start = segment_numer_.start;
    int end = segment_numer_.end;
    if (end > count) {
        end = count;
    }

    std::shared_ptr<FaceFeature> result = nullptr;
    auto config = Configs::GetSystemConfig();
    float ref_score = config->face_score;

    float score = 0.0f;
    for (int index = start; index < end; ++index) {
        if (stop_signal()) {
            break;
        }
        auto feature = face_lib[index];
        score = FaceComparison(feature, face);
        if (score > ref_score) {
           result = feature;
           break;
        }
    }

    if (stop_signal()) {
        set_stop_signal(false);
        LogI("%d线程停止识别", serial_number());

        AiResult ai_result;
        ai_result.find_match_ = false;
        ai_result.frame_serial_ = face.frame_number_;
        ai_result.feature_ = std::make_shared<FaceFeature>(face);
        ai_result.package_serial_ = serial_number();
        ai_result.package_num_ = GetAiManageObj()->GetRegFaceNum();
        ai_result_signal(ai_result);
        return;
    }

    if (result) {
        //匹配上了

        //通知其他线程停止识别，（包括陌生人线程）
        GetAiManageObj()->NotifyAllAiStop();
        set_stop_signal(false);
        //推送
       // LogI("识别信息： name=%s; userid=%d; face_photo=%s; timestamp=%u", result->name.c_str(), result->user_id_, result->face_photo.c_str(), result->timestamp);
        AiResult employee_res;
        employee_res.find_match_ = true;
        employee_res.frame_serial_ = face.frame_number_;
        employee_res.feature_ = result;
        employee_res.package_serial_ = serial_number();
        employee_res.package_num_ = GetAiManageObj()->GetRegFaceNum();
        ai_employee_result_signal(employee_res);
    }

    //推送结果
    AiResult ai_result;
    ai_result.find_match_ = (result) ? true : false;
    ai_result.frame_serial_ = face.frame_number_;
    ai_result.feature_ = std::make_shared<FaceFeature>(face);
    ai_result.package_serial_ = serial_number();
    ai_result.package_num_ = GetAiManageObj()->GetRegFaceNum();
    ai_result_signal(ai_result);

}

void FaceAi::RecvStorageData(const FaceFeature& face)
{

}
