#include "ffmpeg_streamer.h"

void FFmpegStreamer::load_path(String p_path, bool p_convert_to_rgb) {
	if (livestream) {
		livestream->load_stream(p_path, true, 30);
	}
}

void FFmpegStreamer::play() {
	if (livestream) {
		livestream->play();
	}
}

void FFmpegStreamer::pause() {
	if (livestream) {
		livestream->pause();
	}
}

bool FFmpegStreamer::is_playing() const {
	return livestream->is_playing();
}

Ref<ImageTexture> FFmpegStreamer::get_video_texture() {
	return texture;
}

float FFmpegStreamer::get_length() const {
	if (livestream) {
		return livestream->get_length();
	}

	return 0.0;
}

void FFmpegStreamer::set_loop(bool p_enable) {
	if (livestream) {
		livestream->set_loop(p_enable);
	}
}

bool FFmpegStreamer::has_loop() const {
	if (livestream) {
		return livestream->has_loop();
	}
	return false;
}

float FFmpegStreamer::get_playback_position() const {
	if (livestream) {
		livestream->get_playback_position();
	}
	return 0.0f;
}

void FFmpegStreamer::seek(float p_time) {
	if (livestream) {
		livestream->seek(p_time);
	}
}

void FFmpegStreamer::_on_frame_loaded(Ref<Image> image, PoolVector2Array pcm) {
	if (first_frame) {
		texture->create_from_image(image);
		first_frame = false;

		// audio_player->play();
	} else {
		texture->set_data(image);
	}

	emit_signal("pcm_loaded", pcm, 1, 1);
}

void FFmpegStreamer::_on_path_loaded(bool value) {
	emit_signal("path_loaded", value);
}

void FFmpegStreamer::_notification(int p_what) {
}

void FFmpegStreamer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_path", "path", "convert_to_rgb"), &FFmpegStreamer::load_path, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("play"), &FFmpegStreamer::play);
	ClassDB::bind_method(D_METHOD("pause"), &FFmpegStreamer::pause);
	ClassDB::bind_method(D_METHOD("is_playing"), &FFmpegStreamer::is_playing);
	ClassDB::bind_method(D_METHOD("get_video_texture"), &FFmpegStreamer::get_video_texture);
	ClassDB::bind_method(D_METHOD("get_length"), &FFmpegStreamer::get_length);
	ClassDB::bind_method(D_METHOD("set_loop", "enable"), &FFmpegStreamer::set_loop);
	ClassDB::bind_method(D_METHOD("has_loop"), &FFmpegStreamer::has_loop);
	ClassDB::bind_method(D_METHOD("get_playback_position"), &FFmpegStreamer::get_playback_position);
	ClassDB::bind_method(D_METHOD("seek", "time"), &FFmpegStreamer::seek);
	ClassDB::bind_method(D_METHOD("_on_frame_loaded", "image", "pcm"), &FFmpegStreamer::_on_frame_loaded);
	ClassDB::bind_method(D_METHOD("_on_path_loaded", "success"), &FFmpegStreamer::_on_path_loaded);

	ADD_SIGNAL(MethodInfo("path_loaded", PropertyInfo(Variant::BOOL, "successful")));
	ADD_SIGNAL(MethodInfo("pcm_loaded", PropertyInfo(Variant::POOL_BYTE_ARRAY, "pcm"), PropertyInfo(Variant::INT, "channels")));

	// BIND_ENUM_CONSTANT(ASF_FORMAT_UNKNOWN);
	// BIND_ENUM_CONSTANT(ASF_FORMAT_SAMPLE_8);
	// BIND_ENUM_CONSTANT(ASF_FORMAT_SAMPLE_16);
	// BIND_ENUM_CONSTANT(ASF_FORMAT_SAMPLE_32);
	// BIND_ENUM_CONSTANT(ASF_FORMAT_FLOATING);
	// BIND_ENUM_CONSTANT(ASF_FORMAT_DOUBLE);
}



FFmpegStreamer::FFmpegStreamer() {
	texture = Ref<ImageTexture>(memnew(ImageTexture));
	image = Ref<Image>(memnew(Image()));

	texture_rect = memnew(TextureRect);
	add_child(texture_rect);
	texture_rect->set_texture(texture);
	texture_rect->set_expand(true);
	texture_rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	livestream = memnew(Livestream);
	add_child(livestream);
	livestream->connect("frame_loaded", this, "_on_frame_loaded");
	livestream->connect("path_loaded", this, "_on_path_loaded");
	// audio_player = memnew(AudioStreamPlayer);
	// add_child(audio_player);
	// audio_stream = Ref<AudioStreamGenerator>(memnew(AudioStreamGenerator));
	// audio_player->set_stream(audio_stream);


}

FFmpegStreamer::~FFmpegStreamer() {
	if (video_player) {
		player_destroy(video_player);
	}
}
