#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <map>

namespace Ui {
class MainWindow;
}

class HkCamera;
class CameraCtrl;
class FrameData;
class FrameDataProcess;
class FaceProcess;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Initialize();

    void SendCarameSignal(FrameData& data);

    //创建多个线程处理人脸
    void CreateFaceMultiProcess();

    //摇号
    int LotNumber();
signals:
    void carame_signal(FrameData& data);

    void frame_push_signal(FrameData& frame);
private slots:
private:
    virtual bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;

    std::shared_ptr<HkCamera> camera_ = {nullptr};

    std::shared_ptr<CameraCtrl> camera_ctrl_ = nullptr;

    std::shared_ptr<FrameDataProcess> frame_data_process_ = nullptr;

    std::shared_ptr<FaceProcess> face_process_ = nullptr;

    std::map<int, std::shared_ptr<FaceProcess>> faces_analyze_map_;

    int number_ = 0; //号码，不是线程安全的
};

MainWindow* GetMainWindow();

#endif // MAINWINDOW_H
