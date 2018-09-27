#ifndef FACEFILTERBARRIER_H
#define FACEFILTERBARRIER_H

#include <QObject>
#include <memory>

class FaceAi;
class FaceFeature;

//人脸过滤屏障, 防止同一人，频繁识别
class FaceFilterBarrier : public QObject
{
    Q_OBJECT
public:
    explicit FaceFilterBarrier(QObject *parent = nullptr);

    ~FaceFilterBarrier();

    /*返回码：
     * -1，表示人脸特征提取失败
     * 0, 表示队列里没有此人的特征
     * 1，表示找到了此人的特征
    */
    int Barrier(const FaceFeature &feature);
signals:

public slots:
private:
    std::shared_ptr<FaceAi> ai_ = nullptr;
};

#endif // FACEFILTERBARRIER_H
