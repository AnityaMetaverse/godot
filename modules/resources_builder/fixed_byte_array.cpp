#include "fixed_byte_array.h"


void FixedByteArray::push(const PoolByteArray& byte_data)
{
    int size = byte_data.size();
    memcpy(data.write().ptr() + current_pos, byte_data.read().ptr(), size);
    current_pos += size;
}

void FixedByteArray::set_size(int p_size)
{
    data.resize(p_size);
}

PoolByteArray FixedByteArray::get_byte_data()
{
    return data;
}

FixedByteArray::FixedByteArray()
{
    // data.resize(p_size);
    current_pos = 0;
}

FixedByteArray::~FixedByteArray()
{

}

void FixedByteArray::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("push", "byte_data"), &FixedByteArray::push);
    ClassDB::bind_method(D_METHOD("set_size", "size"), &FixedByteArray::set_size);
    ClassDB::bind_method(D_METHOD("get_byte_data"), &FixedByteArray::get_byte_data);
}

