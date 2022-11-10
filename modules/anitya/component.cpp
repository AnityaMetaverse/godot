#include "component.h"

void ComponentProperty::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_name", "name"), &ComponentProperty::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &ComponentProperty::get_name);

    ClassDB::bind_method(D_METHOD("set_value", "value"), &ComponentProperty::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &ComponentProperty::get_value);

    ClassDB::bind_method(D_METHOD("set_type", "type"), &ComponentProperty::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &ComponentProperty::get_type);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "set_value", "get_value");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
}

void Component::_bind_methods()
{

}