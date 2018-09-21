#include "face_analysis_model.h"
#include "faceai.h"
#include "facesdata.h"
#include "facefilterbarrier.h"
#include "face_feature_library.h"
#include "logger.h"

FaceAnalysisModel::FaceAnalysisModel(QObject *parent) : QObject(parent)
{
    face_filter_.reset(new FaceFilterBarrier());
}

FaceAnalysisModel::~FaceAnalysisModel()
{

}

bool FaceAnalysisModel::Init()
{
    auto face_lib = GetFeatureLib();
    if (!face_lib->LoadRegFaceLib()) {
        LogE("加载员工数据失败");
        return false;
    }

    return true;
}

void FaceAnalysisModel::RecvDetectedData(const FacesData &data)
{
    FacesData face_data = data;

    int index = 0;
    std::vector<QRect> faces = data.GetFacesRect();
    for (auto iter = faces.cbegin(); iter != faces.end(); ++iter, index++) {
        face_data.SetIndex(index);
        //人脸缓存队列，同一人被频繁识别
        face_filter_->Barrier(face_data);
    }
}
