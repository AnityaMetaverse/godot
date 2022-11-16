#include "gltf_stream.h"

#include "editor/import/resource_importer_scene.h"

#include "modules/gltf/gltf_accessor.h"
#include "modules/gltf/gltf_animation.h"
#include "modules/gltf/gltf_camera.h"
#include "modules/gltf/gltf_light.h"
#include "modules/gltf/gltf_mesh.h"
#include "modules/gltf/gltf_node.h"
#include "modules/gltf/gltf_skeleton.h"
#include "modules/gltf/gltf_skin.h"
#include "modules/gltf/gltf_spec_gloss.h"
#include "modules/gltf/gltf_state.h"
#include "modules/gltf/gltf_texture.h"

#include "core/bind/core_bind.h" // FIXME: Shouldn't use _Directory but DirAccess.
#include "core/crypto/crypto_core.h"
#include "core/io/json.h"
#include "core/math/disjoint_set.h"
#include "core/os/file_access.h"
#include "core/variant.h"
#include "core/version.h"
#include "drivers/png/png_driver_common.h"
#include "scene/2d/node_2d.h"
#include "scene/3d/bone_attachment.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/multimesh_instance.h"
#include "scene/3d/spatial.h"
#include "scene/animation/animation_player.h"
#include "scene/main/node.h"
#include "scene/resources/surface_tool.h"


