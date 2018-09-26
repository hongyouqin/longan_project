#ifndef FACEAI_H
#define FACEAI_H

#include <QObject>
#include <memory>
#include <atomic>

typedef struct {
    unsigned char		*pbFeature;		// The extracted features
    int                 lFeatureSize;	// The size of pbFeature
} FACEMODEL;

//区段号
typedef struct {
    int start;
    int end;
} SEGMENTNUMER;

enum class AiTypeEnum {
    NON_AI, //没有类型
    REG_AI, //员工类型
    STORAGE_AI //陌生人类型
};

class FaceRecognition;
class FacesData;
class FaceFeature;
class AiResult;

//比对人脸的特征
class FaceAi : public QObject
{
    Q_OBJECT
public:
    explicit FaceAi(QObject *parent = nullptr);

    ~FaceAi();

    bool ExtractFeature(const FacesData& data); //人脸提取

    float FaceComparison(const std::shared_ptr<FaceFeature>& feature, const FaceFeature& ref_feature); //人脸比对

    void SetSegmentNumber(const SEGMENTNUMER& segment);

    void SetFaceModle(unsigned char* feature, int size);

    std::shared_ptr<FACEMODEL> GetFaceModle();

    void set_serial_number(int number);

    int serial_number() const;

    void set_type(AiTypeEnum type);

    AiTypeEnum type() const;

    void set_stop_signal(bool stop);

    bool stop_signal() const;
signals:
    void ai_result_signal(const AiResult& result);
public slots:
    void RecvEmployeeData(const FaceFeature& face);

    void RecvStorageData(const FaceFeature& face);
private:
    std::shared_ptr<FaceRecognition> fr_engine_ = nullptr;

    std::shared_ptr<FACEMODEL> face_model_ = nullptr;

    std::atomic_int serial_number_ = {-1}; //FaceAi对象序列号，不能重复

    std::atomic_bool stop_signal_ = {false}; //停止识别信号

    //只用于处理区段号的数据识别
    SEGMENTNUMER segment_numer_;

    AiTypeEnum type_ = AiTypeEnum::NON_AI;
};

#endif // FACEAI_H
