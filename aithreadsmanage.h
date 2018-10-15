#ifndef AITHREADSMANAGE_H
#define AITHREADSMANAGE_H

#include <QObject>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

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

    void CleanupAiContainter();

    void SendFaceFeatureSignal(const FaceFeature& face_feature);

    //监视人脸库变化
    void MonitorFaceLib();

    //通知所有的ai停止处理这一帧
    void NotifyAllAiStop();

    int GetRegFaceNum();

    //是否触发更新
    bool IsTriggerUpdate() const;

    //触发更新
    void TriggerUpdate();

    //重置更新
    void CancelTriggerUpdate();
signals:
    void frame_face_featrue_signal(const FaceFeature& face);
public slots:
private:
    std::vector<std::shared_ptr<FaceAi>> reg_face_ai_;//用于注册员工的Ai识别

    std::shared_ptr<AiResultReport> ai_report_; //识别的结果

    std::mutex reg_face_mutex_;

    //触发更新
    std::atomic_bool trigger_update_ = {false};
};

AiThreadsManage* GetAiManageObj();

#endif // AITHREADSMANAGE_H
