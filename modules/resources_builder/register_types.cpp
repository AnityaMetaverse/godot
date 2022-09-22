#include "register_types.h"

#include "core/class_db.h"
#include "resources_builder.h"
#include "fixed_byte_array.h"


void register_resources_builder_types()
{
    ClassDB::register_class<ResourcesBuilder>();
    ClassDB::register_class<FixedByteArray>();
}

void unregister_resources_builder_types()
{
    
}