#include "als_client.h"


#include "core/class_db.h"
#include "als.h"

#include "modules/webrtc/webrtc_data_channel.h"

void ALSClient::update()
{

    if (_channel)
    {
        _channel->poll();
        _send_packages();

        // if (_channel->get_ready_state() == WebRTCDataChannel::STATE_OPEN)
        // {
            int packages = _channel->get_available_packet_count();
            if (packages != 0)
            {
                PoolByteArray buffer;
                Error err = _channel->get_packet_buffer(buffer);
                const uint8_t* p = buffer.read().ptr();
                if ((*p & ALS_MESSAGE_TYPE_COMMAND) != 0)
                {
                    switch ((*p) & ~(1 << 7))
                    {
                        case ALS_COMMAND_ASSIGN_ID:
                            print_line("ALS_COMMAND_ASSIGN_ID");
                            _user_id = *(p + 1);
                            _send_user_created();
                            _send_get_user_list();
                            break;
                        case ALS_COMMAND_USER_REGISTER:
                        {
                            print_line("ALS_COMMAND_USER_REGISTER");
                            uint8_t remote_user_id = buffer[1];
                            Ref<ALSUser> ru = memnew(ALSUser);
                            ru->set_user_id(remote_user_id);
                            _users[remote_user_id] = ru;
                            emit_signal("user_added", ru);
                            break;
                        }
                        case ALS_COMMAND_GET_USER_LIST:
                            print_line("ALS_COMMAND_GET_USER_LIST");
                            for (int index = 1; index < buffer.size(); index++)
                            {
                                int8_t uid = buffer[index];
                                if (uid != _user_id)
                                {
                                    Ref<ALSUser> u = memnew(ALSUser);
                                    u->set_user_id(uid);
                                    // _users[buffer[index]] = u;
                                    _users[buffer[index]] = u;
                                    emit_signal("user_added", u);
                                }
                            }
                            break;
                        case ALS_COMMAND_REMOVE_USER:
                        {
                            print_line("ALS_COMMAND_REMOVE_USER");
                            uint8_t user_id = buffer[1];
                            Ref<ALSUser> u = _users.get(user_id, Ref<ALSUser>());
                            if (u.is_valid())
                            {
                                emit_signal("user_removed", u);
                            }
                            _users.erase(user_id);
                            break;
                        }
                    }
                }
                else
                {
                    if (_user_id != INVALID_USER_ID)
                    {
                        PoolByteArray voice;
                        voice.resize(buffer.size() - 1);
                        memcpy(voice.write().ptr(), p + 1, buffer.size() - 1);
                        int8_t uid = buffer[0] & ~(1 << 7);
                        Ref<ALSUser> u = _users.get(uid, Ref<ALSUser>());
                        
                        if (u.is_valid())
                        {
                            // print_line("inserting voice package");
                            print_error(itos(voice.size()));
                            u->add_voice_package(voice);
                        }
                        else
                        {
                            print_line(String("Invalid user. UserId: ") + itos(uid));
                        }
                    }
                }
            }
        // }
    }
}

void ALSClient::_send_user_created()
{
    if (_channel)
    {
        PoolByteArray command;
        command.resize(2);
        command.set(0, ALS_MESSAGE_TYPE_COMMAND | ALS_COMMAND_USER_REGISTER);
        command.set(1, _user_id);

        Error err = _channel->put_packet(command.read().ptr(), command.size());
        if (err != OK)
        {
            print_error(String("Error sending command ALS_COMMAND_USER_REGISTER. Error code: ") + itos(err));
        }
    }
}

void ALSClient::_send_get_user_list()
{
    if (_channel)
    {
        // PoolByteArray command;
        // command.resize(2);
        // command.set(0, ALS_MESSAGE_TYPE_COMMAND | ALS_COMMAND_USER_REGISTER);
        // command.set(1, _user_id);
        uint8_t byte = ALS_MESSAGE_TYPE_COMMAND | ALS_COMMAND_GET_USER_LIST;
        Error err = _channel->put_packet(&byte, 1);
        if (err != OK)
        {
            print_error(String("Error sending command ALS_COMMAND_USER_REGISTER. Error code: ") + itos(err));
        }
    }
}

void ALSClient::_send_packages()
{

    // print_line("Sending packages");
    while(!_packages.empty())
    {
        // print_line("Sending packages within loop");
        Ref<ALSPack> pack = _packages.pop_front();
        Error err = _channel->put_packet(pack->data().read().ptr(), pack->data().size());
        if (err != OK)
        {
            print_error("Cannot send package");
            return;
        }

        
    }
}

void ALSClient::push_voice(const PoolByteArray& array)
{

}

void ALSClient::clean()
{
    Array us = _users.keys();
    for (int index = 0; index < us.size(); index++)
    {
        Ref<ALSUser> u = _users[us[index]];
        emit_signal("user_removed", u);
        _users.erase(us[index]);
    }
}

void ALSClient::push_voice_from_vector2(const PoolVector2Array& array)
{

    if (array.size() == 0)
    {
        return;
    }

    PoolByteArray carray = _ALS::get_singleton()->compress_from_vector2_array(array);
    int n = carray.size() / 1024;
    for (int index = 0; index < carray.size(); index += 1024)
    {
        PoolByteArray sub = carray.subarray(index,  CLAMP(index + 1023, 0, carray.size() - 1));
        PoolByteArray full_array;
        full_array.resize(sub.size() + 1);
        uint8_t* data = full_array.write().ptr();
        *data = ALS_MESSAGE_TYPE_VOICE | static_cast<uint8_t>(get_user_id());
        memcpy(data + 1, sub.read().ptr(), sub.size());
        Ref<ALSPack> pack = memnew(ALSPack);
        pack->assign_data(full_array);
        _packages.push_back(pack);
    }
}

void ALSClient::set_peer(Object* channel) { _channel = cast_to<WebRTCDataChannel>(channel); }

void ALSClient::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("update"), &ALSClient::update);
    ClassDB::bind_method(D_METHOD("push_voice", "voice"), &ALSClient::push_voice);
    ClassDB::bind_method(D_METHOD("push_voice_from_vector2", "voice"), &ALSClient::push_voice_from_vector2);
    ClassDB::bind_method(D_METHOD("set_peer", "peer"), &ALSClient::set_peer);
    ClassDB::bind_method(D_METHOD("clean"), &ALSClient::clean);

    ADD_SIGNAL(MethodInfo("data_received", PropertyInfo(Variant::POOL_BYTE_ARRAY, "data")));
    ADD_SIGNAL(MethodInfo("user_added", PropertyInfo(Variant::OBJECT, "user")));
    ADD_SIGNAL(MethodInfo("user_removed", PropertyInfo(Variant::OBJECT, "user")));
}

ALSClient::ALSClient()
{
    _user_id = INVALID_USER_ID;
}