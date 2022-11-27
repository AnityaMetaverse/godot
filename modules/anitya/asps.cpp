#include "asps.h"

// #include "modules/tracy/profiler.h"

PoolByteArray ASPS::encode(const PoolByteArray& p_data)
{
    
    PoolByteArray result;
    result.resize(p_data.size() + 6);
    uint8_t* p = result.write().ptr();
    *((uint32_t*)(p)) = magic_number;
    *(p + 4) = version;
    *(p + 5) = (uint8_t)45;

    p = result.write().ptr() + 6;


    const uint8_t* origin = p_data.read().ptr();

    for (int index = 0; index < p_data.size(); index++)
    {
        *(p + index) = *(origin + index) + 1;
    }

    return result;
}

PoolByteArray ASPS::decode(const PoolByteArray& p_data)
{
    PoolByteArray result;
    const uint8_t* data = p_data.read().ptr();
    if (*((uint32_t*)data) != magic_number)
    {
        WARN_PRINT("Unrecognised data format");
        return result;
    }

    if (*(data + 4) != version)
    {
        WARN_PRINT(String("Invalid version. Current is: ") + String(itos(version)));
        return result;
    }

    if (*(data + 5) != (uint8_t)45)
    {
        WARN_PRINT("No end of header found");
        return result;
    }

    result.resize(p_data.size() - 6);

    uint8_t* destiny = result.write().ptr();
    const uint8_t* origin = data + 6;

    for (int index = 0; index < result.size(); index++)
    {
        *(destiny + index) = *(origin + index) - 1;
    }

    return result;
}

void ASPS::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("encode", "data"), &ASPS::encode);
    ClassDB::bind_method(D_METHOD("decode", "data"), &ASPS::decode);
    ClassDB::bind_method(D_METHOD("get_version"), &ASPS::get_version);
}