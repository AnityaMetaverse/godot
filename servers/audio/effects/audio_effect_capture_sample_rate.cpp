#include "audio_effect_capture_sample_rate.h"

#include "core/math/math_funcs.h"
#include "servers/audio_server.h"

void AudioEffectCaptureSampleRateInstance::process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) {
	float frames_until_next_sample = AudioServer::get_singleton()->get_mix_rate() / base->rate;

	for (int i = 0; i < p_frame_count; i++) {
		if (processed_frames >= frames_until_next_sample) {
			last_sampled_frame = p_src_frames[i]; // sample a new frame
			processed_frames = Math::fmod(processed_frames - frames_until_next_sample, 1.0f);
		}

		// output dry/wet signal based on the `mix` control
		p_dst_frames[i] = last_sampled_frame * base->mix + p_src_frames[i] * (1.0f - base->mix);

		processed_frames++;
	}

    if (buffer.space_left() >= processed_frames) {
		// Add incoming audio frames to the IO ring buffer
		int32_t ret = buffer.write(p_src_frames, processed_frames);
		ERR_FAIL_COND_MSG(ret != processed_frames, "Failed to add data to effect capture ring buffer despite sufficient space.");
		base->pushed_frames += processed_frames;
	} else {
		base->discarded_frames += processed_frames;
	}
}

Ref<AudioEffectInstance> AudioEffectCaptureSampleRate::instance() {
	Ref<AudioEffectCaptureSampleRateInstance> ins;
	ins.instance();
	ins->base = Ref<AudioEffectCaptureSampleRate>(this);
	return ins;
}

void AudioEffectCaptureSampleRate::set_rate(float p_rate) {
	rate = MAX(p_rate, 1.0f);
}
float AudioEffectCaptureSampleRate::get_rate() const {
	return rate;
}

void AudioEffectCaptureSampleRate::set_mix(float p_mix) {
	mix = CLAMP(p_mix, 0.0f, 1.0f);
}
float AudioEffectCaptureSampleRate::get_mix() const {
	return mix;
}

void AudioEffectCaptureSampleRate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_rate", "rate"), &AudioEffectCaptureSampleRate::set_rate);
	ClassDB::bind_method(D_METHOD("get_rate"), &AudioEffectCaptureSampleRate::get_rate);

	ClassDB::bind_method(D_METHOD("set_mix", "mix"), &AudioEffectCaptureSampleRate::set_mix);
	ClassDB::bind_method(D_METHOD("get_mix"), &AudioEffectCaptureSampleRate::get_mix);

    ClassDB::bind_method(D_METHOD("can_get_buffer", "frames"), &AudioEffectCaptureSampleRate::can_get_buffer);
	ClassDB::bind_method(D_METHOD("get_buffer", "frames"), &AudioEffectCaptureSampleRate::get_buffer);
	ClassDB::bind_method(D_METHOD("clear_buffer"), &AudioEffectCaptureSampleRate::clear_buffer);
	ClassDB::bind_method(D_METHOD("set_buffer_length", "buffer_length_seconds"), &AudioEffectCaptureSampleRate::set_buffer_length);
	ClassDB::bind_method(D_METHOD("get_buffer_length"), &AudioEffectCaptureSampleRate::get_buffer_length);
	ClassDB::bind_method(D_METHOD("get_frames_available"), &AudioEffectCaptureSampleRate::get_frames_available);
	ClassDB::bind_method(D_METHOD("get_discarded_frames"), &AudioEffectCaptureSampleRate::get_discarded_frames);
	ClassDB::bind_method(D_METHOD("get_buffer_length_frames"), &AudioEffectCaptureSampleRate::get_buffer_length_frames);
	ClassDB::bind_method(D_METHOD("get_pushed_frames"), &AudioEffectCaptureSampleRate::get_pushed_frames);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "buffer_length", PROPERTY_HINT_RANGE, "0.01,10,0.01"), "set_buffer_length", "get_buffer_length");

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rate", PROPERTY_HINT_RANGE, "1.0,22050.0,1.0,or_greater,hide_slider,suffix:Hz"), "set_rate", "get_rate");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mix", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_mix", "get_mix");
}

void AudioEffectCaptureSampleRate::set_buffer_length(float p_buffer_length_seconds) {
	buffer_length_seconds = p_buffer_length_seconds;
}

float AudioEffectCaptureSampleRate::get_buffer_length() {
	return buffer_length_seconds;
}

int AudioEffectCaptureSampleRate::get_frames_available() const {
	ERR_FAIL_COND_V(!buffer_initialized, 0);
	return buffer.data_left();
}

int64_t AudioEffectCaptureSampleRate::get_discarded_frames() const {
	return discarded_frames;
}

int AudioEffectCaptureSampleRate::get_buffer_length_frames() const {
	ERR_FAIL_COND_V(!buffer_initialized, 0);
	return buffer.size();
}

int64_t AudioEffectCaptureSampleRate::get_pushed_frames() const {
	return pushed_frames;
}

bool AudioEffectCaptureSampleRate::can_get_buffer(int p_frames) const {
	return buffer.data_left() >= p_frames;
}

PoolVector2Array AudioEffectCaptureSampleRate::get_buffer(int p_frames) {
	ERR_FAIL_COND_V(!buffer_initialized, PoolVector2Array());
	ERR_FAIL_INDEX_V(p_frames, buffer.size(), PoolVector2Array());
	int data_left = buffer.data_left();
	if (data_left < p_frames || p_frames == 0) {
		return PoolVector2Array();
	}

	PoolVector2Array ret;
	ret.resize(p_frames);

	{
		PoolVector<AudioFrame> streaming_data;
		streaming_data.resize(p_frames);
		buffer.read(streaming_data.write().ptr(), p_frames);

		PoolVector2Array::Write retw = ret.write();
		for (int32_t i = 0; i < p_frames; i++) {
			retw[i] = Vector2(streaming_data[i].l, streaming_data[i].r);
		}
	}
	return ret;
}

void AudioEffectCaptureSampleRate::clear_buffer() {
	const int32_t data_left = buffer.data_left();
	buffer.advance_read(data_left);
}

AudioEffectCaptureSampleRate::AudioEffectCaptureSampleRate() {
	rate = 11025.0f;
	mix = 1.0f;
}
