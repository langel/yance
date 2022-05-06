
void comps_colors_render() {
	int swatch_size = 40;

	// ALL COLORS
	int colors_x = comp_space.x + comp_space.w - 8 - swatch_size * 16;
	int colors_y = comp_space.h - 8 - swatch_size * 4;
	for (int c = 0; c < 64; c++) {
		int xoff = colors_x + (c % 16) * swatch_size;
		int yoff = colors_y + (c >> 4) * swatch_size;
		render_color_set(renderer, colors[c]);
		SDL_RenderFillRect(renderer, &(SDL_Rect) { xoff, yoff, swatch_size, swatch_size });
		ascii_color_set(colors[c] ^ 0xffffff0f);
		char hex[3];
		sprintf(hex, "%02X", c);
		ascii_text_render(hex, 16 + xoff, 4 + yoff);
	}

	comps_palettes_render(
		comp_space.x + (colors_x - comp_space.x) / 2, 
		colors_y + (comp_space.h - colors_y) / 2);

	comp_space.h -= swatch_size * 4 + 16;
}
