
int editor_pixel_w;
int editor_pixel_h;

void comps_editor_render() {
	// adjust space for padding
	comp_space.y += 8;
	comp_space.h -= 8;
	comp_space.w -= 8;
	// update editor dimensions
	editor_pixel_w = table_selection.w << 3;
	editor_pixel_h = table_selection.h << 3;
	// calculate comp space ratio
	float x_ratio = (float) comp_space.w / (float) (editor_pixel_w);
	float y_ratio = (float) comp_space.h / (float) (editor_pixel_h);
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

	// HIGHLIGHT COMP (if current)
	if (comp_target == editor) {
		render_color_set(renderer, colors[0x45]);
		int w = (int) ((float) table_selection.w * ratio);
		int h = (int) ((float) table_selection.h * ratio);
		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			x_off - 3,
			y_off - 1,
			w + 6,
			h + 2,
		});
		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			x_off - 2,
			y_off - 2,
			w + 4,
			h + 4,
		});
		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			x_off - 1,
			y_off - 3,
			w + 2,
			h + 6,
		});
	}

	// SHOW SELECTION
	render_color_set(renderer, colors[0x40]);
	float pxl_ratio = ratio / 8.f;
	SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
		x_off + (int) ((float) editor_selection.x * pxl_ratio),
		y_off + (int) ((float) editor_selection.y * pxl_ratio),
		(int) ((float) editor_selection.w * pxl_ratio),
		(int) ((float) editor_selection.h * pxl_ratio),
	});

}

void editor_paint(uint8_t value) {
	for (int x = 0; x < editor_selection.w; x++) {
		for (int y = 0; y < editor_selection.h; y++) {
			int t = ((editor_selection.x + x) >> 3) + table_selection.x + 
				((((editor_selection.y + y) >> 3) + table_selection.y) << 4);
			int pos = (editor_selection.x + x) % 8 + (((editor_selection.y + y) % 8) << 3);
			tile_update_pixel(&table_tiles[t], pos, value);
		}
	}
}

void comps_editor_update() {
	if (keys[SDL_SCANCODE_1]) editor_paint(0);
	if (keys[SDL_SCANCODE_2]) editor_paint(1);
	if (keys[SDL_SCANCODE_3]) editor_paint(2);
	if (keys[SDL_SCANCODE_4]) editor_paint(3);
	void selection_reset() {
		editor_selection.w = 1;
		editor_selection.h = 1;
	}
	if (keys_ctrl && keys[SDL_SCANCODE_D]) {
		selection_reset();
	}
	if (keys[SDL_SCANCODE_UP] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (editor_selection.y != 0) {
				editor_selection.y--;
				editor_selection.h++;
			}
		}
		else if (keys_ctrl) {
			if (editor_selection.h > 1) editor_selection.h--;
		}
		else {
			if (editor_selection.y != 0) editor_selection.y--;
		}
	}
	if (keys[SDL_SCANCODE_DOWN] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (editor_selection.y + editor_selection.h < table_selection.h << 3) {
				editor_selection.h++;
			}
		}
		else if (keys_ctrl) {
			if (editor_selection.h > 1) {
				editor_selection.y++;
				editor_selection.h--;
			}
		}
		else {	
			if (editor_selection.y + editor_selection.h < table_selection.h << 3) {
				editor_selection.y++;
			}
		}
	}
	if (keys[SDL_SCANCODE_LEFT] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (editor_selection.x != 0) {
				editor_selection.x--;
				editor_selection.w++;
			}
		}
		else if (keys_ctrl) {
			if (editor_selection.w > 1) editor_selection.w--;
		}
		else {	
			if (editor_selection.x != 0) editor_selection.x--;
		}
	}
	if (keys[SDL_SCANCODE_RIGHT] % key_repeat == 1) {
		if (keys_shift && !keys_ctrl) {
			if (editor_selection.x + editor_selection.w < table_selection.w << 3) {
				editor_selection.w++;
			}
		}
		else if (keys_ctrl) {
			if (editor_selection.w > 1) {
				editor_selection.x++;
				editor_selection.w--;
			}
		}
		else {
			if (editor_selection.x + editor_selection.w < table_selection.w << 3) {
				editor_selection.x++;
			}
		}
	}

}


