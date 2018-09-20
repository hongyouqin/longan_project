#include "face_analysis_model.h"
#include "faceai.h"
#include "facefilterbarrier.h"

FaceAnalysisModel::FaceAnalysisModel(QObject *parent) : QObject(parent)
{
    face_filter_.reset(new FaceFilterBarrier());
}

FaceAnalysisModel::~FaceAnalysisModel()
{

}
