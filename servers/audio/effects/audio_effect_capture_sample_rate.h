#ifndef AUDIO_EFFECT_CAPTURE_SAMPLE_RATE_H
#define AUDIO_EFFECT_CAPTURE_SAMPLE_RATE_H

#include "servers/audio/audio_effect.h"
#include "core/math/audio_frame.h"
#include "core/pool_vector.h"
#include "core/reference.h"
#include "core/ring_buffer.h"

class AudioEffectCaptureSampleRate;

class AudioEffectCaptureSampleRateInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectCaptureSampleRateInstance, AudioEffectInstance);
	friend class AudioEffectCaptureSampleRate;
	Ref<AudioEffectCaptureSampleRate> base;

	float processed_frames = 0;
	AudioFrame last_sampled_frame;

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);
};

class AudioEffectCaptureSampleRate : public AudioEffect {
	GDCLASS(AudioEffectCaptureSampleRate, AudioEffect);

	friend class AudioEffectCaptureSampleRateInstance;

	RingBuffer<AudioFrame> buffer;
	uint64_t discarded_frames;
	uint64_t pushed_frames;
	float buffer_length_seconds;
	bool buffer_initialized;

protected:
	static void _bind_methods();

public:
	float rate;
	float mix;

	Ref<AudioEffectInstance> instance();
	void set_rate(float p_rate);
	float get_rate() const;
	void set_mix(float p_mix);
	float get_mix() const;

	void set_buffer_length(float p_buffer_length_seconds);
	float get_buffer_length();

	bool can_get_buffer(int p_frames) const;
	PoolVector2Array get_buffer(int p_len);
	void clear_buffer();

	int get_frames_available() const;
	int64_t get_discarded_frames() const;
	int get_buffer_length_frames() const;
	int64_t get_pushed_frames() const;

	AudioEffectCaptureSampleRate();
};

#endif