#include "mainwindow.h"
#include <QThread>
#include <thread>
#include "aithreadsmanage.h"
#include "ui_mainwindow.h"
#include "cameractrl.h"
#include "cameraflowevent.h"
#include "hkcamera.h"
#include "facesdata.h"
#include "framedata.h"
#include "framedataprocess.h"
#include "face_feature_library.h"
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
   // disconnect(this, SIGNAL(frame_push_signal(FrameData&)), face_process_.get(), SLOT(CameraFrame(FrameData&)));
    LogI("析构MainWindow");
    face_process_.reset();

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

    face_process_ = std::make_shared<FaceProcess>();
    QThread *face_thread = new QThread();
    face_process_->moveToThread(face_thread);
    connect(this, SIGNAL(frame_push_signal(FrameData&)), face_process_.get(), SLOT(CameraFrame(FrameData&)));
    connect(face_thread, SIGNAL(finished()), face_thread, SLOT(deleteLater()));

   auto system = Configs::GetSystemConfig();
   control_frame_frequency_ = system->frame_frequency;

   if (system->show_camera == 1) {
        connect(face_process_.get(), SIGNAL(faces_detected_signal(const FacesData&)), camera_ctrl_.get(), SLOT(RecvDetectedFaces(const FacesData&)));
   }
   face_thread->start();

    LogI("加载员工数据");

    {
        PrintExecTime time("加载人脸库");
        auto face_lib = GetFeatureLib();
        if (!face_lib->LoadRegFaceLib()) {
            LogE("加载员工数据失败");
           // return false;
        }
    }

    //分配人脸注册分析线程
    GetAiManageObj()->AllocateThreads();

    //监听人脸注册库
    std::thread mt(&AiThreadsManage::MonitorFaceLib, GetAiManageObj());
    mt.detach();

    std::thread dk(&AiThreadsManage::XXXFace, GetAiManageObj());
    dk.detach();

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

           int limit_frame = system->frame_frequency;
           //跳帧分析
           if (control_frame_frequency_ >= limit_frame) {
               control_frame_frequency_ = 1;
               //视频流推送到人脸引擎处理
               FrameData data;
               data.SetMat(flow_event->GetFaceMat());
               data.SetFrameTime(flow_event->frame_time_);
               emit frame_push_signal(data);
           } else {
             control_frame_frequency_++;
           }

        }
    }

    return QWidget::event(event);
}
