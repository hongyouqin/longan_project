#ifndef PUSH_REDIS_H
#define PUSH_REDIS_H

#include <string>


class PushRedis
{
public:
    PushRedis();

    bool Push(const std::string& user_name, const std::string& camera_id, const std::string& pic_url, unsigned int time);
};

#endif // PUSH_REDIS_H
