#include "face_feature_library.h"
#include "configs.h"
#include "data_center.h"
#include "facequeue.h"
#include "facefeature.h"
#include "logger.h"

FaceFeatureLibrary* GetFeatureLib() {
    static FaceFeatureLibrary lib;
    return &lib;
}

FaceFeatureLibrary::FaceFeatureLibrary(QObject *parent) : QObject(parent)
{
    face_cache_.reset(new FaceQueue<std::shared_ptr<FaceFeature>>(100));
    stranger_face_cache_.reset(new FaceQueue<std::shared_ptr<FaceFeature>>(100));
}

FaceFeatureLibrary::~FaceFeatureLibrary()
{

}

void FaceFeatureLibrary::AddCache(const std::shared_ptr<FaceFeature>& feature)
{
    std::lock_guard<std::mutex> lock(cache_mutex_);
    face_cache_->Push(feature);
}

int FaceFeatureLibrary::GetCacheLen()
{
    std::lock_guard<std::mutex> lock(cache_mutex_);
    return face_cache_->Len();
}

std::shared_ptr<FaceFeature> FaceFeatureLibrary::GetCache(int index)
{
    std::lock_guard<std::mutex> lock(cache_mutex_);
    return face_cache_->GetAt(index);
}

void FaceFeatureLibrary::AddStrangerCache(const std::shared_ptr<FaceFeature> &feature)
{
    std::lock_guard<std::mutex> lock(sc_mutex_);
    stranger_face_cache_->Push(feature);
}

int FaceFeatureLibrary::GetStrangerCacheLen()
{
    std::lock_guard<std::mutex> lock(sc_mutex_);
    return stranger_face_cache_->Len();
}

std::shared_ptr<FaceFeature> FaceFeatureLibrary::GetStrangerCache(int index)
{
    std::lock_guard<std::mutex> lock(sc_mutex_);
    return stranger_face_cache_->GetAt(index);
}

bool FaceFeatureLibrary::LoadRegFaceLib()
{
    auto dc = Configs::GetDataCenterConfig();
    DataCenter data_center(grpc::CreateChannel(
                               dc->addr, grpc::InsecureChannelCredentials()));

    std::vector<std::shared_ptr<FaceFeature>> features;
    bool res = data_center.ExtractFaceRegTableDatas(features);
    if (!res) {
        return false;
    }

    ClearupRegFaceLib();

    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    reg_face_lib_ = std::move(features);

    return true;
}

void FaceFeatureLibrary::ClearupRegFaceLib()
{
    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    reg_face_lib_.clear();
}

size_t FaceFeatureLibrary::GetRegFaceCount()
{
    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    return reg_face_lib_.size();
}

std::vector<std::shared_ptr<FaceFeature> > FaceFeatureLibrary::GetRegFaceLib()
{
    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    return reg_face_lib_;
}

