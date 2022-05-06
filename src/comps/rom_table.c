
int comps_rom_table_scroll_pos;

void comps_rom_table_render() {

	// SHOW TABLES
	int tile_size = (window_rect.w > 1024) ? 24 : 16;
	tile_rect.w = tile_rect.h = tile_size;
	// XXX need to calculate height of all tiles
	//     need to calculate scroll position
	//     and create offsets and boundaries for rendering
	int rows_start = 0;
	int rows_visible = (comp_space.h / tile_size) << 4;
	if (rows_visible > rom_tile_count) {
		rows_visible = rom_tile_count;
	}
	else {
		rows_start = ((table_selection.y + table_selection.h) << 4) - rows_visible;
		if (rows_start < 0) rows_start = 0;
	}
	int i = 0;
	for (int t = rows_start; t < rows_visible; t++) {
		tile_rect.x = 8 + (i % 16) * tile_size;
		tile_rect.y = 8 + (i >> 4) * tile_size;
		SDL_RenderCopy(renderer, table_tiles[t].texture, NULL, &tile_rect);
		i++;
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


void comps_rom_table_update() {
	int key_repeat = 5;
	void selection_reset() {
		table_selection.w = 1;
		table_selection.h = 1;
	}
	if (keys_ctrl && keys[SDL_SCANCODE_D]) {
		selection_reset();
	}
	if (keys[SDL_SCANCODE_PAGEUP] % key_repeat == 1) {
		table_selection.y -= 16;
		if (table_selection.y < 0) table_selection.y = 0;
	}
	if (keys[SDL_SCANCODE_PAGEDOWN] % key_repeat == 1) {
		table_selection.y += 16;
		if (table_selection.y + table_selection.h > rom_tile_count >> 4) table_selection.y = (rom_tile_count >> 4) - table_selection.h;
	}
	if (keys[SDL_SCANCODE_UP] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (table_selection.y != 0) {
				table_selection.y--;
				table_selection.h++;
			}
		}
		else if (keys_ctrl) {
			if (table_selection.h > 1) table_selection.h--;
		}
		else {
			if (table_selection.y != 0) table_selection.y--;
		}
	}
	if (keys[SDL_SCANCODE_DOWN] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (table_selection.y + table_selection.h < rom_tile_count >> 4) {
				table_selection.h++;
			}
		}
		else if (keys_ctrl) {
			if (table_selection.h > 1) {
				table_selection.y++;
				table_selection.h--;
			}
		}
		else {	
			if (table_selection.y + table_selection.h < rom_tile_count >> 4) {
				table_selection.y++;
			}
		}
	}
	if (keys[SDL_SCANCODE_LEFT] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (table_selection.x != 0) {
				table_selection.x--;
				table_selection.w++;
			}
		}
		else if (keys_ctrl) {
			if (table_selection.w > 1) table_selection.w--;
		}
		else {	
			if (table_selection.x != 0) table_selection.x--;
		}
	}
	if (keys[SDL_SCANCODE_RIGHT] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (table_selection.x + table_selection.w < 16) {
				table_selection.w++;
			}
		}
		else if (keys_ctrl) {
			if (table_selection.w > 1) {
				table_selection.x++;
				table_selection.w--;
			}
		}
		else {
			if (table_selection.x + table_selection.w < 16) {
				table_selection.x++;
			}
		}
	}
}
