
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

	SDL_Rect dest = {
		table_selection.x + table_selection.w,
		table_selection.y + table_selection.h,
		abs(table_selection.w),
		abs(table_selection.h),
	};

	int x_off = comp_space.x + (comp_space.w - dest.w * ratio) / 2;
	int y_off = comp_space.y + (comp_space.h - dest.h * ratio) / 2;

	// SHOW TILE(S)
	for (int x = 0; x < dest.w; x++) {
		for (int y = 0; y < dest.h; y++) {
			SDL_RenderCopy(renderer, table_tiles[
				dest.x + x + (dest.y + y) * 16].texture,
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
		int w = (int) ((float) dest.w * ratio);
		int h = (int) ((float) dest.h * ratio);
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
		x_off + (int) ((float) editor_selection.x * pxl_ratio) - 1,
		y_off + (int) ((float) editor_selection.y * pxl_ratio),
		(int) ((float) editor_selection.w * pxl_ratio) + 2,
		(int) ((float) editor_selection.h * pxl_ratio),
	});
	SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
		x_off + (int) ((float) editor_selection.x * pxl_ratio),
		y_off + (int) ((float) editor_selection.y * pxl_ratio) - 1,
		(int) ((float) editor_selection.w * pxl_ratio),
		(int) ((float) editor_selection.h * pxl_ratio) + 2,
	});

}

void editor_paint() {
	for (int x = 0; x < editor_selection.w; x++) {
		for (int y = 0; y < editor_selection.h; y++) {
			int t = ((editor_selection.x + x) >> 3) + table_selection.x + 
				((((editor_selection.y + y) >> 3) + table_selection.y) << 4);
			int pos = (editor_selection.x + x) % 8 + (((editor_selection.y + y) % 8) << 3);
			tile_update_pixel(&table_tiles[t], pos, palette_current_color);
		}
	}
}

void comps_editor_update() {
	void selection_reset() {
		editor_selection.w = 1;
		editor_selection.h = 1;
	}
	if (keys[SDL_SCANCODE_SPACE]) {
		if (keys[SDL_SCANCODE_SPACE] == 1) {
			pixel_struct pxl = pixel_new();
			pxl.rect = (SDL_Rect) {
				table_selection.x << 3,
				table_selection.y << 3,
				table_selection.w << 3,
				table_selection.h << 3,
			};
			undo_record(pixel_state_capture(pxl));
		}
		editor_paint();
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


