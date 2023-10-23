#include "livestream_data.h"


void LivestreamData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_image_data", "data"), &LivestreamData::set_image_data);
    ClassDB::bind_method(D_METHOD("get_image_data"), &LivestreamData::get_image_data);

    ClassDB::bind_method(D_METHOD("set_audio_data", "data"), &LivestreamData::set_audio_data);
    ClassDB::bind_method(D_METHOD("get_audio_data"), &LivestreamData::get_audio_data);

    ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "image_data"), "set_image_data", "get_image_data");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "audio_data"), "set_audio_data", "get_audio_data");
    // ADD_PROPERTY("audio_data", &LivestreamData::set_audio_data, &LivestreamData::get_audio_data);
}