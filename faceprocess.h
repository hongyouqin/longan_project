#ifndef FACEPROCESS_H
#define FACEPROCESS_H

#include <QObject>
#include <memory>

class FaceTracking;
class FacesData;
class FrameData;

class FaceProcess : public QObject
{
    Q_OBJECT
public:
    explicit FaceProcess(QObject *parent = nullptr);

    ~FaceProcess();

    std::vector<QRect> Frame(unsigned char *frame_data,int frame_width,int frame_height, unsigned	int input_image_format);

signals:
    void faces_detected_signal(const FacesData& data); //人脸跟踪信号
public slots:
    void CameraFrame(FrameData& frame); //摄像机帧数据

private:
    std::unique_ptr<FaceTracking> ft_engine_;
};

#endif // FACEPROCESS_H
