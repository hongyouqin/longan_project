#include "configs.h"
#include "cpptoml/cpptoml.h"
#include "logger.h"

namespace {
    const std::string kTomlFile = "config/config.toml";
}

std::shared_ptr<cpptoml::table> Configs::tables_ = nullptr;

HkParamsConfig Configs::hk_config_;

DataCenterConfig Configs::dc_config_;

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
