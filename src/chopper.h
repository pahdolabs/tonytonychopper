#ifndef GD_REDIS
#define GD_REDIS

#include <cpp_redis/cpp_redis>
#include <iostream>
#include <memory>

namespace godot;

class Chopper : public Reference {

    GDCLASS(Chopper,Reference);

protected:
    static void _register_methods();

public:
    Chopper();

    void _init();

    void connect(const String& host="127.0.0.1", int port=6379);
    void multi();
    void watch(const String& key);
    Array exec();

    void set(const String& key, const String& value);
    String get(const String& key);
    int incrby(const String& key);
    void del(const String& key);
    int exists(const Array& keys);
    Array keys(const String& pattern);
    
    String hget(const String& key, const String& field);
    void hset(const String& key, const String& field, const String& value);
    int hincrby(const String& key, const String& field, int incr);
    void hdel(const String& key, const String& field);
    Array hkeys(const String& key);
    Array hvals(const String& key);
    int hlen(const String& key);
    bool hexists(const String& key, const String& field);
    void hmset(const String& key, const Dictionary& pairs);
    Array hmget(const String& key, const Array& hkeys);
    Dictionary hgetall(const String& key);

private:
    cpp_redis::client client;
    std::vector<std::pair<std::string, std::string>> redis_reply_to_pairs_vector(cpp_redis::reply& records);
    Array redis_reply_to_array(cpp_redis::reply& records);
};

#endif