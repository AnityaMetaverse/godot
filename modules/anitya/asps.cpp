#include "asps.h"


PoolByteArray ASPS::encode(const PoolByteArray& p_data)
{
    PoolByteArray result;
    result.resize(p_data.size() + 3);
    result[0] = magic_number;
    result[1] = version;
    result[2] = (uint8_t)45;

    uint8_t* p = result.write().ptr() + 3;
    uint8_t* origin = p_data.read().ptr();

    for (int index = 0; index < p_data.size(); index++)
    {
        *(p + index) = *(origin + index) + 1;
    }

    return result;
}

PoolByteArray ASPS::decode(const PoolByteArray& p_data)
{
    PoolByteArray result;
    uint8_t* data = p_data.write().ptr();
    if (*data != magic_number)
    {
        WARN_PRINT("Unrecognised data format");
        return result;
    }

    if (*(data + 1) != version)
    {
        WARN_PRINT("Invalid version. Current is %d", version);
        return result
    }

    if (*(data + 2) != (uint8_t)45)
    {
        WARN_PRINT("Invalid version. Current is %d", version);
        return result
    }

    result.resize(p_data.size() - 3);

    uint8_t* destiny = result.write().ptr();
    uint8_t* origin = data + 3;

    for (int index = 0; index < result.size(); index++)
    {
        *(destiny + index) = *(origin + index);
    }

    return result;
}

void ASPS::_bind_methods(const PoolByteArray& p_data)
{
    
}