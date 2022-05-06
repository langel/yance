
void comps_editor_render() {
	// adjust space for padding
	comp_space.y += 8;
	comp_space.h -= 8;
	comp_space.w -= 8;
	// calculate comp space ratio
	float x_ratio = (float) comp_space.w / (float) (table_selection.w * 8);
	float y_ratio = (float) comp_space.h / (float) (table_selection.h * 8);
	float ratio = (x_ratio < y_ratio) ? x_ratio : y_ratio;
	ratio *= 8;

	int x_off = comp_space.x + (comp_space.w - table_selection.w * ratio) / 2;
	int y_off = comp_space.y + (comp_space.h - table_selection.h * ratio) / 2;

	// SHOW TILE(S)
	for (int x = 0; x < table_selection.w; x++) {
		for (int y = 0; y < table_selection.h; y++) {
			SDL_RenderCopy(renderer, table_tiles[
				table_selection.x + x + (table_selection.y + y) * 16].texture,
				NULL, &(SDL_Rect) {
				x_off + (int) (ratio * (float) x),
				y_off + (int) (ratio * (float) y),
				(int) ceilf(ratio),
				(int) ceilf(ratio),
			});
		}
	}
}
