#ifndef ANITYA_COMPONENT_H
#define ANITYA_COMPONENT_H

#include "core/reference.h"
#include "scene/main/node.h"
#include "scene/3d/spatial.h"
#include "uuid.h"


class Entity;

class ComponentProperty: public Reference 
{
    GDCLASS(ComponentProperty, Reference);

    private:
        String name;
        String value;
        int type;
        
    
    protected:
        static void _bind_methods();

    public:
        void set_name(const String& p_name) { name = p_name; }
        String get_name() const { return name; }

        void set_value(const String& p_value) { value = p_value; }
        String get_value() const { return value; }

        void set_type(int p_type) { type = p_type; }
        int get_type() const { return type; }
};

class BaseComponent: public Node
{
    GDCLASS(BaseComponent, Node);

    private:
        Ref<UUID> uuid;
        String component_name;
        // Entity* entity;
        PoolStringArray expected_properties;

    protected:
        static void _bind_methods();
    
    public:
        virtual void start() {}
        void set_uuid(const Ref<UUID>& p_uuid) { uuid = p_uuid; }
        Ref<UUID> get_uuid() const { return uuid; }

        //HACK: It must be Enitty class.
        virtual void set_entity(Node* p_entity) {}
        // {
        //     Entity* e = Object::cast_to<Entity>(p_entity);
        //     if (e)
        //     {
        //         entity = e; 
        //     }
        //     else
        //     {
        //         WARN_PRINT("p_entity doesn't inherit from Entity");
        //     }
        // }
        // //HACK: IT MUST BE AN Entity
        // virtual Node* get_entity() const { return (Node*)entity; }
        virtual Spatial* get_entity() const { return nullptr; }

        virtual void apply_property(const Ref<ComponentProperty>& p_property) {}
        void set_component_name(const String& p_name) { component_name = p_name; }
        String get_component_name() const { return component_name; }
        virtual void destroy() {}
        void set_expected_properties(const PoolStringArray& p_expected_properties) { expected_properties = p_expected_properties; }
        PoolStringArray get_expected_properties() const { return expected_properties; }

        // virtual void _on_component_added(Node* p_component) {}
        BaseComponent();
        ~BaseComponent();
};


#endif