#include "als_pack.h"

#include <limits.h>



void ALSPack::_bind_methods()
{
    // ClassDB::bind_method(D_METHOD("pack_from_vector2_array", "array"), &ALSPack::pack_from_vector2_array);
    // ClassDB::bind_method(D_METHOD("unpack_to_vector2_array"), &ALSPack::unpack_to_vector2_array);
    ClassDB::bind_method(D_METHOD("data"), &ALSPack::data);
}

ALSPack::ALSPack()
{

}

void ALSPack::resize(int package_size)
{
    _data.resize(package_size);
}
