
double audio_amp = 0.0;
double audio_hertz = 0.0;
double audio_phase = 0.0;
double audio_fade = 0.7;
double audio_bend = 1.0;


void audio_spec_log(SDL_AudioSpec *as) {
	printf(
		" freq______%5d\n"
		" format____%5d\n"
		" channels__%5d\n"
		" silence___%5d\n"
		" samples___%5d\n"
		" size______%5d\n",
		(int) as->freq,
		(int) as->format,
		(int) as->channels,
		(int) as->silence,
		(int) as->samples,
		(int) as->size
	);
}

SDL_AudioSpec audio_spec;
SDL_AudioSpec audio_actual;
SDL_AudioDeviceID audio_device;

void audio_init(int sample_rate, int channels, int buffer_sample_count, SDL_AudioFormat audio_format, void (*callback)()) {
	audio_spec.freq = sample_rate;
	audio_spec.format = audio_format;
	audio_spec.channels = channels;
	audio_spec.samples = buffer_sample_count;
	audio_spec.callback = callback;
	audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, &audio_actual, 0);
	if (audio_device == 0) {
		printf("FAILED TO OPEN AUDIO");
		audio_spec_log(&audio_spec);
	}
	else {
		//logout("device initialized\n");
		//audio_spec_log(&audio_actual);
		SDL_PauseAudioDevice(audio_device, 0);
	}
}




float osc_noise(float pos) {
	return (((float)rand()/(float)(RAND_MAX)) * 2) - 1;
}

float osc_pulse(float pos, float width) {
	return ((pos > width) * 2) - 1;
}

float osc_saw(float pos) {
	return (pos * 2) - 1;
}

float osc_square(float pos) {
	return ((pos > 0.5) * 2) - 1;
}

float osc_triangle(float pos) {
	return (((pos <= 0.5) ? pos * 2 : (1 - pos) * 2) - 0.5);
}

float osc_noise_white() {
	return (((float)rand()/(float)(RAND_MAX)) * 2.f) - 1.f;
}

float osc_noise_brown(float b) {
	b += osc_noise_white() * 0.1;
	if (b > 1.f) b = 1.f;
	else if (b < -1.f) b = -1.f;
	return b;
}

float osc_helper_blep(float phase, float inc) {
	float t = phase;
	if (t < inc) {
		t /= inc;
		return t + t - t * t - 1.f;
	}
	else if (t > 1.f - inc) {
		t = (t - 1.f) / inc;
		return t * t + t + t + 1.f;
	}
	return 0.f;
}

float osc_square_blep(float phase, float inc) {
	float out = ((phase < 0.5f) * 2.f) - 1.f;
	out += osc_helper_blep(phase, inc);
	phase += 0.5f;
	if (phase > 1.f) phase -= 1.f;
	return out -= osc_helper_blep(phase, inc);
}

void audio_callback(void* userdata, uint8_t* byte_stream, int byte_stream_length) {
	float * float_stream = (float*) byte_stream;
	int float_stream_length = byte_stream_length >> 2;
	for (int i = 0; i < float_stream_length; i += 2) {
		audio_amp *= audio_fade;
		audio_hertz *= audio_bend;
		if (audio_hertz > 16000.0 / 32000.0 && audio_bend > 1.0) audio_bend = 1.0;
		if (audio_hertz < 0.16 / 32000.0 && audio_bend < 1.0) audio_bend = 1.0;
		float output_l = 0.f;
		float output_r = 0.f;
		audio_phase += audio_hertz;
		if (audio_phase > 1.0) audio_phase -= 1.0;
		output_l = output_r = osc_square_blep((float) audio_phase, (float) audio_hertz) * (float) audio_amp * 0.8f;
//		output_l = osc_square((float) audio_phase) * (float) audio_amp * 0.5f;
		float_stream[i] = output_l;
		float_stream[i+1] = output_r;
	}
}
