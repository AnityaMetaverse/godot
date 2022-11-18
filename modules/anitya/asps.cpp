#include "asps.h"

// #include "modules/tracy/profiler.h"

PoolByteArray ASPS::encode(const PoolByteArray& p_data)
{
    
    PoolByteArray result;
    result.resize(p_data.size() + 3);
    uint8_t* p = result.write().ptr();
    *p = magic_number;
    *(p + 1) = version;
    *(p + 2) = (uint8_t)45;

    p = result.write().ptr() + 3;


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
    if (*data != magic_number)
    {
        WARN_PRINT("Unrecognised data format");
        return result;
    }

    if (*(data + 1) != version)
    {
        WARN_PRINT(String("Invalid version. Current is: ") + String(itos(version)));
        return result;
    }

    if (*(data + 2) != (uint8_t)45)
    {
        WARN_PRINT("No end of header found");
        return result;
    }

    result.resize(p_data.size() - 3);

    uint8_t* destiny = result.write().ptr();
    const uint8_t* origin = data + 3;

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