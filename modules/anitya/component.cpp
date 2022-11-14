#include "component.h"

#include "entity.h"

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

void BaseComponent::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start"), &BaseComponent::start);
    ClassDB::bind_method(D_METHOD("apply_property"), &BaseComponent::apply_property);
    ClassDB::bind_method(D_METHOD("destroy"), &BaseComponent::destroy);

    ClassDB::bind_method(D_METHOD("set_entity", "entity"), &BaseComponent::set_entity);
    ClassDB::bind_method(D_METHOD("get_entity"), &BaseComponent::get_entity);

    ClassDB::bind_method(D_METHOD("set_uuid", "uuid"), &BaseComponent::set_uuid);
    ClassDB::bind_method(D_METHOD("get_uuid"), &BaseComponent::get_uuid);

    ClassDB::bind_method(D_METHOD("set_component_name", "BaseComponent_name"), &BaseComponent::set_component_name);
    ClassDB::bind_method(D_METHOD("get_component_name"), &BaseComponent::get_component_name);

    ClassDB::bind_method(D_METHOD("set_expected_properties", "expected_properties"), &BaseComponent::set_expected_properties);
    ClassDB::bind_method(D_METHOD("get_expected_properties"), &BaseComponent::get_expected_properties);

    // ClassDB::bind_method(D_METHOD("_on_component_added"), &Component::_on_component_added);

    // ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity"), "set_entity", "get_entity");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uuid"), "set_uuid", "get_uuid");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "expected_properties"), "set_expected_properties", "get_expected_properties");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "component_name"), "set_component_name", "get_component_name");
}

BaseComponent::BaseComponent(): uuid(memnew(UUID))
{

}

BaseComponent::~BaseComponent()
{

}