#include "entity.h"


void Entity::set_property(Ref<ComponentProperty> p_property, Component* p_component)
{

}

void Entity::add_component(Component* p_component)
{

}

Component* Entity::find_component_by_uuid(const Ref<UUID>& p_uuid)
{
    return nullptr;
}

void Entity::_bind_methods()
{
    // ClassDB::bind_method(D_METHOD("set_property", "property", "component"), &Entity::set_property);
    ClassDB::bind_method(D_METHOD("add_component", "component"), &Entity::add_component);
    ClassDB::bind_method(D_METHOD("find_component_by_uuid", "uuid"), &Entity::find_component_by_uuid);

    ADD_SIGNAL(MethodInfo("component_added", PropertyInfo(Variant::OBJECT, "component")));
    ADD_SIGNAL(MethodInfo("component_removing", PropertyInfo(Variant::OBJECT, "component")));
}