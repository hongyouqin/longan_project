#include "facefilterbarrier.h"
#include <chrono>
#include <iomanip>
#include <thread>
#include "configs.h"
#include "faceai.h"
#include "face_feature_library.h"
#include "facefeature.h"
#include "logger.h"
#include "push_redis.h"
#include "printexectime.h"


FaceFilterBarrier::FaceFilterBarrier(QObject *parent) : QObject(parent)
{
    ai_ = std::make_shared<FaceAi>();
}

FaceFilterBarrier::~FaceFilterBarrier()
{

}

int FaceFilterBarrier::Barrier(const FaceFeature &feature)
{
    auto lib = GetFeatureLib();
    int len = lib->GetCacheLen();
    auto config = Configs::GetSystemConfig();
    float ref_score = config->face_score;
    float simil_score = 0.0f;
    std::shared_ptr<FaceFeature> result = nullptr;
    bool is_recognition = false;

    //LogI("缓存里有目前有：%d条数据", len);

    for (int index = 0; index < len; ++index) {
        auto face = lib->GetCache(index);
        simil_score = ai_->FaceComparison(face, feature);
        if (simil_score > ref_score) {
            result = face;
            is_recognition = true;
             break;
        }
    }

    if (is_recognition) {
        auto now = feature.frame_time_;
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - result->expiry_time_);
        double c = (double)duration.count();
        double milliseconds = c * std::chrono::milliseconds::period::num / std::chrono::milliseconds::period::den;
        int time = static_cast<int>(milliseconds * 1000);
        int push_time = config->employee_push_time;
        if (time > push_time) {
            //LogI("缓存里有目前有：%d条数据", len);
            std::time_t frame_time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&frame_time), "%F %T");
            LogI("员工 name=%s； frame_time=%s; face_photo=%s; 识别分数:%f", result->name.c_str(), ss.str().c_str(), result->face_photo.c_str(), simil_score);
            //推送给php
            std::thread push_info([&](const std::string& name, const std::string& photo){
                PushRedis redis;
                std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                redis.Push(name, "1", photo, unix_timestamp);
            },result->name, result->face_photo);
            push_info.detach();
            result->expiry_time_ = now;
        } else {
            std::time_t frame_time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&frame_time), "%F %T");
            std::time_t exprity_time = std::chrono::system_clock::to_time_t(result->expiry_time_);
            std::stringstream ss2;
            ss2 << std::put_time(std::localtime(&exprity_time), "%F %T");

            LogI("name=%s员工已被识别过, 上一次识别时间是：%s, frame_time=%s，face_photo=%s，识别分数:%f", result->name.c_str(),ss2.str().c_str(), ss.str().c_str(), result->face_photo.c_str(), simil_score);
        }
        return 1;
    }


    return 0;
}
