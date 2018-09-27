#include "airesultreport.h"
#include <chrono>
#include "airesult.h"
#include "facefeature.h"
#include "face_feature_library.h"
#include "logger.h"
#include "push_redis.h"


AiResultReport::AiResultReport(QObject *parent) : QObject(parent)
{

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
            //推送陌生人
          //  LogI("frame_serial=%u, user_id=%d",result.frame_serial_, result.feature_->user_id_);
            LogI("陌生人");
            auto face_lib = GetFeatureLib();
            auto feature = result.feature_;
            feature->expiry_time_ = std::chrono::system_clock::now();
            face_lib->AddCache(feature);
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
    PushRedis redis;
    std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    redis.Push(result.feature_->name, "1", result.feature_->face_photo, unix_timestamp);
}
