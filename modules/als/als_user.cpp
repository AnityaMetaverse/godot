#include "als_user.h"


void ALSUser::add_voice_package(const PoolByteArray& voice)
{
    _packages.push_back(voice);
}

PoolByteArray ALSUser::get_voice_package()
{
    if (_packages.size() != 0)
    {
        PoolByteArray v = _packages.pop_front();
        return v;
    }

    return {};
}

void ALSUser::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_voice_package"), &ALSUser::get_voice_package);
    ClassDB::bind_method(D_METHOD("get_user_id"), &ALSUser::get_user_id);
}

ALSUser::ALSUser(const ALSUser& other)
{
    _user_id = other._user_id;
    _packages = other._packages;
}