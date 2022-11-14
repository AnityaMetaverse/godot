#include "entity.h"
#include "component.h"


void Entity::set_property(const Ref<ComponentProperty>& p_property, const Ref<UUID>& p_uuid)
{

}

void Entity::add_component(Node* p_component)
{
    BaseComponent* c = Object::cast_to<BaseComponent>(p_component);
    if (c)
    {
        c->call("set_entity", this);
        components.push_back(c);
        add_child(c);
        emit_signal("component_added", c);
        // connect("component_added", c, "_on_component_added");
    }
    else
    {
        WARN_PRINT("p_component doesn't inherit from Component");
    }
}

void Entity::remove_component(Node* p_component)
{
    BaseComponent* comp = Object::cast_to<BaseComponent>(p_component);

    if (comp)
    {
        for (int index = 0; index < components.size(); index++)
        {
            BaseComponent* c = components[index];
            if (c == comp)
            {
                remove_child(c);
                components.remove(index);
                emit_signal("component_being_removed", c);
                c->call("destroy");
            }

        }
    }
    else
    {
        WARN_PRINT("p_component doesn't inherit from Component");
    }
    
}


Node* Entity::find_component_by_uuid(const Ref<UUID>& p_uuid)
{
    for (int index = 0; index < components.size(); index++)
    {
        BaseComponent* c = components[index];
        if (c->get_uuid()->get_value() == p_uuid->get_value())
        {
            return (Node*)c;
        }
    }
    return nullptr;
}

void Entity::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_property", "property", "uuid"), &Entity::set_property);
    ClassDB::bind_method(D_METHOD("add_component", "component"), &Entity::add_component);

    ClassDB::bind_method(D_METHOD("find_component_by_uuid", "uuid"), &Entity::find_component_by_uuid);
    // ClassDB::bind_method(D_METHOD("dummy", "e"), &Entity::dummy);

    ClassDB::bind_method(D_METHOD("set_entity_name", "name"), &Entity::set_entity_name);
    ClassDB::bind_method(D_METHOD("get_entity_name"), &Entity::get_entity_name);
    ClassDB::bind_method(D_METHOD("set_uuid", "uuid"), &Entity::set_uuid);
    ClassDB::bind_method(D_METHOD("get_uuid"), &Entity::get_uuid);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uuid"), "set_uuid", "get_uuid");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "entity_name"), "set_entity_name", "get_entity_name");


    ADD_SIGNAL(MethodInfo("component_added", PropertyInfo(Variant::OBJECT, "component")));
    ADD_SIGNAL(MethodInfo("component_being_removed", PropertyInfo(Variant::OBJECT, "component")));
}

Entity::Entity(): entity_name("Entity"), uuid(Ref<UUID>(memnew(UUID)))
{
    // uuid = Ref<UUID>(memnew(UUID));
}

Entity::~Entity()
{

}