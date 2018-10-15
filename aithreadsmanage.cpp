#include "aithreadsmanage.h"
#include <QThread>
#include "airesult.h"
#include "airesultreport.h"
#include "faceai.h"
#include "facefeature.h"
#include "face_feature_library.h"
#include "logger.h"
#include "configs.h"
#include "data_center.h"

AiThreadsManage* GetAiManageObj() {
    static AiThreadsManage manage;
    return &manage;
}

AiThreadsManage::AiThreadsManage(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<FaceFeature>("FaceFeature&");
    qRegisterMetaType<AiResult>("AiResult&");

    ai_report_ = std::make_shared<AiResultReport>();
    QThread* thread = new QThread();
    ai_report_->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void AiThreadsManage::AllocateThreads()
{
    auto system = Configs::GetSystemConfig();
    int base_num = system->per_face_process_num;

    auto face_lib = GetFeatureLib();
    int count = static_cast<int>(face_lib->GetRegFaceCount());
    int allocate_thread_number = (count / base_num) + ((count % base_num != 0) ? 1 : 0);
    if (allocate_thread_number == 0) {
        allocate_thread_number = 1;
    }

    LogI("生成%d个Ai处理线程数", allocate_thread_number);

    for (int index = 0; index < allocate_thread_number; ++index) {
        std::shared_ptr<FaceAi> ai= std::make_shared<FaceAi>();
        ai->set_serial_number(1000 + index);
        SEGMENTNUMER segment{index*base_num, (index+1)*base_num - 1};
        ai->SetSegmentNumber(segment);
        ai->set_type(AiTypeEnum::REG_AI);
        QThread* thread = new QThread();
        ai->moveToThread(thread);
        connect(this, SIGNAL(frame_face_featrue_signal(const FaceFeature&)), ai.get(), SLOT(RecvEmployeeData(const FaceFeature&)));
        connect(ai.get(), SIGNAL(ai_result_signal(const AiResult&)), ai_report_.get(), SLOT(RecvAiResult(const AiResult&)));
        connect(ai.get(), SIGNAL(ai_employee_result_signal(const AiResult&)), ai_report_.get(), SLOT(RecvEmployeeResult(const AiResult&)));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
        std::lock_guard<std::mutex> lock(reg_face_mutex_);
        reg_face_ai_.emplace_back(ai);
    }
}

void AiThreadsManage::CleanupAiContainter()
{
    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    reg_face_ai_.clear();
}

void AiThreadsManage::SendFaceFeatureSignal(const FaceFeature& face_feature)
{
    emit frame_face_featrue_signal(face_feature);
}

void AiThreadsManage::MonitorFaceLib()
{
    LogI("开启监听数据中心人脸注册");
    auto dc = Configs::GetDataCenterConfig();
    DataCenter data_center(grpc::CreateChannel(
                               dc->addr, grpc::InsecureChannelCredentials()));
    data_center.RegisterService();
}

void AiThreadsManage::NotifyAllAiStop()
{
    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    for(auto iter = reg_face_ai_.begin(); iter != reg_face_ai_.end(); ++iter) {
        (*iter)->set_stop_signal(true);
    }
}

int AiThreadsManage::GetRegFaceNum()
{
    std::lock_guard<std::mutex> lock(reg_face_mutex_);
    return static_cast<int>(reg_face_ai_.size());
}

bool AiThreadsManage::IsTriggerUpdate() const
{
    return trigger_update_.load();
}

void AiThreadsManage::TriggerUpdate()
{
    trigger_update_.store(true);
}

void AiThreadsManage::CancelTriggerUpdate()
{
    trigger_update_.store(false);
}
