#ifndef FACE_ANALYSIS_MODEL_H
#define FACE_ANALYSIS_MODEL_H

#include <QObject>
#include <memory>

class FaceAi;
class FacesData;
class FaceFeature;
class FaceFilterBarrier;

//人脸分析核心，人脸的比对都在这里调度
class FaceAnalysisModel : public QObject
{
    Q_OBJECT
public:
    explicit FaceAnalysisModel(QObject *parent = nullptr);

    ~FaceAnalysisModel();

     bool Init();

     unsigned long GetFrameSerial();

signals:
public slots:
    void RecvDetectedData(const FacesData& data);
private:
    std::shared_ptr<FaceFilterBarrier> face_filter_ = nullptr;

    std::shared_ptr<FaceAi> face_ai_ = {nullptr};//专门用于提取人脸特征

    unsigned long frame_serial_ = 0;//帧序列号
};

#endif // FACE_ANALYSIS_MODEL_H
