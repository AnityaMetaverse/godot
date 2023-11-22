#ifndef GODOT_MESH_OPTIMIZER_H
#define GODOT_MESH_OPTIMIZER_H

#include "core/reference.h"
#include "core/array.h"
#include "meshoptimizer.h"
#include "scene/resources/mesh.h"
#include <vector>

class MeshOptimizer : public Reference {
	GDCLASS(MeshOptimizer, Reference);

private:

protected:
	static void _bind_methods();

public:

	Ref<ArrayMesh> optimize_for_collision_counted(Ref<ArrayMesh> p_mesh, int target_index_count, float tolerance);
	static Array surface_optimize_for_cllision_counted(const Array &vertices, int target_index_count, float tolerance);

	// if there is a guarantee that indices are in order we wouldn't need different buffers.
	// check that later if needed.
	template<typename T>
	void remap_array(T *out_data, const T *in_data, unsigned int *indices, size_t index_count);

	static PoolIntArray triangle_strip_to_triangles(const PoolIntArray &triangle_strip);

	MeshOptimizer();
	~MeshOptimizer();
};

template<typename T>
void MeshOptimizer::remap_array(T *out_data, const T *in_data, unsigned int *indices, size_t index_count) {
	for(size_t i = 0; i < index_count; ++i) {
		out_data[i] = in_data[indices[i]];
	}
};

#endif
