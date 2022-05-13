
enum comp_targets comps_manual_cache_src_comp;

void comps_manual_set_as_target() {
	comps_manual_cache_src_comp = comp_target;
	comp_target = manual;
}

/*
	min screen size = 640x480
	font size = 9x16
	71 x 30 characters on min window
	for centering and status bar: 70x28 char grid
*/

SDL_Rect manual_rect = { 0, 0, 70 * 9, 28 * 16 };
unsigned char manual_border_top[4] = { 0xc9, 0xcd, 0xbb };
unsigned char manual_crap[250];

void comps_manual_render() {
	manual_rect.x = (window_rect.w - manual_rect.w) / 2;
	manual_rect.y = (window_rect.h - manual_rect.h) / 2 - 8;
	render_color_set(renderer, colors[0x02]);
	SDL_RenderFillRect(renderer, &manual_rect);
	ascii_color_set(colors[0x40]);
	ascii_text_render(manual_border_top, manual_rect.x, manual_rect.y);
	for (int i = 80; i < 250; i++) manual_crap[i - 80] = i;
	ascii_text_render(manual_crap, manual_rect.x, manual_rect.y);

	unsigned char * text = (unsigned char*) "M A N U A L";
	ascii_text_render(text, manual_rect.x + 18, manual_rect.y + 32);
}


void comps_manual_update() {
	if (keys[SDL_SCANCODE_ESCAPE] == 1) {
		comp_target = comps_manual_cache_src_comp;
	}
}
