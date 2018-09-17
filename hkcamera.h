#ifndef HKCAMERA_H
#define HKCAMERA_H

typedef unsigned int HWND;


class HkCamera
{
public:
    explicit HkCamera(HWND hwnd);

    ~HkCamera();

    bool InitCarema();

    void UnInitCarema();

    void set_hwnd(HWND hwnd);
private:
    bool camera_is_open_ = false;

    int user_id_ = -1;

    int real_play_handle_ = -1; //实时流句柄

    HWND hwnd_ = 0;
};

#endif // HKCAMERA_H
