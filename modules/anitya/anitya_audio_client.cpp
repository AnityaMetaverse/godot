#include "anitya_audio_client.h"


void AudioClientConfig::_bind_methods()
{
     // ADD_SIGNAL(MethodInfo("pcm_received", PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "voice")));
     // ADD_PROPERTY(PropertyInfo(Variant::INT, "operation", PROPERTY_HINT_ENUM, "Union,Intersection,Subtraction"), "set_operation", "get_operation");
     // ADD_PROPERTY(PropertyInfo(Variant::INT, "operation", PROPERTY_HINT_ENUM, "Union,Intersection,Subtraction"), "set_operation", "get_operation");
     ClassDB::bind_method(D_METHOD("set_ip", "ip"), &AudioClientConfig::set_ip);
     ClassDB::bind_method(D_METHOD("get_ip"), &AudioClientConfig::get_ip);
     ADD_PROPERTY(PropertyInfo(Variant::STRING, "ip"), "set_ip", "get_ip");

     ClassDB::bind_method(D_METHOD("set_username", "username"), &AudioClientConfig::set_username);
     ClassDB::bind_method(D_METHOD("get_username"), &AudioClientConfig::get_username);
     ADD_PROPERTY(PropertyInfo(Variant::STRING, "username"), "set_username", "get_username");

     ClassDB::bind_method(D_METHOD("set_port", "port"), &AudioClientConfig::set_port);
     ClassDB::bind_method(D_METHOD("get_port"), &AudioClientConfig::get_port);
     ADD_PROPERTY(PropertyInfo(Variant::STRING, "port"), "set_port", "get_port");

     ClassDB::bind_method(D_METHOD("set_app_id", "app_id"), &AudioClientConfig::set_app_id);
     ClassDB::bind_method(D_METHOD("get_app_id"), &AudioClientConfig::get_app_id);
     ADD_PROPERTY(PropertyInfo(Variant::STRING, "app_id"), "set_app_id", "get_app_id");
}


void AudioClient::_bind_methods()
{
     // ADD_SIGNAL(MethodInfo("pcm_received", PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "voice")));
     ClassDB::bind_method(D_METHOD("init", "config"), &AudioClient::init);
}

void AudioClient::send_voice(PoolVector2Array pcm)
{
     int current_size = voice.size();
     voice.resize(voice.size() + pcm.size() * 4);
     for (int index = 0; index < pcm.size(); index++)
     {
          float* v = (float*)(voice.read().ptr() + (current_size + (index * 4)));
          *v = pcm[index].x;
     }
}

void JoinChannelParameter::_bind_methods()
{
     ClassDB::bind_method(D_METHOD("set_channel_id", "channel_id"), &JoinChannelParameter::set_channel_id);
     ClassDB::bind_method(D_METHOD("get_channel_id"), &JoinChannelParameter::get_channel_id);
     ADD_PROPERTY(PropertyInfo(Variant::STRING, "channel_id"), "set_channel_id", "get_channel_id");

     ClassDB::bind_method(D_METHOD("set_token", "token"), &JoinChannelParameter::set_token);
     ClassDB::bind_method(D_METHOD("get_token"), &JoinChannelParameter::get_token);
     ADD_PROPERTY(PropertyInfo(Variant::STRING, "token"), "set_token", "get_token");
}

void AudioClientUpdatePosition::_bind_methods()
{
     ClassDB::bind_method(D_METHOD("_set_targets", "targets"), &AudioClientUpdatePosition::_set_targets);
     ClassDB::bind_method(D_METHOD("_get_targets"), &AudioClientUpdatePosition::_get_targets);
     ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR3_ARRAY, "targets"), "_set_targets", "_get_targets");
     ClassDB::bind_method(D_METHOD("push_target", "target"), &AudioClientUpdatePosition::push_target);

     ClassDB::bind_method(D_METHOD("_set_ids", "ids"), &AudioClientUpdatePosition::_set_ids);
     ClassDB::bind_method(D_METHOD("_get_ids"), &AudioClientUpdatePosition::_get_ids);
     ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "ids"), "_set_ids", "_get_ids");
     ClassDB::bind_method(D_METHOD("push_id", "id"), &AudioClientUpdatePosition::push_id);

     ClassDB::bind_method(D_METHOD("_set_origin", "origin"), &AudioClientUpdatePosition::_set_origin);
     ClassDB::bind_method(D_METHOD("_get_origin"), &AudioClientUpdatePosition::_get_origin);
     ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "origin"), "_set_origin", "_get_origin");
     

}

void RemoteAudioClient::_bind_methods()
{
     ClassDB::bind_method(D_METHOD("get_config"), &RemoteAudioClient::get_config);
     ADD_SIGNAL(MethodInfo("pcm_received", PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "voice")));
}


void RemoteAudioClientConfig::_bind_methods()
{
     ClassDB::bind_method(D_METHOD("get_username"), &RemoteAudioClientConfig::get_username);
     ClassDB::bind_method(D_METHOD("get_id"), &RemoteAudioClientConfig::get_id);
}