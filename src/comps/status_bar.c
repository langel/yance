
void comps_status_bar_render() {
	// STATUS BAR
	// STATUS BAR INIT
	char status_text[256];
	SDL_Rect status_rect = { 0, 0, window_rect.w, 16 };
	// background
	render_color_set(renderer, colors[0x02]);
	status_rect.w = window_rect.w;
	status_rect.y = window_rect.h - 16;
	SDL_RenderFillRect(renderer, &status_rect);
	// text
	ascii_color_set(colors[0x41]);
	sprintf(status_text, " TILE COUNT: %5d     ROM SIZE: %7d bytes     NO HEADER     FPS: %7.3f", rom_tile_count, rom_tile_count * 16, fps_avg_display);
	ascii_text_render(status_text, 0, status_rect.y);
	// adjust leftover space
	comp_space.h -= 16; // 16 for height 
}
