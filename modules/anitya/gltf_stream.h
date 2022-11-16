#ifndef ANITYA_GLTF_STREAM_H
#define ANITYA_GLTF_STREAM_H

#include "modules/gltf/gltf_document.h"
#include "modules/gltf/packed_scene_gltf.h"

#include "asset.h"


// https://gltf-transform.donmccurdy.com/classes/core.buffer.html

class GLTFDocumentStream: public GLTFDocument
{
    public:
        Error parse_from_stream(Ref<GLTFState> state, const PoolByteArray& p_stream, bool p_read_binary = false);
        // Error _parse_json_from_stream(const String &p_path, Ref<GLTFState> state);
        Error _parse_glb(const PoolByteArray& p_stream,Ref<GLTFState> state);
	    Error _parse_buffers(Ref<GLTFState> state);
	    Error _parse_images(Ref<GLTFState> state);
        

};

class PackedSceneGLTFStream: public PackedSceneGLTF, public IAsset
{
    GDCLASS(PackedSceneGLTFStream, PackedSceneGLTF);
    private:
        PoolByteArray stream;
        Node* scene;
        Error error;
        

    protected:
        static void _bind_methods();

    public:
        Node* get_scene();
        

        virtual void set_stream(const PoolByteArray& p_stream) override;
        Node *import_scene_from_stream(const PoolByteArray& p_stream, uint32_t p_flags,
			int p_bake_fps, uint32_t p_compress_flags,
			List<String> *r_missing_deps,
			Error *r_err,
			Ref<GLTFState> r_state);
        Node *import_gltf_scene_from_stream(const PoolByteArray& p_stream, uint32_t p_flags, float p_bake_fps, uint32_t p_compress_flags, Ref<GLTFState> r_state = Ref<GLTFState>());

        Error get_error() const { return error; }

    PackedSceneGLTFStream();
    ~PackedSceneGLTFStream();
};

class GLTFParser: public Reference, public IAsset
{
    GDCLASS(GLTFParser, Reference);

    private:
        PackedSceneGLTFStream gltf_stream;
        
        uint32_t flags;
        real_t bake_fps;
        uint32_t compress_flags;

        Node* scene;
    protected:
        static void _bind_methods();

    public:
        void set_flags(uint32_t p_flags) { flags = p_flags; }
        uint32_t get_flags() const { return flags; }
        void set_bake_fps(real_t p_bake_fps) { bake_fps = p_bake_fps; }
        real_t get_bake_fps() const { return bake_fps; }
        void set_compress_flags(uint32_t p_compress_flags) { compress_flags = p_compress_flags; }
        uint32_t get_compress_flags() const { return compress_flags; }
        virtual void set_stream(const PoolByteArray& p_stream) override;
        Node* get_scene()
        {
            Node* tmp = scene;
            scene = nullptr;
            return tmp;
        }
    
};

#endif