#include "godot_mesh_optimizer.h"


void MeshOptimizer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("optimize_for_collision_counted", "mesh", "target_index_count", "tolerance"), &MeshOptimizer::optimize_for_collision_counted);
}


MeshOptimizer::MeshOptimizer() {

}

MeshOptimizer::~MeshOptimizer() {
    
}

Ref<ArrayMesh> MeshOptimizer::optimize_for_collision_counted(Ref<ArrayMesh> p_mesh, int p_target_index_count, float p_tolerance) {
    // TODO : optimize buy not generating separate arrays.

    Ref<ArrayMesh> ret_mesh;
    ret_mesh.instance();

    for(size_t sr_index = 0; sr_index < p_mesh->get_surface_count(); ++sr_index) {
        auto type = p_mesh->surface_get_primitive_type(sr_index);
        switch(type) {
            case Mesh::PRIMITIVE_TRIANGLE_STRIP:
            {
                auto arrays = p_mesh->surface_get_arrays(sr_index);
                auto indices = Object::cast_to<PoolIntArray>(arrays[Mesh::ARRAY_INDEX]);
                arrays[Mesh::ARRAY_INDEX] = triangle_strip_to_triangles(*indices);
                Array surface_buffer = surface_optimize_for_cllision_counted(arrays, p_target_index_count, p_tolerance);
                ret_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, surface_buffer);
                break;
            }
            case Mesh::PRIMITIVE_TRIANGLES:
            {
                Array surface_buffer = surface_optimize_for_cllision_counted(p_mesh->surface_get_arrays(sr_index), p_target_index_count, p_tolerance);
                ret_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, surface_buffer);
                break;
            }
            default:
                break;
        }
    }

    return ret_mesh;
}

Array MeshOptimizer::surface_optimize_for_cllision_counted(const Array &arrays,int target_index_count, float tolerance) {
    Array ret;
    ret.resize(Mesh::ARRAY_MAX);

    auto varray = (PoolVector3Array)(arrays[Mesh::ARRAY_VERTEX]);
    const Vector3 *varray_read = varray.read().ptr();
    auto normal_array = (PoolVector3Array)(arrays[Mesh::ARRAY_NORMAL]);
    const Vector3 *normal_read = normal_array.read().ptr();
    auto indices = (PoolIntArray)(arrays[Mesh::ARRAY_INDEX]);
    const int *indices_read = indices.read().ptr();

    bool has_normal = normal_array.size() == varray.size();

    auto simplified = std::vector<int>(indices.size());

    float res_error = 0.0;

    auto simplified_count = meshopt_simplifySloppy<int>(simplified.data(), indices_read,
        indices.size(), (const float *)(varray_read), varray.size(), sizeof(Vector3), target_index_count, tolerance, &res_error);
    
    // now contains an array of indices
    simplified.resize(simplified_count);

    // we now have new indices. we need to remap vertices (attributes are not needed for collision).
    auto vremap_buffer = std::vector<unsigned int>(indices.size());
    PoolIntArray iremap_buffer;
    iremap_buffer.resize(simplified_count);
    // creates an array of indices to create the new array buffer. this is for vertex remapping.
    auto new_vertex_count = meshopt_generateVertexRemap<int>(vremap_buffer.data(), simplified.data(), simplified.size(), varray_read, varray.size(), sizeof(Vector3));
    PoolVector3Array new_varray;
    
    new_varray.resize(new_vertex_count);

    meshopt_remapIndexBuffer<int>(iremap_buffer.write().ptr(), simplified.data(), simplified.size(), vremap_buffer.data());
    meshopt_remapVertexBuffer(new_varray.write().ptr(), varray_read, varray.size(), sizeof(Vector3), (unsigned int *)(vremap_buffer.data()));
    if (has_normal) {
        PoolVector3Array new_normal_array;
        new_normal_array.resize(new_vertex_count);
        meshopt_remapVertexBuffer(new_normal_array.write().ptr(), normal_read, normal_array.size(), sizeof(Vector3), (unsigned int *)(vremap_buffer.data()));
        ret[Mesh::ARRAY_NORMAL] = new_normal_array;
    }

    ret[Mesh::ARRAY_VERTEX] = new_varray;
    ret[Mesh::ARRAY_INDEX] = iremap_buffer;

    return ret;
}

PoolIntArray MeshOptimizer::triangle_strip_to_triangles(const PoolIntArray &triangle_strip) {
    PoolIntArray ret;
    // TODO : optimize this later.

    for (size_t i = 2; i < triangle_strip.size(); ++i) {
        if (i % 2 == 0) {
            // For even i, the order is (i-2, i-1, i)
            ret.push_back(triangle_strip[i - 2]);
            ret.push_back(triangle_strip[i - 1]);
            ret.push_back(triangle_strip[i]);
        } else {
            // For odd i, the order is reversed to maintain winding (i-1, i-2, i)
            ret.push_back(triangle_strip[i - 1]);
            ret.push_back(triangle_strip[i - 2]);
            ret.push_back(triangle_strip[i]);
        }
    }

    return ret;
}