#include "facefilterbarrier.h"
#include "faceai.h"


FaceFilterBarrier::FaceFilterBarrier(QObject *parent) : QObject(parent)
{
    ai_ = std::make_shared<FaceAi>();
}

FaceFilterBarrier::~FaceFilterBarrier()
{

}

int FaceFilterBarrier::Barrier(const FacesData &data)
{


    return 0;
}
