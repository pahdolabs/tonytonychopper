#ifndef GD_CHOPPER
#define GD_CHOPPER

#include <Godot.hpp>
#include <Reference.hpp>
#include <cpp_redis>
#include <iostream>
#include <memory>

namespace godot {
    class Chopper : public Reference {
        GODOT_CLASS(Chopper, Reference);

    public:
        Chopper();

        void _init();
        static void _register_methods();

        void connect(const String& host="127.0.0.1", int port=6379);
        void multi();
        void watch(const Array& keys);
        Array exec();

        void set(const String& key, const String& value);
        String get(const String& key);
        int incrby(const String& key, int incr);
        void del(const Array& keys);
        int exists(const Array& keys);
        Array keys(const String& pattern);
        
        String hget(const String& key, const String& field);
        void hset(const String& key, const String& field, const String& value);
        int hincrby(const String& key, const String& field, int incr);
        void hdel(const String& key, const Array& fields);
        Array hkeys(const String& key);
        Array hvals(const String& key);
        int hlen(const String& key);
        bool hexists(const String& key, const String& field);
        void hmset(const String& key, const Dictionary& pairs);
        Array hmget(const String& key, const Array& hkeys);
        Dictionary hgetall(const String& key);

        // Transaction support
        void tset(const String& key, const String& value);
        void tget(const String& key);
        void tincrby(const String& key, int incr);
        void tdel(const Array& keys);
        void texists(const Array& keys);
        void tkeys(const String& pattern);
        
        void thget(const String& key, const String& field);
        void thset(const String& key, const String& field, const String& value);
        void thincrby(const String& key, const String& field, int incr);
        void thdel(const String& key, const Array& fields);
        void thkeys(const String& key);
        void thvals(const String& key);
        void thlen(const String& key);
        void thexists(const String& key, const String& field);
        void thmset(const String& key, const Dictionary& pairs);
        void thmget(const String& key, const Array& hkeys);
        void thgetall(const String& key);

    private:
        cpp_redis::client client;
        std::vector<std::pair<std::string, std::string>> redis_reply_to_pairs_vector(cpp_redis::reply& records);
        Array redis_reply_to_array(cpp_redis::reply& records);
        std::vector<std::string> godot_array_to_vector(const Array& arr);
    };
}

#endif
