#include "livestream.h"


void Livestream::load_stream(String p_url, bool p_to_rgb, int framerate){
	clean_stream();
	is_processing = false;
    destroy_thread = true; // Danger!
    _update_framerate(framerate);
    CharString utf8 = p_url.utf8();
	const char *cstr = utf8.get_data();
	video_player = player_create(cstr, p_to_rgb ? 1 : 0);
	Thread t;
	t.start(_init_video_audio_process, this, Thread::Settings());
}

void Livestream::play() {
    if (video_player && player_is_playing(video_player) == 0) {
		player_play(video_player);
		set_process(true);
		is_processing = true;
	}
}

void Livestream::pause() {
    if (video_player && player_is_playing(video_player) == 1) {
		player_stop(video_player);
		set_process(false);
		is_processing = false;
	}
}

bool Livestream::is_playing() const {
    return player_is_playing(video_player) == 1;
    // return true;
}

void Livestream::_update_framerate(int p_framerate) {
    framerate = p_framerate;
    delta = 1.0f / (float)framerate;
	print_line(String("Framerate: ") + rtos(delta));
}

float Livestream::get_length() const {
	if (video_player && player_get_state(video_player) == StateReady) {
		return player_get_length(video_player);
	}

	return 0.0;
}

void Livestream::set_loop(bool p_enable) {
	player_set_loop(video_player, p_enable ? 1 : 0);
}

bool Livestream::has_loop() const {
	return player_has_loop(video_player) == 1;
    // return true;
}

float Livestream::get_playback_position() const {
	return player_get_playback_position(video_player);
    // return 0.0f;
}

void Livestream::seek(float p_time) {
	clean_audio = true;
	if (video_player && player_get_state(video_player) == StateReady) {
		float time = CLAMP(p_time, 0, player_get_length(video_player));
		player_seek(video_player, time);
	}
}

void Livestream::clean_stream() {
	stream_buffer.resize(0);
}



void Livestream::_init_video_audio_process() {
	do {
		if (player_state == StateError) {
			if (was_loading) {
				was_loading = false;
				set_process(false);
				print_error("== ANITYA == Error loading URL");
				emit_signal("path_loaded", false);
			}
			
		}
	} while((player_state = player_get_state(video_player)) == StateLoading);


	if (player_state == StateReady) {
			player_get_video_format(video_player, &width, &height);
			data_size = width * height * 3;
			image_data.resize(data_size);

			was_loading = false;
			set_process(false);

			
			player_get_audio_format(video_player, &mix_rate, &audio_channels);
			print_line(String("MixRate: ") + itos(mix_rate));
			// audio_stream->set_mix_rate(mix_rate);

			audio_overlap_length = (size_t)(overlap_time * mix_rate + 0.5f);
			audio_data_length = (audio_frame_size) * audio_channels;
			emit_signal("path_loaded", true);
			print_line("== ANITYA == URL Opened");
			is_processing = true;
            destroy_thread = false;
            first_frame = true;
	        set_process(true);
			// video_audio_thread.start(_video_audio_process, this, Thread::Settings());
	}
	else if (player_state == StateError) {
		print_error("Error opening URL");
	}

}

void Livestream::_init_video_audio_process(void *p_user_data) {
	Livestream * l = (Livestream*)p_user_data;
	l->_init_video_audio_process();
}

void Livestream::_video_audio_process() {
    while(!exit_thread && !destroy_thread) {
        // while (is_running_process.load());
    }
}

void Livestream::_video_audio_process(void *p_user_data) {
	Livestream *l = (Livestream*)p_user_data;
	l->_video_audio_process();
}

Ref<Image> Livestream::get_video_frame() const {
	Ref<Image> i = nullptr;

	if (last_video_release_ptr != nullptr)
	{
		i = Ref<Image>(memnew(Image));
		i->create(width, height, false, Image::FORMAT_RGB8, image_data);
	}
	return i;
}


void Livestream::_grab_video() {
    void *release_ptr = nullptr;

    do {
		player_grab_video_frame(video_player, &release_ptr, video_data);

		if (release_ptr != NULL) {

			if (last_video_release_ptr != NULL) {
				// print_line("Fetched release video data...");
				memcpy(image_data.write().ptr(), video_data[0], image_data.size());
				void *temp_release_ptr = last_video_release_ptr;
				last_video_release_ptr = release_ptr;
				player_release_frame(video_player, temp_release_ptr);
			}

			last_video_release_ptr = release_ptr;
		}
	} while (release_ptr != nullptr);
}


