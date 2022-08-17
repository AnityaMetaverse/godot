#include "register_types.h"

#include "core/class_db.h"
#include "ffmpeg_streamer.h"

void register_ffmpeg_streamer_types() {
	ClassDB::register_class<FFmpegStreamer>();
}

void unregister_ffmpeg_streamer_types() {}
