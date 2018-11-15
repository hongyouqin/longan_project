#ifndef FACE_FEATURE_LIBRARY_H
#define FACE_FEATURE_LIBRARY_H

#include <QObject>
#include <memory>
#include <vector>
#include <mutex>

template<class T>
class FaceQueue;

class FaceFeature;

//人脸特征库
class FaceFeatureLibrary : public QObject
{
    Q_OBJECT
public:
    explicit FaceFeatureLibrary(QObject *parent = nullptr);

    ~FaceFeatureLibrary();

    void AddCache(const std::shared_ptr<FaceFeature>& feature);

    int GetCacheLen();

    void ClearCache();

    //这里更新时间，做了特殊处理，注意
    bool UpdateCacheTime(std::shared_ptr<FaceFeature> &face, const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& now, int tap_value);//

    std::shared_ptr<FaceFeature> GetCache(int index);

    void AddStrangerCache(const std::shared_ptr<FaceFeature>& feature);

    int GetStrangerCacheLen();

    std::shared_ptr<FaceFeature> GetStrangerCache(int index);

    //加载注册人脸库数据
    bool LoadRegFaceLib();

    void ClearupRegFaceLib(); //清空注册人脸库

    //获取注册人脸个数
    size_t GetRegFaceCount();

    std::vector<std::shared_ptr<FaceFeature>> GetRegFaceLib();

signals:

public slots:

private:
    std::unique_ptr<FaceQueue<std::shared_ptr<FaceFeature>>> face_cache_; //注册人脸缓存

    std::unique_ptr<FaceQueue<std::shared_ptr<FaceFeature>>> stranger_face_cache_;//未注册人脸缓存

    std::vector<std::shared_ptr<FaceFeature>> reg_face_lib_; //注册的人脸数据库

    std::mutex reg_face_mutex_; //注册人脸库容器互斥量

    std::mutex cache_mutex_;

    std::mutex sc_mutex_;//未注册人脸缓存互斥量
};

FaceFeatureLibrary* GetFeatureLib();

#endif // FACE_FEATURE_LIBRARY_H
