#include "mainwindow.h"
#include <QThread>
#include "ui_mainwindow.h"
#include "cameractrl.h"
#include "cameraflowevent.h"
#include "hkcamera.h"
#include "facesdata.h"
#include "framedata.h"
#include "framedataprocess.h"
#include "faceprocess.h"
#include "printexectime.h"
#include "logger.h"
#include "configs.h"


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
    auto system = Configs::GetSystemConfig();
    if (system->show_camera == 1) {
        disconnect(this, SIGNAL(frame_push_signal(FrameData&)), face_process_.get(), SLOT(CameraFrame(FrameData&)));
        face_process_.reset();
    }

    if (camera_) {
        camera_->UnInitCarema();
    }

    delete ui;
}

void MainWindow::Initialize()
{
    //安装日志
    setbuf(stdout, NULL);
    Logger::InitLog();
    Logger::StartLog();

    if (!Configs::LoadConfig()) {
        return;
    }

    qRegisterMetaType<FrameData>("FrameData&");
    qRegisterMetaType<FacesData>("FacesData&");

    frame_data_process_ = std::make_shared<FrameDataProcess>();
    QThread *thread = new QThread();
    frame_data_process_->moveToThread(thread);
    connect(this, SIGNAL(carame_signal(FrameData&)), frame_data_process_.get(), SLOT(RecvCameraData(FrameData&)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    camera_ctrl_ = std::make_shared<CameraCtrl>(ui->camera);
    int w = ui->camera->geometry().width();
    int h = ui->camera->geometry().height();
    camera_ctrl_->setFixedSize(w,h);

    auto system = Configs::GetSystemConfig();
    if (system->show_camera == 1) {
        face_process_ = std::make_shared<FaceProcess>();
        QThread *face_thread = new QThread();
        face_process_->moveToThread(face_thread);
        connect(this, SIGNAL(frame_push_signal(FrameData&)), face_process_.get(), SLOT(CameraFrame(FrameData&)));
        connect(face_process_.get(), SIGNAL(faces_detected_signal(const FacesData&)), camera_ctrl_.get(), SLOT(RecvDetectedFaces(const FacesData&)));
        connect(face_thread, SIGNAL(finished()), face_thread, SLOT(deleteLater()));
        face_thread->start();
    } else {
        CreateFaceMultiProcess();
    }

    //开启摄像头
    HWND hwnd = (HWND)winId();
    camera_ = std::make_shared<HkCamera>(hwnd);
    camera_->InitCarema();
}

void MainWindow::SendCarameSignal(FrameData &data)
{
    emit carame_signal(data);
}

void MainWindow::CreateFaceMultiProcess()
{
    auto system = Configs::GetSystemConfig();
    for (int index = 0; index < system->face_process_num; ++index) {
        std::shared_ptr<FaceProcess> process = std::make_shared<FaceProcess>();
        process->set_serial_number(index+1);
        QThread *face_thread = new QThread();
        process->moveToThread(face_thread);
        connect(this, SIGNAL(frame_push_signal(FrameData&)), process.get(), SLOT(CameraFrame(FrameData&)));
        connect(face_thread, SIGNAL(finished()), face_thread, SLOT(deleteLater()));
        faces_analyze_map_[index+1] = process;
        face_thread->start();
    }
}

int MainWindow::LotNumber()
{
    number_++;
    int count = static_cast<int>(faces_analyze_map_.size());
    if (number_ > count) {
        number_ = 0;
    }
    return number_;
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == camera_flow_event) {
        CameraFLowEvent *flow_event = static_cast<CameraFLowEvent*>(event);
        if (nullptr != flow_event) {
           auto system = Configs::GetSystemConfig();
           if (system->show_camera == 1) {
               //视频显示
               cv::Mat cvImg = flow_event->GetShowMat();
               QImage image =QImage((const unsigned char*)(cvImg.data),
                                    cvImg.cols, cvImg.rows,
                                    cvImg.cols*cvImg.channels(),
                                    QImage::Format_RGB888);
               ui->camera->setPixmap(QPixmap::fromImage(image));
               this->update();
           }
            //视频流推送到人脸引擎处理
            FrameData data;
            int number = LotNumber();
            data.SetSerial(number);
            data.SetMat(flow_event->GetFaceMat());
            emit frame_push_signal(data);
        }
    }

    return QWidget::event(event);
}
