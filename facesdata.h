#ifndef FACESDATA_H
#define FACESDATA_H

#include <vector>
#include <QRect>
#include <QMetaType>

//检测到的人脸数据
class FacesData
{
public:
    FacesData();

    void SetFacesRect(const std::vector<QRect>& rect_faces);

    std::vector<QRect> GetFacesRect() const;

    void SetFrameWidth(int width);

    int FrameWidth() const;

    void SetFrameHeight(int height);

    int FrameHeight() const;
private:
    std::vector<QRect> rect_faces_; //检测到的人脸数据

    int frame_width_ = 0;

    int frame_height_ = 0;
};
Q_DECLARE_METATYPE(FacesData);

#endif // FACESDATA_H
