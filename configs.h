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

class Configs
{
public:
    Configs();

    static bool LoadConfig();

    static HkParamsConfig* GetHkConfig();

    static DataCenterConfig* GetDataCenterConfig();
private:
    static std::shared_ptr<cpptoml::table> tables_;

    static HkParamsConfig hk_config_;

    static DataCenterConfig dc_config_;
};

#endif // CONFIGS_H
