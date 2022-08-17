#ifndef LIVESTREAM_H
#define LIVESTREAM_H

#include "scene/main/node.h"
#include "core/os/thread.h"
#include "core/os/mutex.h"
// #include "audio_array.h"
#include "player.h"

// #include <atomic>
// #include <thread>

class Livestream: public Node {

    GDCLASS(Livestream, Node);

    private:
        struct StreamTupleRef {
            Ref<Image> image;
            PoolVector2Array audio_data;
        };
        PoolVector<StreamTupleRef> stream_buffer;
        int framerate = 30;
        float delta = 1.0f / 30.0f;
        float acc = 0.0f;
        size_t audio_data_length = 0;
        int width;
        int height;
        int data_size;
        String selected_url;
        int audio_channels = 1;
        MediaPlayerContext *video_player = nullptr;
        PoolRealArray audio_array;
        // AudioStreamPlayer *audio_player = nullptr;
        bool exit_thread = false;
        bool destroy_thread = false;
        bool clean_audio = false;
        bool is_processing = false;
        bool is_dispatching = false;
        int mix_rate;
        bool was_loading = false;
        int player_state = -1;
        uint8_t *last_video_data;
        void *last_video_release_ptr = nullptr;
        uint8_t* video_data[3];
        void *last_audio_release_ptr = nullptr;
        float accumulator_fetching = 0.0f;;
        float accumulator_dispatch = 0.0f;
        const float _60_fps = 1.0f / 60.0f;
        // std::atomic<bool> is_running_process;
        // std::thread video_audio_thread;
        Mutex data_mutex;
        Mutex accumulator_mutex;
        Thread video_audio_thread;
        bool first_frame;
        PoolByteArray image_data;

        /*
	* Some definitions are not completely clear to me. Need more investigation.
	*/
        size_t audio_frame_size = 2048;
        float overlap_time = 0.02f;
        float audio_overlap_length;
    
    private:
        void _grab_video();
        void _grab_audio();
        void _clean_audio();
        void _update_framerate(int framerate);
        void _init_video_audio_process();
        static void _init_video_audio_process(void *p_user_data);
        void _video_audio_process();
        static void _video_audio_process(void *p_user_data);
        void clean_stream();

    protected:
        static void _bind_methods();
        void _notification(int p_what);

    public:
        void load_stream(String p_path, bool p_to_rgb, int framerate);

        void play();
        void pause();
        bool is_playing() const;
        void set_loop(bool p_enable);
        bool has_loop() const;

        float get_length() const;

        float get_playback_position() const;
        void seek(float p_time);
        Ref<Image> get_video_frame() const;
    
        Livestream();
        ~Livestream();
};

#endif