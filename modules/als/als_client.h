#ifndef ALS_ALS_CLIENT_H
#define ALS_ALS_CLIENT_H

#include "core/object.h"

#include "core/os/mutex.h"
#include "als_pack.h"
#include "core/list.h"

#include "als_user.h"

class ALSClient: public Object
{
    GDCLASS(ALSClient, Object)

    private:
        static const uint8_t INVALID_USER_ID = 1 <<7;

        class WebRTCDataChannel* _channel = nullptr;
        Mutex _mutex;
        PoolByteArray _data;
        Array _packages;
        int8_t _user_id;
        // Map<int8_t, ALSUser*> _users;
        Dictionary _users;

        enum ALS_MESSAGE_TYPE
        {
            ALS_MESSAGE_TYPE_VOICE = 0,
            ALS_MESSAGE_TYPE_COMMAND = 1 << 7
        };

        enum ALS_COMMAND
        {
            ALS_COMMAND_ASSIGN_ID = 1,
            ALS_COMMAND_USER_REGISTER = 2,
            ALS_COMMAND_GET_USER_LIST = 3,
            ALS_COMMAND_REMOVE_USER = 4
        };

    private:
        void _send_packages();

    protected:
        static void _bind_methods();
    

    public:
        void set_peer(Object* channel);
        void push_voice(const PoolByteArray& array);
        void push_voice_from_vector2(const PoolVector2Array& array);
        int get_user_id() const { return _user_id; }
        void clean();
    
    private:
        void _send_user_created();
        void _send_get_user_list();

        void update();
        ALSClient();
};

#endif