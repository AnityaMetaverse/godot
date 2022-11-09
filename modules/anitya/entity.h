#ifndef ANITYA_ENTITY_H
#define ANITYA_ENTITY_H

#include "scene/3d/spatial.h"
#include "component.h"

class Entity: public Spatial
{
    GDCLASS(Entity, Spatial);
    private:
    protected:
        static void _bind_methods();
    public:
        void set_property(Ref<ComponentProperty> p_property, Component* p_component);
        void add_component(Component* p_component);
        Component* find_component_by_uuid(const Ref<UUID>& p_uuid);
};

#endif