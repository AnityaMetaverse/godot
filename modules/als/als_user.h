#ifndef ALS_ALS_USER_H
#define ALS_ALS_USER_H

#include "core/reference.h"

class ALSUser: public Reference
{
    GDCLASS(ALSUser, Reference)

    private:
        Array _packages;
        int _user_id;

    protected:
        static void _bind_methods();
        
    public:
        PoolByteArray get_voice_package();
        void add_voice_package(const PoolByteArray& voice);
        int get_user_id() const { return _user_id; }
        void set_user_id(int user_id) { _user_id = static_cast<int8_t>(user_id); }
        ALSUser() = default;
        ALSUser(const ALSUser& other);
};

#endif