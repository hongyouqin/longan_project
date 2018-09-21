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
    face_cache_.reset(new FaceQueue<std::shared_ptr<FaceFeature>>(50));
}

FaceFeatureLibrary::~FaceFeatureLibrary()
{

}

void FaceFeatureLibrary::AddCache(const std::shared_ptr<FaceFeature>& feature)
{
    face_cache_->Push(feature);
}

int FaceFeatureLibrary::GetCacheLen() const
{
    return face_cache_->Len();
}

std::shared_ptr<FaceFeature> FaceFeatureLibrary::GetCache(int index)
{
    int len = face_cache_->Len();
    if (index >= len) {
        LogA("%d索引超出队列了");
        return nullptr;
    }
    return face_cache_->GetAt(index);
}

bool FaceFeatureLibrary::LoadRegFaceLib()
{

    auto dc = Configs::GetDataCenterConfig();
    DataCenter data_center(grpc::CreateChannel(
                               dc->addr, grpc::InsecureChannelCredentials()));
    proto::ExtractFaceParam param;
    ::proto::StorageReply reply;
    bool res = data_center.ExtractFaceRegTableDatas(param, &reply);
    if (res) {
        auto faces = reply.face_reg_tables();
        for (auto iter = faces.cbegin(); iter != faces.cend(); ++iter) {
            std::shared_ptr<FaceFeature> feature = std::make_shared<FaceFeature>();
            feature->name = (*iter).name();
            feature->user_id_ = (*iter).user_id();
            feature->face_photo = (*iter).face_photo();
            feature->timestamp = (*iter).acquisition_time();
            int size = ((*iter).face_feature()).size();
            unsigned char* buf = (unsigned char*)(((*iter).face_feature()).c_str());
            std::unique_ptr<unsigned char[], deleter> face_feature(new unsigned char[size](), auto_deleter);
            memcpy(face_feature.get(), buf, size);
            feature->feature_ = std::move(face_feature);
            feature->feature_size_ = (*iter).face_size();
            reg_face_lib_.emplace_back(feature);
        }

    }


    return false;
}

