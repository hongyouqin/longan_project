#include "cameractrl.h"
#include <qpainter.h>


CameraCtrl::CameraCtrl(QLabel *parent) : QLabel(parent)
{

}

void CameraCtrl::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);

    if (!rect_faces_.empty()) {
        painter.setPen(Qt::red);
        QSize sz = this->size();
        float width_factor = sz.width() / (float)frame_width_;
        float height_factor = sz.height() / (float) frame_height_;
        for (auto iter = rect_faces_.cbegin(); iter != rect_faces_.cend(); ++iter) {
            QRect rect = *iter;
            int left = rect.left() * width_factor;
            int top = rect.top() * height_factor;
            int right = rect.right() * width_factor;
            int bottom = rect.bottom() * height_factor;
            QRect paint_rect(left, top, right - left, bottom - top);
            painter.drawRect(paint_rect);
        }
        rect_faces_.clear();
        frame_width_ = 0;
        frame_height_ = 0;
    }
}

void CameraCtrl::RecvDetectedFaces(const FacesData& data)
{
    rect_faces_ = data.GetFacesRect();
    frame_width_ = data.FrameWidth();
    frame_height_ = data.FrameHeight();
    update();
}
