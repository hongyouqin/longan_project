#include "face_analysis_model.h"
#include <thread>
#include "aithreadsmanage.h"
#include "faceai.h"
#include "facesdata.h"
#include "facefeature.h"
#include "facefilterbarrier.h"
#include "logger.h"
#include "printexectime.h"


FaceAnalysisModel::FaceAnalysisModel(QObject *parent) : QObject(parent)
{
}

FaceAnalysisModel::~FaceAnalysisModel()
{
    face_ai_.reset();
}

bool FaceAnalysisModel::Init()
{
    face_filter_ = std::make_shared<FaceFilterBarrier>();

    face_ai_.reset(new FaceAi());

    return true;
}

unsigned long FaceAnalysisModel::GetFrameSerial()
{
    if(++frame_serial_ > 1000000) {
        LogE("帧编号重置");
        frame_serial_ = 0;
    }
    return frame_serial_;
}

void FaceAnalysisModel::Analys(const FacesData &data)
{
    FACEMODEL face_model;
    if (!face_ai_->ExtractFeature(data, &face_model)) {
        return;
    }

    FaceFeature face_feature;
    std::unique_ptr<unsigned char[], deleter> feature(new unsigned char[face_model.lFeatureSize](), auto_deleter);
    memcpy(feature.get(), face_model.pbFeature, face_model.lFeatureSize);
    face_feature.feature_ = std::move(feature);
    face_feature.feature_size_ = face_model.lFeatureSize;
    face_feature.frame_number_ = GetFrameSerial();
    face_feature.mat_ = data.GetMat();
    face_feature.frame_time_ = data.FrameTime();

    //人脸缓存队列，同一人被频繁识别
    if (1 == face_filter_->Barrier(face_feature)) {
        //人脸找到
        return;
    }
    GetAiManageObj()->SendFaceFeatureSignal(face_feature);
}

void FaceAnalysisModel::RecvDetectedData(const FacesData &data)
{
   // PrintExecTime time;
    FacesData face_data = data;
    int index = 0;
    std::vector<QRect> faces = data.GetFacesRect();
    for (auto iter = faces.cbegin(); iter != faces.end(); ++iter, index++) {
        face_data.SetIndex(index);
        std::thread analys(&FaceAnalysisModel::Analys, this, face_data);
        analys.detach();
    }
}
