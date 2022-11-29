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

void Component::set_entity(Node* p_entity)
{
    Entity* e = Object::cast_to<Entity>(p_entity);
    if (e)
    {
        entity = e; 
    }
    else
    {
        ERR_PRINT("p_entity doesn't inherit from Entity");
    }
}

void Component::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start"), &Component::start);
    ClassDB::bind_method(D_METHOD("apply_property"), &Component::apply_property);
    ClassDB::bind_method(D_METHOD("destroy"), &Component::destroy);

    ClassDB::bind_method(D_METHOD("set_entity", "entity"), &Component::set_entity);
    ClassDB::bind_method(D_METHOD("get_entity"), &Component::get_entity);

    ClassDB::bind_method(D_METHOD("set_uuid", "uuid"), &Component::set_uuid);
    ClassDB::bind_method(D_METHOD("get_uuid"), &Component::get_uuid);

    ClassDB::bind_method(D_METHOD("set_component_name", "component_name"), &Component::set_component_name);
    ClassDB::bind_method(D_METHOD("get_component_name"), &Component::get_component_name);

    ClassDB::bind_method(D_METHOD("set_expected_properties", "expected_properties"), &Component::set_expected_properties);
    ClassDB::bind_method(D_METHOD("get_expected_properties"), &Component::get_expected_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &Component::get_properties);
    // ClassDB::bind_method(D_METHOD("_init"), &Component::_init);

    // ClassDB::bind_method(D_METHOD("_on_component_added"), &Component::_on_component_added);


    // ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity"), "set_entity", "get_entity");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uuid"), "set_uuid", "get_uuid");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "expected_properties"), "set_expected_properties", "get_expected_properties");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "component_name"), "set_component_name", "get_component_name");

    ADD_SIGNAL(MethodInfo("component_ready", PropertyInfo(Variant::OBJECT, "component")));
}

// void Component::_init()
// {
//     // uuid = Ref<UUID>(memnew(UUID));
// }

Component::Component(): uuid(Ref<UUID>(memnew(UUID)))
{

}

Component::~Component()
{

}