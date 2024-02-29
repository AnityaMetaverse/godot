#include "als.h"

PoolByteArray _ALS::to_audio_network_array(const PoolVector2Array& p_array, int sample_bytes_size) const
{
    return PoolByteArray();
}

PoolVector2Array _ALS::to_vector2_array(const PoolByteArray& p_array, int sample_bytes_size) const
{
    return PoolVector2Array();
}

void _ALS::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("to_audio_network_array", "vector_array", "sample_bytes_size"), &_ALS::to_audio_network_array);
    ClassDB::bind_method(D_METHOD("to_vector2_array", "array", "sample_bytes_size"), &_ALS::to_vector2_array);
}


_ALS* _ALS::_singleton = nullptr;

_ALS::_ALS()
{
    _singleton = this;
}
