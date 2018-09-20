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

class Configs
{
public:
    Configs();

    static bool LoadConfig();

    static HkParamsConfig* GetHkConfig();
private:
    static std::shared_ptr<cpptoml::table> tables_;

    static HkParamsConfig hk_config_;
};

#endif // CONFIGS_H
