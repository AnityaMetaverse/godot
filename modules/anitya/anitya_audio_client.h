#ifndef ANITYA_AUDIO_CLIENT_H
#define ANITYA_AUDIO_CLIENT_H

#include "scene/main/node.h"

#include <unordered_map>

class JoinChannelParameter: public Reference 
{
    GDCLASS(JoinChannelParameter, Reference)
    private:
        String channel_id;
        String token;
    
    protected:
        static void _bind_methods();
    
    public:
        String get_channel_id() const { return channel_id; }
        void set_channel_id(const String& p_channel_id) { channel_id = p_channel_id; }
        String get_token() const { return token; }
        void set_token(const String& p_token) { token = p_token; }
};

class AudioClientConfig: public Reference
{
    GDCLASS(AudioClientConfig, Reference);

    private:
        String ip;
        String port;
        String username;
        String app_id;
        
    
    protected:
        static void _bind_methods();

    public:
        String get_app_id() const { return app_id; }
        void set_app_id(const String& p_app_id) { app_id = p_app_id; }

        void set_ip(const String& p_ip) { ip = p_ip; }
        String get_ip() const { return ip; }

        void set_username(const String& p_username) { username = p_username; }
        String get_username() const { return username; }

        void set_port(const String& p_port) { port = p_port; }
        String get_port() const { return port; }
};

class AudioClient: public Node
{
    GDCLASS(AudioClient, Node);

    private:

        Ref<AudioClientConfig> config;
        PoolByteArray voice;

    protected:
        
        static void _bind_methods();
    public:
        virtual void init(Ref<AudioClientConfig> p_config) { config = p_config; }
        Ref<AudioClientConfig> get_config() { return config; }
        virtual void join_channel(Ref<JoinChannelParameter> param) {}
        virtual void upadte_position(const Vector3& pos) {}
        virtual void set_self_mute(bool value) {}
        virtual void set_mute_user(int user_id, bool value) {}
        void send_voice(PoolVector2Array pcm);
        PoolByteArray get_voice() const { return voice; }
};

class RemoteAudioClientConfig: public Reference
{
    GDCLASS(RemoteAudioClientConfig, Reference);

    private:
        int id;
        String username;
    
    protected:
        static void _bind_methods();

    public:
        void set_id(int p_id) { id = p_id; }
        int get_id() const { return id; }

        void set_username(const String& p_username) { username = p_username; }
        String get_username() const { return username; }
};

class RemoteAudioClient: public Node
{
    GDCLASS(RemoteAudioClient, Node);

    private:
        Ref<RemoteAudioClientConfig> config;

    protected:
        static void _bind_methods();
    public:
        Ref<RemoteAudioClientConfig> get_config() { return config; }
};

#endif