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
                // print_line(String("package size: " + itos(buffer.size())));
                // print_line(String("package first value: " + itos(buffer[0])));
                if ((*p & ALS_MESSAGE_TYPE_COMMAND) != 0)
                {
                    switch ((*p) & ~(1 << 7))
                    {
                    case ALS_COMMAND_ASSIGN_ID:
                        print_line("[ANITYA] ID of user assigned.");
                        _user_id = *(p + 1);
                        break;
                    }
                }
                else
                {
                    if (_user_id != INVALID_USER_ID)
                    {
                        PoolByteArray voice;
                        voice.resize(buffer.size() - 1);
                        memcpy(voice.write().ptr(), p + 1, buffer.size() - 1);
                        emit_signal("data_received", voice);
                    }
                }
            }
        // }
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
    ADD_SIGNAL(MethodInfo("data_received", PropertyInfo(Variant::POOL_BYTE_ARRAY, "data")));
}

ALSClient::ALSClient()
{
    _user_id = INVALID_USER_ID;
}