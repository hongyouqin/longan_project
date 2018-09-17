#ifndef CAMERACTRL_H
#define CAMERACTRL_H

#include <QtWidgets/QLabel>
#include <vector>
#include "facesdata.h"

class CameraCtrl : public QLabel
{
    Q_OBJECT
public:
    explicit CameraCtrl(QLabel *parent = nullptr);

private:
    virtual void paintEvent(QPaintEvent *event) override;

    std::vector<QRect> rect_faces_;

    int frame_width_ = 0;

    int frame_height_ = 0;

signals:

public slots:
     void RecvDetectedFaces(const FacesData& data);
};


#endif // CAMERACTRL_H
