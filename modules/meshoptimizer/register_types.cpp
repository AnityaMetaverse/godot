#include "register_types.h"

#include "core/class_db.h"
#include "godot_mesh_optimizer.h"

void register_meshoptimizer_types() {
    ClassDB::register_class<MeshOptimizer>();
}

void unregister_meshoptimizer_types() {
}
