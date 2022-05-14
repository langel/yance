
SDL_Rect comp_space;
enum comp_targets {
	rom_table,
	editor,
	manual,
};
enum comp_targets comp_target;


SDL_Point table_cursor;
SDL_Rect table_selection;
SDL_Point table_selection_origin;

SDL_Point editor_cursor;
SDL_Rect editor_selection;
SDL_Point editor_selection_origin;



#include "./comps/editor.c"
#include "./comps/palettes.c"
#include "./comps/colors.c"
#include "./comps/rom_table.c"
#include "./comps/status_bar.c"
#include "./comps/manual.c"


void comp_init() {
	comp_target = rom_table;
	SDL_Point point = { 0, 0 };
	SDL_Rect rect = { 0, 0, 1, 1 };
	table_cursor = point;
	table_selection = rect;
	table_selection_origin = point;
	editor_cursor = point;
	editor_selection = rect;
	editor_selection_origin = point;
	comps_manual_init();
}


void comp_render() {
	// clear background
	render_color_set(renderer, colors[71]);
	SDL_RenderFillRect(renderer, NULL);
	// setup comp space and render comps
	comp_space = (SDL_Rect) { 0, 0, window_rect.w, window_rect.h };
	comps_status_bar_render();
	comps_rom_table_render();
	comps_colors_render();
	comps_editor_render();
	if (comp_target == manual) {
		comps_manual_render();
	};
}


void comp_update() {

	if (keys[SDL_SCANCODE_F1] == 1) {
		comps_manual_set_as_target();
	}
	// quit
	//if (keys[SDL_SCANCODE_ESCAPE]) running = 0;
	if (keys_ctrl && keys[SDL_SCANCODE_Q]) running = 0;

	// tab around
	if (keys[SDL_SCANCODE_TAB] == 1) {
		comp_target++;
		if (comp_target > 1) comp_target = 0;
	}

	// save
	if (keys_ctrl && keys[SDL_SCANCODE_S] == 1) {	
		table_save();
	}

	// open github
	if (keys_ctrl && keys[SDL_SCANCODE_G] == 1) {
		printf("opening source\n");
		SDL_OpenURL("https://github.com/langel/yance");
	}

	// undo
	if (keys_ctrl && !keys_shift && keys[SDL_SCANCODE_Z] == 1) {
		undo_rewind();
	}

	// redo
	if (keys_ctrl && keys_shift && keys[SDL_SCANCODE_Z] == 1) {
		undo_redo();
	}

	SDL_Rect oprect;
	if (comp_target == rom_table) oprect = (SDL_Rect) {
		table_selection.x << 3,
		table_selection.y << 3,
		table_selection.w << 3,
		table_selection.h << 3,
	};
	if (comp_target == editor) oprect = (SDL_Rect) {
		(table_selection.x << 3) + editor_selection.x,
		(table_selection.y << 3) + editor_selection.y,
		editor_selection.w,
		editor_selection.h,
	};
	if (oprect.w) {
		// copy
		if (keys_ctrl && keys[SDL_SCANCODE_C] == 1) {
			clipboard_pixels_copy(oprect);
		}
		// cut
		if (keys_ctrl && keys[SDL_SCANCODE_X] == 1) {
			clipboard_pixels_cut(oprect);
		}
		// paste
		if (keys_ctrl && keys[SDL_SCANCODE_V] == 1) {
			clipboard_pixels_paste(oprect);
		}

		// flip
		if (keys[SDL_SCANCODE_F] == 1) {
			transform_flip(oprect);
		}
		// mirror
		if (keys[SDL_SCANCODE_M] == 1) {
			transform_mirror(oprect);
		}
		// rotate
		if (keys[SDL_SCANCODE_R] == 1) {
			transform_rotate(oprect);
		}
	}

	// handle palette stuff
	if (keys_ctrl) {
		// PALETTE CHANGE WHICH
		if (keys[SDL_SCANCODE_1] == 1) {
			palette_current_set(0);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_2] == 1) {
			palette_current_set(1);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_3] == 1) {
			palette_current_set(2);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_4] == 1) {
			palette_current_set(3);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_5] == 1) {
			palette_current_set(4);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_6] == 1) {
			palette_current_set(5);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_7] == 1) {
			palette_current_set(6);
			table_update_palette();
		}
		if (keys[SDL_SCANCODE_8] == 1) {
			palette_current_set(7);
			table_update_palette();
		}
		// CHANGE CURRENT COLOR VALUE
		int color_target = palette_current_color_id;
		if (keys[SDL_SCANCODE_RIGHT] % key_repeat == 1) 
			if ((color_target & 0x0f) < 0x0f) color_target++;
		if (keys[SDL_SCANCODE_LEFT] % key_repeat == 1) 
			if ((color_target & 0x0f) > 0x00) color_target--;
		if (keys[SDL_SCANCODE_UP] % key_repeat == 1) 
			if (color_target > 0x0f) color_target -= 16;
		if (keys[SDL_SCANCODE_DOWN] % key_repeat == 1) 
			if ((color_target & 0xf0) < 0x30) color_target += 16;
		if (color_target != palette_current_color_id) {
			if (color_target < 0x00) color_target = 0x00;
			if (color_target > 0x3f) color_target = 0x3f;
			palette_current_color_value_set(color_target);
			table_update_palette();
		}
	}
	else {
		// CHANGE CURRENT BRUSH COLOR
		if (keys[SDL_SCANCODE_1]) palette_current_color_set(0);
		if (keys[SDL_SCANCODE_2]) palette_current_color_set(1);
		if (keys[SDL_SCANCODE_3]) palette_current_color_set(2);
		if (keys[SDL_SCANCODE_4]) palette_current_color_set(3);
	}

	// handle other comps
	if (comp_target == rom_table) comps_rom_table_update();
	if (comp_target == editor) comps_editor_update();
	if (comp_target == manual) comps_manual_update();
}
