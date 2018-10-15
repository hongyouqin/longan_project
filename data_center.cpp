#include "data_center.h"
#include <thread>
#include "logger.h"
#include "configs.h"
#include "facefeature.h"
#include "aithreadsmanage.h"
#include "face_feature_library.h"
#include "printexectime.h"

DataCenter::DataCenter(std::shared_ptr<grpc::Channel> channel)
    : stub_(proto::LonganDataCenter::NewStub(channel)) {

}

bool DataCenter::StorageEmployeeFace(const ::proto::UserParam& request, ::proto::StorageReply* response) {
    if (!stub_) {
        return false;
    }
    grpc::ClientContext context;
    grpc::Status status = stub_->StorageEmployeeFace(&context, request, response);
    if (status.ok()) {
        return true;
    } else {
        LogE("StorageEmployeeFace call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
    }

    return false;
}

bool DataCenter::StorageStrangerFace(const ::proto::UserParam& request, ::proto::StorageReply* response) {
     if (!stub_) {
        return false;
    }
    grpc::ClientContext context;
    grpc::Status status = stub_->StorageStrangerFace(&context, request, response);
    if (status.ok()) {
        return true;
    } else {
        LogE("StorageStrangerFace call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
    }

    return false;
}

bool  DataCenter::ExtractFaceRegTableDatas(std::vector<std::shared_ptr<FaceFeature>> &faces) {
   grpc::ClientContext context;
   ::proto::Empty empty;
   auto client = stub_->ExtractFaceRegTableDatas(&context, empty);
   ::proto::Feature employee;
   while (client->Read(&employee)) {
       auto face = employee.face();
       std::shared_ptr<FaceFeature> feature = std::make_shared<FaceFeature>();
       feature->name = face.name();
       feature->user_id_ = face.user_id();
       feature->face_photo = face.face_photo();
       feature->timestamp = face.acquisition_time();
       int size = (face.face_feature()).size();
       unsigned char* buf = (unsigned char*)((face.face_feature()).c_str());
       std::unique_ptr<unsigned char[], deleter> face_feature(new unsigned char[size](), auto_deleter);
       memcpy(face_feature.get(), buf, size);
       feature->feature_ = std::move(face_feature);
       feature->feature_size_ = face.face_size();
       faces.emplace_back(feature);
   }

   grpc::Status status = client->Finish();
   if (status.ok()) {
       return true;
   } else {
       LogE("ExtractFaceRegTableDatas call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
   }

   return false;
}

void DataCenter::RegisterService()
{
    grpc::ClientContext context;
    ::proto::Empty empty;
    auto notify = stub_->RegisterClient(&context, empty);
    ::proto::UpdateNotity msg;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> update_time;

    auto system = Configs::GetSystemConfig();
    int delay_update_time = system->delay_upadte_time;

    while(true) {
        bool res = notify->Read(&msg);
        if (!res) {
            LogI("@@@@@@@@@@数据读取完成@@@@@@@@@@");
            continue;
        } else {
            LogI("@@@@@@@@@@有更新通知@@@@@@@@@@");
            if (GetAiManageObj()->IsTriggerUpdate()) {
                continue;
            }
            LogI("触发更新,%d分钟后开始更新", delay_update_time);
            GetAiManageObj()->TriggerUpdate();
            update_time = std::chrono::system_clock::now();
            std::thread update_thread(&DataCenter::UpdateFaceLib, this, update_time, delay_update_time);
            update_thread.detach();
        }
    }
    LogI("@@@@@@@@@@监视线程退出@@@@@@@@@@");
}

void DataCenter::UpdateFaceLib(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> update_time, int delay_time)
{
    while(true) {
        if (GetAiManageObj()->IsTriggerUpdate()) {
            auto now_time = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::minutes>(now_time - update_time);
            double c = (double)duration.count();
            double minutes = c * std::chrono::minutes::period::num / std::chrono::minutes::period::den;
            if (minutes >= delay_time*60) {
                PrintExecTime time("加载人脸库");
                LogI("开始更新人脸库");
                auto face_lib = GetFeatureLib();
                if (face_lib->LoadRegFaceLib()) {
                    GetAiManageObj()->CleanupAiContainter();
                    //分配人脸注册分析线程
                    GetAiManageObj()->AllocateThreads();
                    LogI("更新人脸库完成");
                } else {
                    LogI("更新人脸库失败");
                }

                GetAiManageObj()->CancelTriggerUpdate();
                break;
            } else {
                std::this_thread::yield(); //交出cpu调度时间
            }
        } else {
            std::this_thread::yield(); //交出cpu调度时间
        }
    }

    LogI("更新线程退出");
}
