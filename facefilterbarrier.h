#ifndef FACEFILTERBARRIER_H
#define FACEFILTERBARRIER_H

#include <QObject>
#include <memory>
#include "facefeature.h"

class FaceAi;

template<class T>
class FaceQueue;

//人脸过滤屏障, 防止同一人，频繁识别
class FaceFilterBarrier : public QObject
{
    Q_OBJECT
public:
    explicit FaceFilterBarrier(QObject *parent = nullptr);

    ~FaceFilterBarrier();

    bool Barrier();
signals:

public slots:
private:
    std::unique_ptr<FaceQueue<FaceFeature> > face_cache_; //储存最近识别的人脸特征

    std::shared_ptr<FaceAi> ai_ = nullptr;
};

#endif // FACEFILTERBARRIER_H
