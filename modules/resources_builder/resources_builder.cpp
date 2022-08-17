#include "resources_builder.h"

#include "scene/3d/spatial.h"


Spatial* ResourcesBuilder::build(const String& p_filepath, int p_resource_type)
{
    // ClassDB::bind_method(D_METHOD("import_gltf_scene", "path", "flags", "bake_fps", "compress_flags", "state"),
	// 		&PackedSceneGLTF::import_gltf_scene, DEFVAL(0), DEFVAL(1000.0f), DEFVAL(Mesh::ARRAY_COMPRESS_DEFAULT), DEFVAL(Ref<GLTFState>()));
    
    Ref<GLTFState> state;
    Node* node = glft_parser->import_gltf_scene(p_filepath, 0, 1000.0f, Mesh::ARRAY_COMPRESS_DEFAULT, state);
    return cast_to<Spatial>(node);
}

ResourcesBuilder::ResourcesBuilder()
{
    glft_parser.instance();
}

void ResourcesBuilder::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("build", "filepath", "resource_type"), &ResourcesBuilder::build);
}

