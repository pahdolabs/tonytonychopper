#include <Godot.hpp>
#include <Reference.hpp>

using namespace godot;

#include "chopper.h"

void Chopper::Chopper()
{
    //! Enable logging
    cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);
}

void Chopper::_init() {}

void Chopper::connect(const String &host, int port)
{

    //std::string stg_host = host.c_str();
    std::string stg_host = std::string(host.utf8().get_data());

    client.connect(stg_host, port, [this](const std::string &host, std::size_t port, cpp_redis::client::connect_state status){
        if (status == cpp_redis::client::connect_state::dropped)
        {
            //std::cout << "client disconnected from " << host << ":" << port << std::endl;
            ERR_EXPLAIN("client disconnected!");
        }
        else if (status == cpp_redis::client::connect_state::ok)
        {
            emit_signal("callback_Chopper_connect");
        }
    });
}

void Chopper::multi()
{
    client.multi();
}

void Chopper::watch(const String &key) {
    client.watch(std::string(key.utf8().get_data()));
}

Array Chopper::exec() {
    Array ret;
    client.exec([this, &ret](cpp_redis::reply &reply) {
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.exec error!");
        }
        ret = redis_reply_to_array(reply);
    });
    client.sync_commit();
    return ret;
}

void Chopper::set(const String &key, const String &value)
{

    std::string stg_key = std::string(key.utf8().get_data());
    std::string stg_value = std::string(value.utf8().get_data());

    client.set(stg_key, stg_value, [](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.set error!");
        }
    });
    client.sync_commit();
}

String Chopper::get(const String &key)
{

    String value;
    std::string stg_key = std::string(key.utf8().get_data());

    client.get(stg_key, [&value](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.get error!");
        }
        else
        {
            if (reply.is_string())
            {
                std::string s = reply.as_string();
                const char *stg = &s[0];
                value = stg;
            }
        }
    });
    client.sync_commit();
    return value;
}

int Chopper::incrby(const String &key, int incr) {
    int res;
    std::string stg_key = std::string(key.utf8().get_data());

    client.incrby(stg_key, incr, [&res](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hdel error!");
        }
        if (reply.is_integer())
            {
                res = reply.as_integer();
            }
    });

    client.sync_commit();
    return res;
}

void Chopper::del(const String &key)
{

    std::string stg_key = std::string(key.utf8().get_data());

    client.del(stg_key, [](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.del error!");
        }
    });
    client.sync_commit();
}

int Chopper::exists(const Array &keys)
{

    int res;
    int max = keys.size();
    std::vector<std::string> pairs_vector(max);

    for (int i = 0; i < max; i++)
    {
        std::string s_key = std::string(String(keys[i]).utf8().get_data());
        pairs_vector[i] = s_key;
    }

    client.exists(pairs_vector, [&res](cpp_redis::reply &reply){
            if (reply.is_error())
            {
                ERR_EXPLAIN("client.exists error!");
            }
            else
            {
                if (reply.is_integer())
                {
                    res = reply.as_integer();
                }
            }
        });
    client.sync_commit();
    return res;
}

Array Chopper::keys(const String &pattern) {
    Array keys;
    std::string stg_pattern = std::string(pattern.utf8().get_data());

    client.keys(stg_pattern, [this, &keys](cpp_redis:: reply & reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.keys error!");
        }
        else
        {
            if (reply.is_array())
            {
                keys = redis_reply_to_array(reply);
            }
        }
    })

    client.sync_commit();
    return keys;
}

String Chopper::hget(const String &key, const String &field)
{

    String value;
    std::string stg_key = std::string(key.utf8().get_data());
    std::string stg_field = std::string(field.utf8().get_data());

    client.hget(stg_key, stg_field, [&value](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hget error!");
        }
        else
        {
            if (reply.is_string())
            {
                std::string s = reply.as_string();
                const char *stg = &s[0];
                value = stg;
            }
        }
    });
    client.sync_commit();
    return value;
}

String Chopper::hset(const String &key, const String &field, const String &value)
{
    std::string stg_key = std::string(key.utf8().get_data());
    std::string stg_field = std::string(field.utf8().get_data());
    std::string stg_value = std::String(value.utf8().get_data());

    client.hset(stg_key, stg_field, stg_value, [](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hset error!");
        }
    });
    client.sync_commit();
}

int Chopper::hincrby(const String &key, const String &field, int incr) {
    int res;
    std::string stg_key = std::string(key.utf8().get_data());
    std::string stg_field = std::string(field.utf8().get_data());

    client.hincrby(stg_key, stg_field, incr, [&res](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hdel error!");
        }
        if (reply.is_integer())
            {
                res = reply.as_integer();
            }
    });

    client.sync_commit();
    return res;
}

String Chopper::hdel(const String &key, const String &field)
{
    std::string stg_key = std::string(key.utf8().get_data());
    std::string stg_field = std::string(field.utf8().get_data());

    client.hdel(stg_key, stg_field, [](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hdel error!");
        }
    });
    client.sync_commit();
}

void Chopper::hmset(const String &key, const Dictionary &pairs){

    int reply_ok;
    int max = pairs.size();
    std::string stg_key = std::string(key.utf8().get_data());
    Array keys_pairs = pairs.keys();
    std::vector<std::pair<std::string, std::string> > pairs_vector(max);

    for (int i = 0; i < max; i++)
    {
        std::string s_key = std::string(String(keys_pairs[i]).utf8().get_data());
        std::string s_value = std::string(String(pairs[keys_pairs[i]]).utf8().get_data());

        pairs_vector[i] = std::make_pair(s_key, s_value);
    }
    client.hmset(stg_key, pairs_vector, [](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hmset error!");
        }
    });
    client.sync_commit();
}

