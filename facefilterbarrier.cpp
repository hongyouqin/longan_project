#include "facefilterbarrier.h"
#include "configs.h"
#include "faceai.h"
#include "face_feature_library.h"
#include "facefeature.h"
#include <chrono>
#include "logger.h"
#include "push_redis.h"


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

    for (int index = 0; index < len; ++index) {
        auto face = lib->GetCache(index);
        simil_score = ai_->FaceComparison(face, feature);
        if (simil_score > ref_score) {
            result = face;
            is_recognition = true;
            if (face->is_employee_) {
                break;
            }
        }
    }

    if (is_recognition) {
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - result->expiry_time_);
        double c = (double)duration.count();
        double milliseconds = c * std::chrono::milliseconds::period::num / std::chrono::milliseconds::period::den;
        int time = static_cast<int>(milliseconds * 1000);
        if (time > 500) {
            LogI("缓存里有目前有：%d条数据", len);
            if (result->name.empty()) {
                LogI("==== name=%s； user_id=%d; face_photo=%s", "陌生人", result->user_id_, result->face_photo.c_str());
                PushRedis redis;
                std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                redis.Push("陌生人", "1", "http://192.168.79.206:8005/face_lib/test/15372566646368847403.jpg", unix_timestamp);
            } else {
                LogI("员工 name=%s； user_id=%d; face_photo=%s; 识别分数:%f", result->name.c_str(), result->user_id_, result->face_photo.c_str(), simil_score);
                //推送给php
                PushRedis redis;
                std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                redis.Push(result->name, "1", result->face_photo, unix_timestamp);
            }

            result->expiry_time_ = now;
        } else {
            //LogI("同一人");
        }
        return 1;
    }


    return 0;
}
