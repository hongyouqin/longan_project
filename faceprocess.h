#ifndef FACEPROCESS_H
#define FACEPROCESS_H

#include <QObject>
#include <memory>
#include <atomic>

class FaceTracking;
class FaceDetection;
class FacesData;
class FrameData;
class FaceAnalysisModel;

class FaceProcess : public QObject
{
    Q_OBJECT
public:
    explicit FaceProcess(QObject *parent = nullptr);

    ~FaceProcess();

    std::vector<QRect> Frame(unsigned char *frame_data,int frame_width,int frame_height, unsigned	int input_image_format, int &out_face_orient);

    void set_serial_number(int serial);

    int serial_number() const;

    void SendDetectedSignal();
signals:
    void faces_detected_signal(const FacesData& data); //人脸跟踪信号
public slots:
    void CameraFrame(FrameData& frame); //摄像机帧数据

private:
    std::unique_ptr<FaceTracking> ft_engine_;

    std::unique_ptr<FaceDetection> fd_engine_;

    std::shared_ptr<FaceAnalysisModel> face_analysis_;

    int serial_number_ = 0;
};

#endif // FACEPROCESS_H
