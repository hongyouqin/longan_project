#ifndef CONFIGS_H
#define CONFIGS_H

#include <string>
#include <memory>

namespace cpptoml {
    class table;
}

//海康摄像头配置
struct HkParamsConfig {
    std::string ip;

    int port;

    std::string user;

    std::string passwrod;
};

//数据中心配置
struct DataCenterConfig {
    std::string addr;
};

//系统配置
struct SystemConfig {
    float face_score;

    int show_camera;

    int stranger_push_time;

    int is_push_stranger;

    int employee_push_time;

    int frame_frequency;
};

//redis服务器配置
struct RedisConfig {
    std::string ip;

    int port;

    std::string auth;
};

class Configs
{
public:
    Configs();

    static bool LoadConfig();

    static HkParamsConfig* GetHkConfig();

    static DataCenterConfig* GetDataCenterConfig();

    static SystemConfig* GetSystemConfig();

    static RedisConfig* GetRedisConfig();
private:
    static std::shared_ptr<cpptoml::table> tables_;

    static HkParamsConfig hk_config_;

    static DataCenterConfig dc_config_;

    static SystemConfig system_config_;

    static RedisConfig redis_config_;
};

#endif // CONFIGS_H
