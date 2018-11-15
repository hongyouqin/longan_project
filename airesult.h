#ifndef AIRESULT_H
#define AIRESULT_H
#include <QMetaType>
#include <memory>

class FaceFeature;
//帧结果
struct AiResult
{
    unsigned long frame_serial_;//帧序列号

    bool find_match_;//是否找到匹配

    int package_serial_; //包的序列号

    int package_num_;//包的数量

    float face_score_;

    std::shared_ptr<FaceFeature> feature_;
};

Q_DECLARE_METATYPE(AiResult);

#endif // AIRESULT_H
