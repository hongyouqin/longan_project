#include "push_redis.h"
#include <hiredis/hiredis.h>
#include "logger.h"

PushRedis::PushRedis()
{

}

bool PushRedis::Push(const std::string& user_name, const std::string& camera_id, const std::string& pic_url, unsigned int time)
{
    redisContext* conn = redisConnect("192.168.79.206", 6379);
    if (nullptr == conn || conn->err) {
        LogE("redis server connection error %s", conn->errstr);
        return false;
    }
    redisReply* reply= (redisReply*)redisCommand(conn, "AUTH %s", "123456");
    if (nullptr == reply || reply->type == REDIS_REPLY_ERROR) {
        /* Authentication failed */
        LogE("Authentication failed");
        redisFree(conn);
        return false;
    }
    freeReplyObject(reply);

    reply = (redisReply*)redisCommand(conn, "publish visit_data_queue {\"username\":\"%s\",\"camera_uid\":\"%s\",\"pic_url\":\"%s\",\"time\":%u}", user_name.c_str(), camera_id.c_str(), pic_url.c_str(), time);
    if (nullptr == reply || reply->type == REDIS_REPLY_ERROR) {
        LogE("publish failed");
        redisFree(conn);
        return false;
    }
    freeReplyObject(reply);

    redisFree(conn);
    return true;
}
