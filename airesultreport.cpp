#include "airesultreport.h"
#include <chrono>
#include <thread>
#include "airesult.h"
#include "configs.h"
#include "facefeature.h"
#include "face_feature_library.h"
#include "faceai.h"
#include "logger.h"
#include "push_redis.h"


AiResultReport::AiResultReport(QObject *parent) : QObject(parent)
{
    ai_ = std::make_shared<FaceAi>();
}

void AiResultReport::PushStranger(const FaceFeature &feature, int package_serial)
{
    auto lib = GetFeatureLib();
    int len = lib->GetStrangerCacheLen();
    auto config = Configs::GetSystemConfig();
    float ref_score = config->face_score;
    float simil_score = 0.0f;
    bool exist = false;
    std::shared_ptr<FaceFeature> result = nullptr;

    for (int index = 0; index < len; ++index) {
        auto face = lib->GetStrangerCache(index);
        simil_score = ai_->FaceComparison(face, feature);
        if (simil_score > ref_score) {
            exist = true;
            result = face;
            break;
        }
    }

    if (!exist) {
        // add cache
        auto face = std::make_shared<FaceFeature>(feature);
        face->expiry_time_ = std::chrono::system_clock::now();
        lib->AddStrangerCache(face);

        LogI("*****推送新陌生人%d", package_serial);

        //推送给php
        std::thread push_info([&](const std::string& name, const std::string& photo){
            PushRedis redis;
            std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            redis.Push(name, "1", photo, unix_timestamp);
        }, "陌生人", "http://192.168.79.206:8005/face_lib/test/15372566646368847403.jpg");
        push_info.detach();
    } else {
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - result->expiry_time_);
        double c = (double)duration.count();
        double milliseconds = c * std::chrono::milliseconds::period::num / std::chrono::milliseconds::period::den;
        int time = static_cast<int>(milliseconds * 1000);
        if (time > 1000*5) {
            LogI("=====推送常见陌生人%d", package_serial);
            //推送给php
            std::thread push_info([&](const std::string& name, const std::string& photo){
                PushRedis redis;
                std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                redis.Push(name, "1", photo, unix_timestamp);
            }, "陌生人", "http://192.168.79.206:8005/face_lib/test/15372566646368847403.jpg");
            push_info.detach();

            result->expiry_time_ = now;
        }
    }

}

void AiResultReport::RecvAiResult(const AiResult &result)
{
    //LogI("frame_serial=%u, package_serial=%d", result.frame_serial_, result.package_serial_);
    std::map<int, std::vector<std::shared_ptr<AiResult>>>::iterator iter;
    std::vector<std::shared_ptr<AiResult>> v;
    if ((iter = result_map_.find(result.package_serial_)) == result_map_.cend()) {
        //如果没有找到，就新创建一个队列
        std::shared_ptr<AiResult> ai_res = std::make_shared<AiResult>(result);
        v.emplace_back(ai_res);
        result_map_[result.package_serial_] = v;
    } else {
       v = iter->second;
       std::shared_ptr<AiResult> ai_res = std::make_shared<AiResult>(result);
       v.emplace_back(ai_res);
       result_map_[result.package_serial_] = v;
    }

    int count = static_cast<int>(v.size());
    if (count == result.package_num_) {
        //一帧数据结果

        //陌生人
        auto is_stranger = [&]() ->bool {
            for (auto iter2 = v.cbegin(); iter2 != v.cend(); ++iter2) {
                if ((*iter2)->find_match_) {
                    return true;
                }
            }
            return false;
        };
        if (!is_stranger()) {
            //陌生人处理
            auto feature = result.feature_.get();
            PushStranger(*feature, result.frame_serial_);
        }

        //删除元素
        if (iter != result_map_.cend()) {
            result_map_.erase(iter);
        } else {
            iter = result_map_.find(result.package_serial_);
            if (iter != result_map_.cend()) {
                result_map_.erase(iter);
            }
        }
    }


}

void AiResultReport::RecvEmployeeResult(const AiResult &result)
{
    LogI(" Employee frame_serial=%u, package_serial=%d", result.frame_serial_, result.package_serial_);
    auto feature = result.feature_;
    feature->expiry_time_ = std::chrono::system_clock::now();
    feature->is_employee_ = true;
    auto face_lib = GetFeatureLib();
    face_lib->AddCache(feature);
    //推送给php
    std::thread push_info([&](const std::string& name, const std::string& photo){
        PushRedis redis;
        std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        redis.Push(name, "1", photo, unix_timestamp);
    }, result.feature_->name, result.feature_->face_photo);
    push_info.detach();
}
