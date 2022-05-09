
#define fps_avg_frame_total 16
float fps_avg;
float fps_avg_display;
uint32_t fps_tracker[fps_avg_frame_total] = { 0 };
uint32_t frame_counter;


void fps_update() {
	fps_tracker[frame_counter % fps_avg_frame_total] = SDL_GetTicks();
	fps_avg = 0.f;
	int time_total = 0;
	for (int i = 0; i < fps_avg_frame_total - 1; i++) {
		time_total += fps_tracker[(frame_counter - i) % fps_avg_frame_total] - fps_tracker[(frame_counter - i - 1) % fps_avg_frame_total];
	}
	fps_avg = 1000.f / ((float) time_total / (float) (fps_avg_frame_total - 1));
	if (frame_counter % 20 == 0) fps_avg_display = fps_avg;
	frame_counter++;
}
