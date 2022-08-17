#ifndef RESOURCES_BUILDER
#define RESOURCES_BUILDER

#include "core/class_db.h"
#include "core/reference.h"
#include "modules/gltf/packed_scene_gltf.h"


class Spatial;
// #include "modules/glft/glft_document.h"

class ResourcesBuilder: public Reference
{
    GDCLASS(ResourcesBuilder, Reference);

    private:
        // enum ResourceType {
        //     GLFT
        // };
    
    Ref<PackedSceneGLTF> glft_parser;

    protected:
        static void _bind_methods();

    public:
        ResourcesBuilder();
        Spatial* build(const String &p_filepath, int p_resource_type);
};

#endif