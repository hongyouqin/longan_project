#ifndef FACE_ANALYSIS_MODEL_H
#define FACE_ANALYSIS_MODEL_H

#include <QObject>
#include <vector>
#include <memory>

class FaceAi;
class FacesData;
class FaceFilterBarrier;

//人脸分析核心，人脸的比对都在这里调度
class FaceAnalysisModel : public QObject
{
    Q_OBJECT
public:
    explicit FaceAnalysisModel(QObject *parent = nullptr);

    ~FaceAnalysisModel();
signals:

public slots:
    void RecvDetectedData(const FacesData& data);
private:
    std::vector<std::shared_ptr<FaceAi>> reg_face_ai_;//用于注册员工的识别

    std::shared_ptr<FaceFilterBarrier> face_filter_ = nullptr;
};

#endif // FACE_ANALYSIS_MODEL_H
