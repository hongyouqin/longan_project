#include "facefilterbarrier.h"
#include "facequeue.h"
#include "faceai.h"

FaceFilterBarrier::FaceFilterBarrier(QObject *parent) : QObject(parent)
{
    face_cache_.reset(new FaceQueue<FaceFeature>(50));
    ai_ = std::make_shared<FaceAi>();
}

FaceFilterBarrier::~FaceFilterBarrier()
{

}

bool FaceFilterBarrier::Barrier()
{

    return true;
}
