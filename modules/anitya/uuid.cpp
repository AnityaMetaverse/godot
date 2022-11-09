#include "uuid.h"

#include <random>
#include "sstream"

void UUID::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_value", "value"), &UUID::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &UUID::get_value);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "set_value", "get_value");
}

UUID::UUID()
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    }

    uuid = String(ss.str().c_str());
    // uuid.set(0, values[generator.randi() % 16]);

}

UUID::~UUID()
{

}