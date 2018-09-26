#ifndef AITHREADSMANAGE_H
#define AITHREADSMANAGE_H

#include <QObject>
#include <vector>
#include <memory>
#include <mutex>

class FaceFeature;
class FaceAi;
class AiResultReport;

//人脸识别线程管理
class AiThreadsManage : public QObject
{
    Q_OBJECT
public:
    explicit AiThreadsManage(QObject *parent = nullptr);

    void AllocateThreads();

    void SendFaceFeatureSignal(const FaceFeature& face_feature);

    //通知所有的ai停止处理这一帧
    void NotifyAllAiStop();

    int GetRegFaceNum();
signals:
    void frame_face_featrue_signal(const FaceFeature& face);
public slots:
private:
    std::vector<std::shared_ptr<FaceAi>> reg_face_ai_;//用于注册员工的识别

    std::shared_ptr<AiResultReport> ai_report_; //识别的结果

    std::mutex reg_face_mutex_;
};

AiThreadsManage* GetAiManageObj();

#endif // AITHREADSMANAGE_H
