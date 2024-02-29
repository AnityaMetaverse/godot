#include "als_pack.h"

#include <limits.h>

void ALSPack::pack_from_vector2_array(const PoolVector2Array& array)
{
    _data = PoolByteArray();

    if (array.size() == 0)
    {
        return;
    }

    _data.resize(array.size() * 2);
    uint8_t* data = _data.write().ptr();
    const Vector2* vdata = array.read().ptr();

    for (int  index = 0; index < array.size(); index++)
    {
        float x = (vdata + index)->x;
        short value = short(x * float(SHRT_MAX));
        *(data + index * _sample_bytes_size) = uint8_t(value & 0xFF);
        *((data + index * _sample_bytes_size) + 1) = uint8_t((value >> 8));
    }
}

PoolVector2Array ALSPack::unpack_to_vector2_array() const
{
    PoolVector2Array voice;
    if (_data.size() == 0)
    {
        return voice;
    }

    voice.resize(_data.size() / 2);
    const uint8_t* data = _data.read().ptr();
    Vector2* vdata = voice.write().ptr();

    for (int  index = 0; index < voice.size(); index++)
    {
        uint8_t s0 = *(data + index * _sample_bytes_size);
        uint8_t s1 = *((data + index * _sample_bytes_size) + 1);

        short value = s0 | (s1 << 8);

        float f = float(value) / float(SHRT_MAX);
        *(vdata + index) = Vector2(f, f);
    }

    return voice;
}



int ALSPack::assemble_pack(const PoolByteArray& array)
{
    _data = array;
    return 0;
}

void ALSPack::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("pack_from_vector2_array", "array"), &ALSPack::pack_from_vector2_array);
    ClassDB::bind_method(D_METHOD("unpack_to_vector2_array"), &ALSPack::unpack_to_vector2_array);
    ClassDB::bind_method(D_METHOD("data"), &ALSPack::data);
}