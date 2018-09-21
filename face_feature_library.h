#ifndef FACE_FEATURE_LIBRARY_H
#define FACE_FEATURE_LIBRARY_H

#include <QObject>
#include <memory>
#include <vector>

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

    int GetCacheLen() const;

    std::shared_ptr<FaceFeature> GetCache(int index);

    //加载注册人脸库数据
    bool LoadRegFaceLib();

signals:

public slots:

private:
    std::unique_ptr<FaceQueue<std::shared_ptr<FaceFeature>>> face_cache_; //储存最近识别的人脸特征

    std::vector<std::shared_ptr<FaceFeature>> reg_face_lib_; //注册的人脸数据库
};

FaceFeatureLibrary* GetFeatureLib();

#endif // FACE_FEATURE_LIBRARY_H
