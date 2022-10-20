#ifndef AGORA_H
#define AGORA_H

#include "core/object.h"

#include "AgoraBase.h"
#include "IAgoraRtcEngine.h"
#include "IAgoraMediaEngine.h"
#include "anitya_audio_client.h"

using namespace std;
using namespace agora;
using namespace agora::media;
using namespace agora::rtc;

class AgoraClient;

class AgoraRTCObserver: public agora::rtc::IPacketObserver
{
    public:
        AgoraClient* client;
        virtual bool onSendAudioPacket(Packet& packet) override;
        
        virtual bool onSendVideoPacket(Packet& packet) override;
        
        virtual bool onReceiveAudioPacket(Packet& packet) override;
        
        virtual bool onReceiveVideoPacket(Packet& packet) override;
};


class RTCEventHandler: public IRtcEngineEventHandler{

public:
    AgoraClient* client;
    RTCEventHandler() {};
    ~RTCEventHandler() {};
    virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
    virtual void onLeaveChannel(const RtcStats& stat);
    virtual void onUserJoined(uid_t uid, int elapsed) {};
    virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) {};
    virtual void onConnectionStateChanged(CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason);
    virtual void onError(int err, const char *msg);
    virtual void onLocalUserRegistered(uid_t uid, const char* userAccount);
    virtual void onStreamMessage(uid_t userId, int streamId, const char* data, size_t length, uint64_t sentTs) override;
    virtual void onUserInfoUpdated(uid_t uid, const agora::rtc::UserInfo& info) override;
    
    bool joined_to_channel = false;
    CONNECTION_STATE_TYPE current_state = CONNECTION_STATE_DISCONNECTED;
};



class AgoraClient : public AudioClient {
    GDCLASS(AgoraClient, AudioClient);

    private:
        String username;
        String app_id;
        const int package_per_loop = 5;
        const int package_size = 1024;
        uid_t user_id;
        PoolByteArray send_data;
        void process();
        AgoraRTCObserver packet_observer;
        const Vector3 up = Vector3(0, 1, 0);
        const Vector3 right = Vector3(1, 0, 0);
        const Vector3 forward = Vector3(0, 0, -1);


        std::unordered_map<std::string, uid_t> username_uid;
        std::unordered_map<uid_t, std::string> uid_username;

    protected:
        static void _bind_methods();
        bool initialized;
        IRtcEngine *rtc_engine;
        RTCEventHandler *event_handler;
        void _notification(int what);

    public:
        virtual void init(Ref<AudioClientConfig> p_config) override;
        // void set_app_id(const String& p_app_id);
        // void join_channel(const String& channel_id, uid_t user_id, const String& token);
        virtual void join_channel(Ref<JoinChannelParameter> params) override;
        // Ref<JoinChannelParameter> params
        void set_username(const String& p_name) { username = p_name; }
        // void set_user_id(uid_t p_user_id) { user_id = p_user_id; }
        // void register_account();
        void _no_set_int(int value) {}
        void set_client_id(uid_t p_uid) { user_id = p_uid; }
        uid_t get_client_id() const { return user_id; }
        virtual void set_self_mute(bool value) override;
        virtual void set_mute_user(const String& user_id, bool value) override;
        void on_client_data_received(uid_t uid, const char* data, size_t length);
        virtual void update_position(Ref<AudioClientUpdatePosition> value) override;
        void add_user(const String& username, uid_t user_id);
        void remove_user(uid_t user_id);
        bool is_muted() const;
        void leave_channel();
        void shutdown();
        std::vector<uid_t> get_users();
        AgoraClient();
        ~AgoraClient();
};

#endif
