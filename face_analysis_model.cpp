#include "face_analysis_model.h"
#include "aithreadsmanage.h"
#include "faceai.h"
#include "facesdata.h"
#include "facefeature.h"
#include "facefilterbarrier.h"
#include "face_feature_library.h"
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
    face_filter_.reset(new FaceFilterBarrier());

    face_ai_.reset(new FaceAi());

    auto face_lib = GetFeatureLib();
    if (!face_lib->LoadRegFaceLib()) {
        LogE("加载员工数据失败");
       // return false;
    }

    //分配线程，根据人脸库的个数分配

    //分配人脸注册分析线程
    GetAiManageObj()->AllocateThreads();

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

void FaceAnalysisModel::RecvDetectedData(const FacesData &data)
{
   // PrintExecTime time;
    FacesData face_data = data;
    int index = 0;
    std::vector<QRect> faces = data.GetFacesRect();
    for (auto iter = faces.cbegin(); iter != faces.end(); ++iter, index++) {
        face_data.SetIndex(index);
        if (!face_ai_->ExtractFeature(face_data)) {
            continue;
        }

        auto face = face_ai_->GetFaceModle();
        FaceFeature face_feature;
        std::unique_ptr<unsigned char[], deleter> feature(new unsigned char[face->lFeatureSize](), auto_deleter);
        memcpy(feature.get(), face->pbFeature, face->lFeatureSize);
        face_feature.feature_ = std::move(feature);
        face_feature.feature_size_ = face->lFeatureSize;
        face_feature.frame_number_ = GetFrameSerial();//GetFrameSerial函数不是线程安全的
        face_feature.mat_ = face_data.GetMat();

        //人脸缓存队列，同一人被频繁识别
        if (1 == face_filter_->Barrier(face_feature)) {
            //人脸找到
            continue;
        }
        GetAiManageObj()->SendFaceFeatureSignal(face_feature);

    }
}
