#include "hkcamera.h"
#include <QtWidgets/QApplication>
#include "configs.h"
#include "mainwindow.h"
#include "framedata.h"
#include "includeCn/HCNetSDK.h"
#include "includeCn/PlayM4.h"
#include "logger.h"

LONG lPort; //全局的播放库 port 号

void CALLBACK DvrExceptionCallback(DWORD dwType, LONG lUserID, LONG lHandle, void
                                           *pUser) {
    char tempbuf[256];
    memset(tempbuf, 0, sizeof(tempbuf));
    switch(dwType) {
    case EXCEPTION_RECONNECT: { //预览时重连
            LogW("dvr: %s", tempbuf);
        }
        break;
    default:
        break;
    }
}

void CALLBACK DecCBFun(int nPort,char* pBuf,int nSize,FRAME_INFO * pFrameInfo, void* nReserved1,int nReserved2) {
    long frame_type = pFrameInfo->nType;
    if(frame_type ==T_YV12)
    {
        FrameData data;
        data.SetFrameData(pFrameInfo->nWidth, pFrameInfo->nHeight, frame_type, pBuf, nSize);
        GetMainWindow()->SendCarameSignal(data);
    }
}

void CALLBACK RealDataCallBack (LONG lRealHandle, DWORD dwDataType, BYTE
                                          *pBuffer, DWORD dwBufSize, void *pUser) {
    HWND hwnd = reinterpret_cast<unsigned long>(pUser);

    switch (dwDataType)
    {
    case NET_DVR_SYSHEAD: //系统头
        if (!PlayM4_GetPort(&lPort))
        {
            break;
        }

        //第一次回调的是系统头,将获取的播放库 port 号赋值给全局 port,下次回调数据时即使用此 port 号播放
        if (dwBufSize > 0)
        {
            if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))
                //设置实时流播放模式
            {
                break;
            }
            if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024*1024)) //打开流接口
            {
                break;
            }

            //设置解码回调函数 只解码不显示
            if (!PlayM4_SetDecCallBack(lPort,DecCBFun))
            {
                //PlayM4_GetLastError(lPort);
                break;
            }
              //设置解码回调函数，并显示
//            if (!PlayM4_SetDecCallBackEx(lPort,DecCBFun, NULL, 0))
//            {
//                //PlayM4_GetLastError(lPort);
//                break;
//            }
            if (!PlayM4_Play(lPort, hwnd)) //播放开始
            {
                break;
            }
        }
        break;
    case NET_DVR_STREAMDATA:
        //码流数据
        if (dwBufSize > 0 && lPort != -1)
        {
            if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
            {
                break;
            }
        }
        break;
    default:
        //其他数据
        if (dwBufSize > 0 && lPort != -1)
        {
            if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
            {
                break;
            }
        }
        break;
    }
}

////////////////////////////////////////////////


HkCamera::HkCamera(HWND hwnd) : hwnd_(hwnd)
{

}

HkCamera::~HkCamera()
{

}

bool HkCamera::InitCarema()
{
    if (camera_is_open_) {
        LogI("摄像头已经打开");
        return false;
    }

    if (hwnd_ == 0) {
        LogI("窗口句柄为空");
        return false;
    }

    NET_DVR_Init();

    //设置连接超时时间
    if (!NET_DVR_SetConnectTime(5000, 3)) {
        LogE("dvr: SetConnectTime error %d", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return false;
    }

    //用户注册设备
    NET_DVR_DEVICEINFO_V30 struDeviceInfo = {0};
    auto hk = Configs::GetHkConfig();
    LONG user_id = NET_DVR_Login_V30((char*)hk->ip.c_str(), hk->port, (char*)hk->user.c_str(), (char*)hk->passwrod.c_str(), &struDeviceInfo);\
    if (user_id < 0) {
        LogE("dvr: login error %d", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return false;
    }

    //注册异常消息回调函数
    if (!NET_DVR_SetExceptionCallBack_V30(0, nullptr, DvrExceptionCallback, nullptr)) {
        LogE("dvr: SetExceptionCallBack error %d", NET_DVR_GetLastError());
        //注销用户
        NET_DVR_Logout(user_id);
        //释放SDK资源
        NET_DVR_Cleanup();
        return false;
    }

    //开启实时预览
    NET_DVR_PREVIEWINFO pre_info = {0};
    pre_info.hPlayWnd = NULL;
    pre_info.lChannel = 1;
    pre_info.dwStreamType = 0;
    pre_info.dwLinkMode = 0;
    pre_info.bBlocked = 0;//请求码流过程是否阻塞
    LONG lRealPlayHandle = NET_DVR_RealPlay_V40(user_id, &pre_info, RealDataCallBack, (void*)hwnd_);
    if (lRealPlayHandle < 0) {
        LogE("dvr: NET_DVR_RealPlay_V40 error %d", NET_DVR_GetLastError());
        //注销用户
        NET_DVR_Logout(user_id);
        //释放SDK资源
        NET_DVR_Cleanup();
        return false;
    }

    user_id_ = user_id;
    real_play_handle_ = lRealPlayHandle;
    camera_is_open_ = true;
    LogI("摄像头成功开启");
    return true;
}

void HkCamera::UnInitCarema()
{
    if (camera_is_open_) {
        //关闭预览
        NET_DVR_StopRealPlay(real_play_handle_);
        //注销用户
        NET_DVR_Logout(user_id_);
        //释放SDK资源
        NET_DVR_Cleanup();
        camera_is_open_ = false;
    }
}

void HkCamera::set_hwnd(HWND hwnd)
{
    hwnd_ = hwnd;
}
