#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include "cameractrl.h"
#include "cameraflowevent.h"
#include "hkcamera.h"
#include "framedata.h"
#include "framedataprocess.h"
#include "printexectime.h"


namespace {
    MainWindow *g_main_window_ = nullptr;
}

MainWindow* GetMainWindow() {
    return g_main_window_;
}

//////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->camera->setScaledContents(true); //开启图片自适应
    g_main_window_ = this;
}

MainWindow::~MainWindow()
{  
    if (camera_) {
        camera_->UnInitCarema();
    }

    delete ui;
}

void MainWindow::Initialize()
{
    qRegisterMetaType<FrameData>("FrameData&");

    frame_data_process_ = std::make_shared<FrameDataProcess>();
    QThread *thread = new QThread();
    frame_data_process_->moveToThread(thread);
    connect(this, SIGNAL(carame_signal(FrameData&)), frame_data_process_.get(), SLOT(RecvCameraData(FrameData&)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    camera_ctrl_ = std::make_shared<CameraCtrl>();
    int w = ui->camera->geometry().width();
    int h = ui->camera->geometry().height();
    camera_ctrl_->setFixedSize(w,h);

    //开启摄像头
    HWND hwnd = (HWND)winId();
    camera_ = std::make_shared<HkCamera>(hwnd);
    camera_->InitCarema();
}

void MainWindow::SendCarameSignal(FrameData &data)
{
    emit carame_signal(data);
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == camera_flow_event) {
        CameraFLowEvent *flow_event = static_cast<CameraFLowEvent*>(event);
        if (nullptr != flow_event) {
           // PrintExecTime time;
            //视频显示
            cv::Mat cvImg = flow_event->GetShowMat();
            QImage image =QImage((const unsigned char*)(cvImg.data),
                                 cvImg.cols, cvImg.rows,
                                 cvImg.cols*cvImg.channels(),
                                 QImage::Format_RGB888);
            ui->camera->setPixmap(QPixmap::fromImage(image));
            this->update();
        }
    }

    return QWidget::event(event);
}
