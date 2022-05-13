
void comps_colors_render() {

	int swatch_size = (int) ((float) comp_space.w * 0.666f / 16.f);
	if (swatch_size < 16) swatch_size = 16;

	// ALL COLORS
	int colors_x = comp_space.x + comp_space.w - swatch_size * 16 - 8;
	int colors_y = comp_space.y + comp_space.h - swatch_size * 4;
	for (int c = 0; c < 64; c++) {
		int xoff = colors_x + (c % 16) * swatch_size;
		int yoff = colors_y + (c >> 4) * swatch_size;
		render_color_set(renderer, colors[c]);
		SDL_RenderFillRect(renderer, &(SDL_Rect) { xoff, yoff, swatch_size, swatch_size });
		if (swatch_size >= 24) {
			ascii_color_set(colors[c] ^ 0xffffff0f);
			unsigned char hex[3];
			sprintf((char*)hex, "%02X", c);
			ascii_text_render(hex, xoff + swatch_size - 24, yoff + 4);
		}
	}
	// highlight current color
	render_color_set(renderer, (palette_current_color_id < 0x20) ? colors[0x41] : colors[0x45]);
	int x_off = colors_x + (palette_current_color_id % 16) * swatch_size;
	int y_off = colors_y + (palette_current_color_id >> 4) * swatch_size;
	SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
		x_off - 1, y_off,
		swatch_size + 2, swatch_size
	});
	SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
		x_off, y_off - 1,
		swatch_size, swatch_size + 2
	});

	// draw all palette swatches
	comps_palettes_render(
		comp_space.x + (colors_x - comp_space.x) / 2, 
		colors_y + (comp_space.h - colors_y) / 2);

	comp_space.h -= swatch_size * 4 + 16;
}
