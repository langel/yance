
void comps_rom_table_render() {

	// SHOW TABLES
	int tile_size = (window_rect.w > 1024) ? 24 : 16;
	tile_rect.w = tile_rect.h = tile_size;
	// XXX need to calculate height of all tiles
	//     need to calculate scroll position
	//     and create offsets and boundaries for rendering
	for (int i = 0; i < rom_tile_count; i++) {
		tile_rect.x = 8 + (i % 16) * tile_size;
		tile_rect.y = 8 + (i >> 4) * tile_size;
		SDL_RenderCopy(renderer, table_tiles[i].texture, NULL, &tile_rect);
	}

	// SHOW SELECTION
	render_color_set(renderer, colors[0x40]);
	SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
		6 + table_selection.x * tile_size,
		7 + table_selection.y * tile_size,
		table_selection.w * tile_size + 4,
		table_selection.h * tile_size + 2,
	});
	SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
		7 + table_selection.x * tile_size,
		6 + table_selection.y * tile_size,
		table_selection.w * tile_size + 2,
		table_selection.h * tile_size + 4,
	});

	// adjust space
	int size = tile_size * 16 + 16;
	comp_space.x += size;
	comp_space.w -= size;
}
