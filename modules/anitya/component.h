#ifndef ANITYA_COMPONENT_H
#define ANITYA_COMPONENT_H

#include "core/reference.h"
#include "scene/main/node.h"
#include "uuid.h"

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

class Component: public Node
{
    GDCLASS(Component, Node);

    private:
        Ref<UUID> uuid;
        String unique_name;
    protected:
        static void _bind_methods();
    
    public:
        Component() {}
        ~Component() {}
};


#endif