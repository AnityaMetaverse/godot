#include "als.h"


PoolByteArray _ALS::compress_from_vector2_array(const PoolVector2Array& array)
{
    PoolByteArray pvoice = PoolByteArray();

    if (array.size() == 0)
    {
        return pvoice;
    }

    pvoice.resize(array.size() * 2);
    uint8_t* data = pvoice.write().ptr();
    const Vector2* vdata = array.read().ptr();


    _compress_from_vector2_array(data, (float*)vdata, array.size());

    return pvoice;
}

PoolVector2Array _ALS::uncompress_to_vector2_array(const PoolByteArray& stream)
{
    PoolVector2Array voice;
    if (stream.size() == 0)
    {
        return voice;
    }

    voice.resize(stream.size() / 2);
    const uint8_t* data = stream.read().ptr();
    Vector2* vdata = voice.write().ptr();

    _uncompress_to_vector2_array((float*)vdata, data, voice.size());

    return voice;
}

void _ALS::_compress_from_vector2_array(uint8_t* destiny, const float* source, int samount, bool surround)
{
    for (int  index = 0; index < samount; index++)
    {
        float x = *(source + index * 2);
        short value = short(x * float(SHRT_MAX));
        *(destiny + index * 2) = uint8_t(value & 0xFF);
        *((destiny + index * 2) + 1) = uint8_t((value >> 8));
    }
}

void _ALS::_uncompress_to_vector2_array(float* destiny, const uint8_t* source, int damount)
{
    for (int  index = 0; index < damount; index++)
    {
        uint8_t s0 = *(source + index * 2);
        uint8_t s1 = *((source + index * 2) + 1);

        short value = s0 | (s1 << 8);

        float f = float(value) / float(SHRT_MAX);
        Vector2 v = Vector2(f, f);
        memcpy(destiny + index * 2, &v, sizeof(Vector2));
    }
}

// int _ALS::assemble_compress(const PoolByteArray& array)
// {
//     return 0;
// }

void _ALS::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("compress_from_vector2_array", "vector_array"), &_ALS::compress_from_vector2_array);
    ClassDB::bind_method(D_METHOD("uncompress_to_vector2_array", "array"), &_ALS::uncompress_to_vector2_array);
    // ClassDB::bind_method(D_METHOD("assemble_compress", "array"), &_ALS::assemble_compress);
}


_ALS* _ALS::_singleton = nullptr;

_ALS::_ALS()
{
    _singleton = this;
}
