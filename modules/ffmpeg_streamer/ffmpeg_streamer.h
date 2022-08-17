#ifndef FFMPEG_STREAMER_H
#define FFMPEG_STREAMER_H

#include "player.h"

#include "core/reference.h"
#include "scene/audio/audio_stream_player.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/resources/texture.h"
#include "servers/audio/effects/audio_stream_generator.h"
#include "livestream.h"



class FFmpegStreamer : public MarginContainer {
	GDCLASS(FFmpegStreamer, MarginContainer);

public:
	enum AudioStreamFormat {
		ASF_FORMAT_UNKNOWN = 0, 
		ASF_FORMAT_SAMPLE_8,  
		ASF_FORMAT_SAMPLE_16,
		ASF_FORMAT_SAMPLE_32,
		ASF_FORMAT_FLOATING, 
		ASF_FORMAT_DOUBLE
	};

private:
	Ref<ImageTexture> texture;
	Ref<Image> image;
	Ref<AudioStreamGenerator> audio_stream;

	MediaPlayerContext *video_player = nullptr;
	AudioStreamPlayer *audio_player = nullptr;
	PoolByteArray audio_buffer;
	size_t audio_data_length = 0;
	Livestream* livestream;

	/*
	* Some definitions are not completely clear to me. Need more investigation.
	*/
	size_t audio_frame_size = 2048;
	float overlap_time = 0.02f;
	float audio_overlap_length;

	TextureRect *texture_rect = nullptr;

	bool was_loading = false;
	bool first_frame = true;
	bool paused = false;
	bool looping = false;

	int width = 0;
	int height = 0;
	int data_size = 0;

	int audio_channels = 1;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void load_path(String p_path, bool p_convert_to_rgb = true);

	void play();
	void pause();

	bool is_playing() const;

	void set_loop(bool p_enable);
	bool has_loop() const;

	Ref<ImageTexture> get_video_texture();

	float get_length() const;

	float get_playback_position() const;
	void seek(float p_time);

	void _on_frame_loaded(Ref<Image> image, PoolVector2Array pcm);
	void _on_path_loaded(bool success);

	FFmpegStreamer();
	~FFmpegStreamer();
};

#endif // FFMPEG_STREAMER_H
