#ifndef FACEAI_H
#define FACEAI_H

#include <QObject>
#include <memory>

class FaceRecognition;
class FacesData;

//比对人脸的特征
class FaceAi : public QObject
{
    Q_OBJECT
public:
    explicit FaceAi(QObject *parent = nullptr);

    ~FaceAi();

    bool AiAnalysis(const FacesData& data);
signals:

public slots:
    void RecvDetectedData(const FacesData& data);
private:
    std::shared_ptr<FaceRecognition> fr_engine_ = nullptr;
};

#endif // FACEAI_H
