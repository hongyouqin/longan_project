#include "configs.h"
#include "cpptoml/cpptoml.h"
#include "logger.h"

namespace {
    const std::string kTomlFile = "config/config.toml";
}

std::shared_ptr<cpptoml::table> Configs::tables_ = nullptr;

HkParamsConfig Configs::hk_config_;

DataCenterConfig Configs::dc_config_;

SystemConfig Configs::system_config_;

RedisConfig Configs::redis_config_;

Configs::Configs()
{

}

bool Configs::LoadConfig()
{
    try
    {
        tables_ = cpptoml::parse_file(kTomlFile);
        //read hk
        hk_config_.ip = tables_->get_qualified_as<std::string>("hk.ip").value_or("");
        hk_config_.port = tables_->get_qualified_as<int>("hk.port").value_or(0);
        hk_config_.user = tables_->get_qualified_as<std::string>("hk.user").value_or("");
        hk_config_.passwrod = tables_->get_qualified_as<std::string>("hk.password").value_or("");

        //read dc
        dc_config_.addr = tables_->get_qualified_as<std::string>("data_center.addr").value_or("");

        //read redis
        redis_config_.ip = tables_->get_qualified_as<std::string>("redis.ip").value_or("");
        redis_config_.port = tables_->get_qualified_as<int>("redis.port").value_or(0);
        redis_config_.auth = tables_->get_qualified_as<std::string>("redis.auth").value_or("");

        //read system
        system_config_.face_score = (float)tables_->get_qualified_as<double>("system.face_score").value_or(0.0);
        system_config_.show_camera = tables_->get_qualified_as<int>("system.show_camera").value_or(0);
        system_config_.face_process_num = tables_->get_qualified_as<int>("system.face_process_num").value_or(4);
        system_config_.stranger_push_time = tables_->get_qualified_as<int>("system.stranger_push_time").value_or(500);
        system_config_.is_push_stranger = tables_->get_qualified_as<int>("system.is_push_stranger").value_or(0);
        system_config_.employee_push_time = tables_->get_qualified_as<int>("system.employee_push_time").value_or(500);
    }
    catch (const cpptoml::parse_exception &e)
    {
        LogE("Failed to parse %s : %s", kTomlFile.c_str(), e.what());
        return false;
    }

    return true;
}

HkParamsConfig *Configs::GetHkConfig()
{
    return &hk_config_;
}

DataCenterConfig *Configs::GetDataCenterConfig(){
    return &dc_config_;
}

SystemConfig *Configs::GetSystemConfig()
{
    return &system_config_;
}

RedisConfig *Configs::GetRedisConfig()
{
    return &redis_config_;
}
