#ifndef FRAMEDATAPROCESS_H
#define FRAMEDATAPROCESS_H

#include <QObject>

class FrameData;

//帧数据加工
class FrameDataProcess : public QObject
{
    Q_OBJECT
public:
    explicit FrameDataProcess(QObject *parent = nullptr);

signals:

public slots:
    void RecvCameraData(FrameData& data);
};

#endif // FRAMEDATAPROCESS_H
