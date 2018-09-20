#ifndef FACEAI_H
#define FACEAI_H

#include <QObject>
#include <memory>

typedef struct {
    unsigned char		*pbFeature;		// The extracted features
    int                 lFeatureSize;	// The size of pbFeature
} FACEMODEL;

class FaceRecognition;
class FacesData;
class FaceFeature;

//比对人脸的特征
class FaceAi : public QObject
{
    Q_OBJECT
public:
    explicit FaceAi(QObject *parent = nullptr);

    ~FaceAi();

    bool ExtractFeature(const FacesData& data); //人脸提取

    float FaceComparison(const std::shared_ptr<FaceFeature>& feature); //人脸比对
signals:

public slots:
    void RecvDetectedData(const FacesData& data);
private:
    std::shared_ptr<FaceRecognition> fr_engine_ = nullptr;

    std::unique_ptr<FACEMODEL> face_model_ = nullptr;
};

#endif // FACEAI_H
