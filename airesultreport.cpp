#include "airesultreport.h"
#include "airesult.h"
#include "facefeature.h"
#include "logger.h"


AiResultReport::AiResultReport(QObject *parent) : QObject(parent)
{

}

void AiResultReport::RecvAiResult(const AiResult &result)
{
    LogI("frame_serial=%u, package_serial=%d", result.frame_serial_, result.package_serial_);
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
         //   LogI("推送陌生人");
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
}
