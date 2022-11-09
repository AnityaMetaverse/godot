#ifndef UUID_H
#define UUID_H

#include "core/reference.h"
#include "core/math/random_number_generator.h"

class UUID: public Reference
{
    GDCLASS(UUID, Reference);

    private:
        RandomNumberGenerator generator;
        String uuid;
    protected:
        static void _bind_methods();
    
    public:
        Ref<UUID> copy();
        // String as_string();
        // void use(const String& p_uuid_string) { uuid = p_uuid_string; }
        void set_value(const String& p_uuid) { uuid = p_uuid; }
        String get_value() const { return uuid; }

        UUID();
        ~UUID();
};

#endif