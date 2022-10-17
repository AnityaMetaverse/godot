#include "register_types.h"
#include "agora.h"
// #include "agora_client.h"
#include "core/class_db.h"
#include "core/engine.h"

void register_agora_types() {
	// Engine::get_singleton()->add_singleton(Engine::Singleton("Agora", memnew(Agora)));

	ClassDB::register_class<AgoraClient>();
}

void unregister_agora_types() {
}
