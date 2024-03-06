#include "register_types.h"

#include "core/class_db.h"
#include "core/engine.h"

#include "als.h"
#include "als_pack.h"
#include "als_user.h"
#include "als_client.h"


static _ALS* _als = nullptr;

void register_als_types()
{
    print_error(String("[ANITYA] calling register_als_types"));
    ClassDB::register_class<_ALS>();
    ClassDB::register_class<ALSPack>();
    ClassDB::register_class<ALSClient>();
    ClassDB::register_class<ALSUser>();

    _als = memnew(_ALS);
    Engine::get_singleton()->add_singleton(Engine::Singleton("ALS", _ALS::get_singleton()));
}

void unregister_als_types()
{
    memdelete(_als);
}