void Livestream::_grab_audio() {
	if (clean_audio) {
		_clean_audio();
		clean_audio = false;
	}

	int frame_size = 0;
	int format;
	int helper_format = 0;
	uint8_t *audio_data = nullptr;
	void *release_ptr = nullptr;
	player_grab_audio_frame(video_player, &release_ptr, &audio_data, &frame_size, &format);
	frame_size *= audio_channels;

	if (audio_data != nullptr) {
		// Convert all formats into a common format to emit signal.
		// In order to properly interpret it, use format parameter
		switch (format)
		{
			case AV_SAMPLE_FMT_NONE:
				helper_format = 0;
				print_line("== ANITYA == Received a UNKNOWN audio");

				break;
			case AV_SAMPLE_FMT_FLT:
				helper_format = 1;
				// print_line("== ANITYA == Received a floating audio");
				
				
				break;
			case AV_SAMPLE_FMT_S16:
				helper_format = 2;
				print_line("== ANITYA == Received a SAMPLE 16 BITS audio");

				break;
			case AV_SAMPLE_FMT_U8:
				helper_format = 3;
				print_line("== ANITYA == Received a UNSIGNED 8 BITS audio");

				break;
			case AV_SAMPLE_FMT_S32:
				helper_format = 4;
				print_line("== ANITYA == Received a SAMPLE 32 BITS audio");

				break;
			case AV_SAMPLE_FMT_DBL:
				helper_format = 5;
				print_line("== ANITYA == Received a DOUBLE audio");
				break;
		}

		int float_frame_size = frame_size * 4;
		PoolRealArray temp;
		temp.resize(frame_size);
		memcpy(temp.write().ptr(), audio_data, float_frame_size);
		audio_array.append_array(temp);
		
		if (audio_array.size() > audio_data_length)
		{
			StreamTupleRef tuple;
			tuple.image = get_video_frame();
			tuple.audio_data.resize(audio_array.size() / 2);
			// memcpy(tuple.audio_data.write().ptr(), audio_data, tuple.audio_data.size() * 4);
			for (int index = 0; index < tuple.audio_data.size(); index++)
			{
				tuple.audio_data.set(index,
					Vector2(
						audio_array[index * 2],
						audio_array[index * 2 + 1]
					)
				);
			}


			emit_signal("frame_loaded", tuple.image, tuple.audio_data);
			audio_array = PoolRealArray();
			// stream_buffer.push_back(tuple);
		}
		player_release_frame(video_player, release_ptr);
	}
}

void Livestream::_clean_audio() {
	// audio_buffer.resize(0);
}

void Livestream::_notification(int p_what) {
    // print_line("_notification");
    if (p_what == NOTIFICATION_PROCESS) {

		float value = get_process_delta_time();
		accumulator_dispatch += value;
		accumulator_fetching += value;

		if (is_processing) {
            _grab_video();
            _grab_audio();
        }
        
		// if (accumulator_dispatch > delta) {
			// if (!is_dispatching && stream_buffer.size() > 60) {
			// 	is_dispatching = true;
			// }

			// if (is_dispatching) {
			// 	Ref<Image> i = stream_buffer[0].image;
			// 	PoolVector2Array a = stream_buffer[0].audio_data;
			// 	emit_signal("frame_loaded", i, a);

			// 	stream_buffer.remove(0);
			// 	if (stream_buffer.empty()) {
			// 		is_dispatching = false;
			// 	}

			// 	data_mutex.unlock();
			// }
			
			// accumulator_dispatch = accumulator_dispatch - delta;
        // }
    }
}

void Livestream::_bind_methods() {

	ClassDB::bind_method(D_METHOD("load_stream", "path", "to_rgb", "framerate"), &Livestream::load_stream);
	ClassDB::bind_method(D_METHOD("play"), &Livestream::play);
	ClassDB::bind_method(D_METHOD("pause"), &Livestream::pause);
	ClassDB::bind_method(D_METHOD("is_playing"), &Livestream::is_playing);
	// ClassDB::bind_method(D_METHOD("get_video_texture"), &Livestream::get_video_texture);
	ClassDB::bind_method(D_METHOD("get_length"), &Livestream::get_length);
	ClassDB::bind_method(D_METHOD("set_loop", "enable"), &Livestream::set_loop);
	ClassDB::bind_method(D_METHOD("has_loop"), &Livestream::has_loop);
	ClassDB::bind_method(D_METHOD("get_playback_position"), &Livestream::get_playback_position);
	ClassDB::bind_method(D_METHOD("seek", "time"), &Livestream::seek);

    ADD_SIGNAL(MethodInfo("path_loaded", PropertyInfo(Variant::BOOL, "successful")));
    ADD_SIGNAL(MethodInfo("frame_loaded", PropertyInfo(Variant::OBJECT, "p_image", PROPERTY_HINT_RESOURCE_TYPE, "Image"), PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "pcm")));
}

Livestream::Livestream() {
    video_player = nullptr;
    // accumulator = 0.0f;
    // is_running_process.store(false);
    first_frame = true;
    destroy_thread = false;
}

// Livestream::Livestream() {
//     // first_frame = true;
//     // destroy_thread = false;
// }


Livestream::~Livestream() {
    exit_thread = true;
    // video_audio_thread.join();
    if (video_player) {
        player_destroy(video_player);
    }
	destroy_thread = true;
}