Array Chopper::hmget(const String &key, const Array &hkeys)
{

    Array res;
    int max = hkeys.size();
    std::string stg_key = std::string(key.utf8().get_data());
    std::vector<std::string> keys_vector(max);

    for (int i = 0; i < max; i++)
    {
        std::string s_key = std::string(String(hkeys[i]).utf8().get_data());
        keys_vector[i] = s_key;
    }

    client.exists(stg_key, keys_vector, [this, &res](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hmget error!");
        }
        else
        {
            if (reply.is_array())
            {
                res = redis_reply_to_array(reply);
            }
        }
    });
    client.sync_commit();
    return res;
})

Dictionary Chopper::hgetall(const String &key)
{

    Dictionary dict;
    std::string stg_key = std::string(key.utf8().get_data());

    client.hgetall(stg_key, [this, &dict](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hgetall error!");
        }
        else
        {
            if (reply.is_array())
            {
                std::vector<std::pair<std::string, std::string> > data = redis_reply_to_pairs_vector(reply);
                int max = data.size();
                for (int i = 0; i < max; i++)
                {
                    std::string s1 = data[i].first;
                    std::string s2 = data[i].second;
                    const char *stg1 = &s1[0];
                    const char *stg2 = &s2[0];
                    dict[stg1] = stg2;
                }
            }
        }
    });
    client.sync_commit();
    return dict;
}

Array Chopper::hkeys(const String &key) {
    Array keys;
    std::string stg_key = std::string(key.utf8().get_data());

    client.hkeys(stg_key, [this, &keys](cpp_redis:: reply & reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.keys error!");
        }
        else
        {
            if (reply.is_array())
            {
                keys = redis_reply_to_array(reply);
            }
        }
    })

    client.sync_commit();
    return keys;
}

Array Chopper::hvals(const String &key) {
    Array vals;
    std::string stg_key = std::string(key.utf8().get_data());

    client.hvals(stg_key, [this, &vals](cpp_redis:: reply & reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.keys error!");
        }
        else
        {
            if (reply.is_array())
            {
                vals = redis_reply_to_array(reply);
            }
        }
    })

    client.sync_commit();
    return vals;
}

int hlen(const String& key)
{

    int res;
    std::string stg_key = std::string(key.utf8().get_data());

    client.hlen(stg_key, [&res](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hlen error!");
        }
        else
        {
            if (reply.is_integer())
            {
                res = reply.as_integer();
            }
        }
    });
    client.sync_commit();
    return res;
}

bool hexists(const String& key, const String& field)
{

    bool ret;
    std::string stg_key = std::string(key.utf8().get_data());
    std::string stg_field = std::string(field.utf8().get_data());

    client.hexists(stg_key, stg_field, [&ret](cpp_redis::reply &reply){
        if (reply.is_error())
        {
            ERR_EXPLAIN("client.hexists error!");
        }
        else
        {
            if (reply.is_integer())
            {
                ret = reply.as_integer();
            }
        }
    });
    client.sync_commit();
    return ret;
}

Array Chopper::redis_reply_to_array(cpp_redis::reply &reply)
{
    int len = reply.as_array().size();
    Array vals;
    std::vector<cpp_redis::reply> data = reply.as_array();

    for (int i = 0; i < len; i++) {
        if (data[i].is_array()) {
            Array nested = redis_reply_to_array(data[i]);
            vals.append(nested);
        }
        if (data[i].is_string()) {
            std::string s = data[i].as_string();
            const char *stg = &s[0];
            vals.append(stg);
        }
        if (data[i].is_integer()) {
            int i = data[i].as_integer();
            vals.append(i);
        }
        if (data[i].is_null()) {
            Variant v;
            vals.append(v);
        }
    }

    return vals;
}

std::vector<std::pair<std::string, std::string> > Chopper::redis_reply_to_pairs_vector(cpp_redis::reply &reply)
{

    int max = reply.as_array().size();
    std::vector<std::pair<std::string, std::string> > res(max * 0.5);
    std::string key;
    std::string value;

    int i = 0;
    for (int j = 0; j < max; j += 2)
    {
        key = reply.as_array()[j].as_string();
        value = reply.as_array()[j + 1].as_string();
        res[i] = std::make_pair(key, value);
        i++;
    }
    return res;
}

static void Chopper::_register_methods()
{
    register_method("connect", &Chopper::connect);

    register_method("set", &Chopper::set);
    register_method("get", &Chopper::get);
    register_method("incrby", &Chopper::incrby);
    register_method("del", &Chopper::del);
    register_method("exists", &Chopper::exists);
    register_method("keys", &Chopper::keys);

    register_method("hget", &Chopper::hget);
    register_method("hset", &Chopper::hset);
    register_method("hincrby", &Chopper::hincrby);
    register_method("hdel", &Chopper::hdel);
    register_method("hkeys", &Chopper::hkeys);
    register_method("hvals", &Chopper::hvals);
    register_method("hlen", &Chopper::hlen);
    register_method("hexists", &Chopper::hexists);
    register_method("hmset", &Chopper::hmset);
    register_method("hmget", &Chopper::hmget);
    register_method("hgetall", &Chopper::hgetall);
}

/** GDNative Initialize **/
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
    godot::Godot::gdnative_init(o);
}

/** GDNative Terminate **/
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
    godot::Godot::gdnative_terminate(o);
}

/** NativeScript Initialize **/
extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
    godot::Godot::nativescript_init(handle);

    godot::register_class<Chopper>();
}
