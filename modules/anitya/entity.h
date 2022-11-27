#ifndef ANITYA_ENTITY_H
#define ANITYA_ENTITY_H

#include "scene/3d/spatial.h"
#include "uuid.h"
#include "component.h"


// class DummyEntity: public Spatial
// {
//     GDCLASS(DummyEntity, Spatial);

//     protected:
//         static void _bind_methods() {}
// };

// class Component;
class ComponentProperty;
class Entity: public Spatial
{
    GDCLASS(Entity, Spatial);
    private:
        Ref<UUID> uuid;
        String entity_name;
        Vector<Component*> components;
        // enum 
    protected:
        static void _bind_methods();
    public:
        void set_uuid(const Ref<UUID>& p_uuid) { uuid = p_uuid; }
        Ref<UUID> get_uuid() const { return uuid; }

        void set_entity_name(const String& p_entity_name) { entity_name = p_entity_name; }
        String get_entity_name() const { return entity_name; }



        void set_property(const Ref<ComponentProperty>& p_property, const Ref<UUID>& p_uuid);

        //HACK: IT MUST BE Component
        void add_component(Node* p_component);

        //HACK: IT MUST BE Component
        void remove_component(Node* p_component);

        //HACK: IT MUST BE Vector<Component>
        Vector<Variant> get_components() const;

        void start();
        // void dummy(DummyEntity* e) {}

        Node* find_component_by_uuid(const Ref<UUID>& p_uuid);
        Node* find_component_by_name(const String& p_name);
    
        Entity();
        ~Entity();
};

#endif