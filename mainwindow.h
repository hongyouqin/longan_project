#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class HkCamera;
class CameraCtrl;
class FrameData;
class FrameDataProcess;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Initialize();

    void SendCarameSignal(FrameData& data);
signals:
    void carame_signal(FrameData& data);
private slots:
private:
    virtual bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;

    std::shared_ptr<HkCamera> camera_ = {nullptr};

    std::shared_ptr<CameraCtrl> camera_ctrl_ = nullptr;

    std::shared_ptr<FrameDataProcess> frame_data_process_ = nullptr;
};

MainWindow* GetMainWindow();

#endif // MAINWINDOW_H
