#include "register_types.h"

// #include "core/class_db.h"
// #include "anitya_audio_client.h"
#include "asps.h"
#include "uuid.h"
#include "entity.h"
#include "component.h"

void register_anitya_types()
{
    ClassDB::register_class<UUID>();
    ClassDB::register_class<ComponentProperty>();
    // ClassDB::register_class<BaseComponent>();
    ClassDB::register_class<Entity>();
    ClassDB::register_class<BaseComponent>();
    ClassDB::register_class<ASPS>();

    // ClassDB::register_class<AudioClientUpdatePosition>();
    // ClassDB::register_class<JoinChannelParameter>();
    // ClassDB::register_virtual_class<AudioClient>();
    // ClassDB::register_class<AudioClientConfig>();
    // ClassDB::register_virtual_class<RemoteAudioClient>();
    // ClassDB::register_class<AudioClientConfig>();
    // ClassDB::register_class<RemoteAudioClient>();
}

void unregister_anitya_types()
{

}
