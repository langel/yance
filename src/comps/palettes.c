
// $3f00-0f bg
// $3f10-1f spr
void comps_palettes_render(int x_cen, int y_cen) {
	int swatch_size = 8;
	int xoff = x_cen - 2 * swatch_size;
	int yoff = y_cen - 4 * swatch_size;
	for (int pal = 0; pal < 8; pal++) {
		for (int col = 0; col < 4; col++) {
			int color_id = palette[pal][col];
			if (col == 0) color_id = palette[0][0];
			render_color_set(renderer, colors[color_id]);
			SDL_RenderFillRect(renderer, &(SDL_Rect) {
				xoff + col * (swatch_size + 1),
				yoff + (pal * (swatch_size + 1)),
				swatch_size, swatch_size,
			});
		}
	}
}
