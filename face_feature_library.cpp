#include "face_feature_library.h"
#include "facequeue.h"
#include "facefeature.h"
#include "logger.h"

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

