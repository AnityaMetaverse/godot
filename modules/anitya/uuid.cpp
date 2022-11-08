#include "uuid.h"

void UUID::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("copy"), &UUID::copy);
    ClassDB::bind_method(D_METHOD("as_string"), &UUID::as_string);
}

Ref<UUID> UUID::copy()
{
    return Ref<UUID>();
}

String UUID::as_string()
{
    return uuid;
}

UUID::UUID()
{
    const char* values = "0123456789abcdef";
    // int[5] sections = {8, 4, 4, 4, 16};
    uuid = String("00000000-0000-0000-0000-000000000000");

    // uuid.set(0, values[generator.randi() % 16]);

}

UUID::~UUID()
{

}