Error GLTFDocumentStream::parse_from_stream(Ref<GLTFState> state, const PoolByteArray& p_stream, bool p_read_binary)
{
	Error err;
	// FileAccessRef f = FileAccess::open(p_path, FileAccess::READ, &err);
	// if (!f) {
	// 	return err;
	// }

	// uint32_t magic = f->get_32();
	const uint8_t* data = p_stream.read().ptr();
	uint32_t magic = *((uint32_t*)(data));
	if (magic == 0x46546C67) {
		//binary file
		//text file
		err = _parse_glb(p_stream, state);
		if (err) {
			print_line("Error parsing GLB");
			return FAILED;
		}
	} 
	else {
		// ERR_FAIL_COND_V(false, Error::FAILED);
		ERR_PRINT("GLTF it is not supported");
		return Error::FAILED;
		//text file
		// err = _parse_json(p_path, state);
		// if (err) {
		// 	return FAILED;
		// }
	}
	// f->close();

	// get file's name, use for scene name if none
	// state->filename = p_path.get_file().get_slice(".", 0);
	state->filename = String("Model");

	// return OK;
	

	ERR_FAIL_COND_V(!state->json.has("asset"), Error::FAILED);

	Dictionary asset = state->json["asset"];

	ERR_FAIL_COND_V(!asset.has("version"), Error::FAILED);

	String version = asset["version"];

	state->major_version = version.get_slice(".", 0).to_int();
	state->minor_version = version.get_slice(".", 1).to_int();

	/* PARSE EXTENSIONS */

	print_line("Parsing extension");
	err = _parse_gltf_extensions(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE SCENE */
	print_line("Parsing scenes");
	err = _parse_scenes(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE NODES */
	print_line("Parsing nodes");
	err = _parse_nodes(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE BUFFERS */
	print_line("Parsing buffers");
	err = _parse_buffers(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE BUFFER VIEWS */
	print_line("Parsing extension");
	err = _parse_buffer_views(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE ACCESSORS */
	print_line("Parsing accessors");
	err = _parse_accessors(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE IMAGES */
	print_line("Parsing images");
	err = _parse_images(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE TEXTURES */
	print_line("Parsing textures");
	err = _parse_textures(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE TEXTURES */
	print_line("Parsing materials");
	err = _parse_materials(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE SKINS */
	print_line("Parsing skins");
	err = _parse_skins(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* DETERMINE SKELETONS */
	print_line("determine skeletons");
	err = _determine_skeletons(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* CREATE SKELETONS */
	print_line("crearte skeletons");
	err = _create_skeletons(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* CREATE SKINS */
	print_line("create skins");
	err = _create_skins(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE MESHES (we have enough info now) */
	print_line("Parsing meshes");
	err = _parse_meshes(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE LIGHTS */
	print_line("Parsing lights");
	err = _parse_lights(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE CAMERAS */
	print_line("Parsing cameras");
	err = _parse_cameras(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* PARSE ANIMATIONS */
	print_line("Parsing animations");
	err = _parse_animations(state);
	if (err != OK) {
		return Error::FAILED;
	}

	/* ASSIGN SCENE NAMES */
	print_line("assign scene names");
	_assign_scene_names(state);

	return OK;
}


Error GLTFDocumentStream::_parse_glb(const PoolByteArray &p_stream, Ref<GLTFState> state) {
	Error err;
	// FileAccessRef f = FileAccess::open(p_path, FileAccess::READ, &err);
	// if (!f) {
	// 	return err;
	// }

	uint32_t index = 0;
	const uint8_t* data = p_stream.read().ptr();
	uint32_t magic = *((uint32_t*)(data));
	// uint32_t magic = f->get_32();
	index = 4;
	ERR_FAIL_COND_V(magic != 0x46546C67, ERR_FILE_UNRECOGNIZED); //glTF
	// f->get_32(); // version
	index += 4;
	// f->get_32(); // length
	index += 4;

	// uint32_t chunk_length = f->get_32();
	uint32_t chunk_length = *((uint32_t*)(data + index));
	index += 4;
	// uint32_t chunk_type = f->get_32();
	uint32_t chunk_type = *((uint32_t*)(data + index));
	index += 4;

	ERR_FAIL_COND_V(chunk_type != 0x4E4F534A, ERR_PARSE_ERROR); //JSON
	Vector<uint8_t> json_data;
	json_data.resize(chunk_length);
	// uint32_t len = f->get_buffer(json_data.ptrw(), chunk_length);
	memcpy(json_data.ptrw(), data + index, chunk_length);
	// uint32_t len = f->get_buffer(json_data.ptrw(), chunk_length);
	index += chunk_length;
	// ERR_FAIL_COND_V(len != chunk_length, ERR_FILE_CORRUPT);

	String text;
	text.parse_utf8((const char *)json_data.ptr(), json_data.size());

	print_line(text);
	// return OK;
	String err_txt;
	int err_line;
	Variant v;
	err = JSON::parse(text, v, err_txt, err_line);
	if (err != OK) {
		_err_print_error("", "Parsing stream", err_line, err_txt.utf8().get_data(), ERR_HANDLER_SCRIPT);
		return err;
	}

	state->json = v;

	//data?

	// chunk_length = f->get_32();
	chunk_length = *((uint32_t*)(data + index));
	index += 4;
	// chunk_type = f->get_32();
	chunk_type = *((uint32_t*)(data + index));
	index += 4;

	if (index + 1 == p_stream.size()) {
		print_line("All good");
		return OK; //all good
	}

	ERR_FAIL_COND_V(chunk_type != 0x004E4942, ERR_PARSE_ERROR); //BIN

	state->glb_data.resize(chunk_length);
	// uint32_t len = f->get_buffer(state->glb_data.ptrw(), chunk_length);
	memcpy(state->glb_data.ptrw(), data + index, chunk_length);
	// ERR_FAIL_COND_V(len != chunk_length, ERR_FILE_CORRUPT);

	return OK;
}

Error GLTFDocumentStream::_parse_images(Ref<GLTFState> state) {
	if (!state->json.has("images")) {
		return OK;
	}

	// Ref: https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#images

	const Array &images = state->json["images"];
	for (int i = 0; i < images.size(); i++) {
		const Dictionary &d = images[i];

		// glTF 2.0 supports PNG and JPEG types, which can be specified as (from spec):
		// "- a URI to an external file in one of the supported images formats, or
		//  - a URI with embedded base64-encoded data, or
		//  - a reference to a bufferView; in that case mimeType must be defined."
		// Since mimeType is optional for external files and base64 data, we'll have to
		// fall back on letting Godot parse the data to figure out if it's PNG or JPEG.

		// We'll assume that we use either URI or bufferView, so let's warn the user
		// if their image somehow uses both. And fail if it has neither.
		ERR_CONTINUE_MSG(!d.has("uri") && !d.has("bufferView"), "Invalid image definition in glTF file, it should specify an 'uri' or 'bufferView'.");
		if (d.has("uri") && d.has("bufferView")) {
			WARN_PRINT("Invalid image definition in glTF file using both 'uri' and 'bufferView'. 'uri' will take precedence.");
		}

		String mimetype;
		if (d.has("mimeType")) { // Should be "image/png" or "image/jpeg".
			mimetype = d["mimeType"];
		}

		Vector<uint8_t> data;
		const uint8_t *data_ptr = nullptr;
		int data_size = 0;

		// if (d.has("uri")) {
		// 	// Handles the first two bullet points from the spec (embedded data, or external file).
		// 	String uri = d["uri"];

		// 	if (uri.begins_with("data:")) { // Embedded data using base64.
		// 		// Validate data MIME types and throw a warning if it's one we don't know/support.
		// 		if (!uri.begins_with("data:application/octet-stream;base64") &&
		// 				!uri.begins_with("data:application/gltf-buffer;base64") &&
		// 				!uri.begins_with("data:image/png;base64") &&
		// 				!uri.begins_with("data:image/jpeg;base64")) {
		// 			WARN_PRINT(vformat("glTF: Image index '%d' uses an unsupported URI data type: %s. Skipping it.", i, uri));
		// 			state->images.push_back(Ref<Texture>()); // Placeholder to keep count.
		// 			continue;
		// 		}
		// 		data = _parse_base64_uri(uri);
		// 		data_ptr = data.ptr();
		// 		data_size = data.size();
		// 		// mimeType is optional, but if we have it defined in the URI, let's use it.
		// 		if (mimetype.empty()) {
		// 			if (uri.begins_with("data:image/png;base64")) {
		// 				mimetype = "image/png";
		// 			} else if (uri.begins_with("data:image/jpeg;base64")) {
		// 				mimetype = "image/jpeg";
		// 			}
		// 		}
		// 	} else { // Relative path to an external image file.
		// 		uri = uri.http_unescape();
		// 		uri = p_base_path.plus_file(uri).replace("\\", "/"); // Fix for Windows.
		// 		// ResourceLoader will rely on the file extension to use the relevant loader.
		// 		// The spec says that if mimeType is defined, it should take precedence (e.g.
		// 		// there could be a `.png` image which is actually JPEG), but there's no easy
		// 		// API for that in Godot, so we'd have to load as a buffer (i.e. embedded in
		// 		// the material), so we do this only as fallback.
		// 		Ref<Texture> texture = ResourceLoader::load(uri);
		// 		if (texture.is_valid()) {
		// 			state->images.push_back(texture);
		// 			continue;
		// 		} else if (mimetype == "image/png" || mimetype == "image/jpeg") {
		// 			// Fallback to loading as byte array.
		// 			// This enables us to support the spec's requirement that we honor mimetype
		// 			// regardless of file URI.
		// 			data = FileAccess::get_file_as_array(uri);
		// 			if (data.size() == 0) {
		// 				WARN_PRINT(vformat("glTF: Image index '%d' couldn't be loaded as a buffer of MIME type '%s' from URI: %s. Skipping it.", i, mimetype, uri));
		// 				state->images.push_back(Ref<Texture>()); // Placeholder to keep count.
		// 				continue;
		// 			}
		// 			data_ptr = data.ptr();
		// 			data_size = data.size();
		// 		} else {
		// 			WARN_PRINT(vformat("glTF: Image index '%d' couldn't be loaded from URI: %s. Skipping it.", i, uri));
		// 			state->images.push_back(Ref<Texture>()); // Placeholder to keep count.
		// 			continue;
		// 		}
		// 	}
		// } else 
		if (d.has("bufferView")) {
			// Handles the third bullet point from the spec (bufferView).
			ERR_FAIL_COND_V_MSG(mimetype.empty(), ERR_FILE_CORRUPT,
					vformat("glTF: Image index '%d' specifies 'bufferView' but no 'mimeType', which is invalid.", i));

			const GLTFBufferViewIndex bvi = d["bufferView"];

			ERR_FAIL_INDEX_V(bvi, state->buffer_views.size(), ERR_PARAMETER_RANGE_ERROR);

			Ref<GLTFBufferView> bv = state->buffer_views[bvi];

			const GLTFBufferIndex bi = bv->buffer;
			ERR_FAIL_INDEX_V(bi, state->buffers.size(), ERR_PARAMETER_RANGE_ERROR);

			ERR_FAIL_COND_V(bv->byte_offset + bv->byte_length > state->buffers[bi].size(), ERR_FILE_CORRUPT);

			data_ptr = &state->buffers[bi][bv->byte_offset];
			data_size = bv->byte_length;
		}

		Ref<Image> img;

		// First we honor the mime types if they were defined.
		if (mimetype == "image/png") { // Load buffer as PNG.
			ERR_FAIL_COND_V(Image::_png_mem_loader_func == nullptr, ERR_UNAVAILABLE);
			img = Image::_png_mem_loader_func(data_ptr, data_size);
		} else if (mimetype == "image/jpeg") { // Loader buffer as JPEG.
			ERR_FAIL_COND_V(Image::_jpg_mem_loader_func == nullptr, ERR_UNAVAILABLE);
			img = Image::_jpg_mem_loader_func(data_ptr, data_size);
		}

		// If we didn't pass the above tests, we attempt loading as PNG and then
		// JPEG directly.
		// This covers URIs with base64-encoded data with application/* type but
		// no optional mimeType property, or bufferViews with a bogus mimeType
		// (e.g. `image/jpeg` but the data is actually PNG).
		// That's not *exactly* what the spec mandates but this lets us be
		// lenient with bogus glb files which do exist in production.
		if (img.is_null()) { // Try PNG first.
			ERR_FAIL_COND_V(Image::_png_mem_loader_func == nullptr, ERR_UNAVAILABLE);
			img = Image::_png_mem_loader_func(data_ptr, data_size);
		}
		if (img.is_null()) { // And then JPEG.
			ERR_FAIL_COND_V(Image::_jpg_mem_loader_func == nullptr, ERR_UNAVAILABLE);
			img = Image::_jpg_mem_loader_func(data_ptr, data_size);
		}
		// Now we've done our best, fix your scenes.
		if (img.is_null()) {
			ERR_PRINT(vformat("glTF: Couldn't load image index '%d' with its given mimetype: %s.", i, mimetype));
			state->images.push_back(Ref<Texture>());
			continue;
		}

		Ref<ImageTexture> t;
		t.instance();
		t->create_from_image(img);

		state->images.push_back(t);
	}

	print_verbose("glTF: Total images: " + itos(state->images.size()));

	return OK;
}


Error GLTFDocumentStream::_parse_buffers(Ref<GLTFState> state) {
	if (!state->json.has("buffers")) {
		return OK;
	}

	const Array &buffers = state->json["buffers"];
	if (state->glb_data.size()) {
		state->buffers.push_back(state->glb_data);

	}
	// for (GLTFBufferIndex i = 0; i < buffers.size(); i++) {
	// 	if (i == 0 && state->glb_data.size()) {
	// 		state->buffers.push_back(state->glb_data);

	// 	} else {
	// 		const Dictionary &buffer = buffers[i];
	// 		if (buffer.has("uri")) {
	// 			Vector<uint8_t> buffer_data;
	// 			String uri = buffer["uri"];

	// 			if (uri.begins_with("data:")) { // Embedded data using base64.
	// 				// Validate data MIME types and throw an error if it's one we don't know/support.
	// 				if (!uri.begins_with("data:application/octet-stream;base64") &&
	// 						!uri.begins_with("data:application/gltf-buffer;base64")) {
	// 					ERR_PRINT("glTF: Got buffer with an unknown URI data type: " + uri);
	// 				}
	// 				buffer_data = _parse_base64_uri(uri);
	// 			} else { // Relative path to an external image file.
	// 				uri = uri.http_unescape();
	// 				uri = p_base_path.plus_file(uri).replace("\\", "/"); // Fix for Windows.
	// 				buffer_data = FileAccess::get_file_as_array(uri);
	// 				ERR_FAIL_COND_V_MSG(buffer.size() == 0, ERR_PARSE_ERROR, "glTF: Couldn't load binary file as an array: " + uri);
	// 			}

	// 			ERR_FAIL_COND_V(!buffer.has("byteLength"), ERR_PARSE_ERROR);
	// 			int byteLength = buffer["byteLength"];
	// 			ERR_FAIL_COND_V(byteLength < buffer_data.size(), ERR_PARSE_ERROR);
	// 			state->buffers.push_back(buffer_data);
	// 		}
	// 	}
	// }

	print_verbose("glTF: Total buffers: " + itos(state->buffers.size()));

	return OK;
}

Node *PackedSceneGLTFStream::import_scene_from_stream(const PoolByteArray& p_stream, uint32_t p_flags,
			int p_bake_fps, uint32_t p_compress_flags,
			List<String> *r_missing_deps,
			Error *r_err,
			Ref<GLTFState> r_state)
{
    if (r_state == Ref<GLTFState>()) {
		r_state.instance();
	}
	r_state->use_named_skin_binds =
			p_flags & EditorSceneImporter::IMPORT_USE_NAMED_SKIN_BINDS;
	r_state->use_legacy_names =
			p_flags & EditorSceneImporter::IMPORT_USE_LEGACY_NAMES;
	r_state->compress_flags = p_compress_flags;

	Ref<GLTFDocumentStream> gltf_document;
	gltf_document.instance();
	Error err = gltf_document->parse_from_stream(r_state, p_stream);
	*r_err = err;
	ERR_FAIL_COND_V(err != Error::OK, nullptr);

	Spatial *root = memnew(Spatial);
	// return root;
	if (r_state->use_legacy_names) {
		root->set_name(gltf_document->_legacy_validate_node_name(r_state->scene_name));
	} else {
		root->set_name(r_state->scene_name);
	}
	for (int32_t root_i = 0; root_i < r_state->root_nodes.size(); root_i++) {
		gltf_document->_generate_scene_node(r_state, root, root, r_state->root_nodes[root_i]);
	}
	gltf_document->_process_mesh_instances(r_state, root);
	if (r_state->animations.size()) {
		AnimationPlayer *ap = memnew(AnimationPlayer);
		root->add_child(ap);
		ap->set_owner(root);
		for (int i = 0; i < r_state->animations.size(); i++) {
			gltf_document->_import_animation(r_state, ap, i, p_bake_fps);
		}
	}

	print_line("Returning parsed GLB");
	return cast_to<Spatial>(root);
}

Node *PackedSceneGLTFStream::import_gltf_scene_from_stream(const PoolByteArray& p_stream, uint32_t p_flags, float p_bake_fps, uint32_t p_compress_flags, Ref<GLTFState> r_state)
{
    Error err = FAILED;
	List<String> deps;
    return import_scene_from_stream(p_stream, p_flags, p_bake_fps, p_compress_flags, &deps, &err, r_state);
}

void PackedSceneGLTFStream::set_stream(const PoolByteArray& p_stream)
{
    // scene = 
}

Node* PackedSceneGLTFStream::get_scene()
{
    Node* tmp = scene;
	scene = nullptr;
	return tmp;
}

void PackedSceneGLTFStream::_bind_methods()
{
    
    ClassDB::bind_method(D_METHOD("set_stream", "stream"), &PackedSceneGLTFStream::set_stream);

    
}

PackedSceneGLTFStream::PackedSceneGLTFStream()
{

}

PackedSceneGLTFStream::~PackedSceneGLTFStream()
{

}

void GLTFParser::set_stream(const PoolByteArray& p_stream)
{
	// gltf_stream.set_stream(p_stream);
	scene = gltf_stream.import_gltf_scene_from_stream(p_stream, get_flags(), get_bake_fps(), get_compress_flags());
}

void GLTFParser::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_flags", "flags"), &GLTFParser::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &GLTFParser::get_flags);
    ClassDB::bind_method(D_METHOD("set_bake_fps", "bake_fps"), &GLTFParser::set_bake_fps);
    ClassDB::bind_method(D_METHOD("get_bake_fps"), &GLTFParser::get_bake_fps);
    ClassDB::bind_method(D_METHOD("set_compress_flags", "compress_flags"), &GLTFParser::set_compress_flags);
    ClassDB::bind_method(D_METHOD("get_compress_flags"), &GLTFParser::get_compress_flags);
	ClassDB::bind_method(D_METHOD("set_stream", "stream"), &GLTFParser::set_stream);
	ClassDB::bind_method(D_METHOD("get_scene"), &GLTFParser::get_scene);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "flags"), "set_flags", "get_flags");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "bake_fps"), "set_bake_fps", "get_bake_fps");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "compress_flags"), "set_compress_flags", "get_compress_flags");
}