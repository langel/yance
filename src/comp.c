
SDL_Rect comp_space;
enum comp_targets {
	rom_table,
	editor,
};
enum comp_targets comp_target;

SDL_Rect table_cursor = { 0, 0, 1, 1 };
SDL_Rect table_selection = { 0, 0, 1, 1 };
SDL_Rect editor_cursor = { 0, 0, 1, 1 };
SDL_Rect editor_selection = { 0, 0, 1, 1 };



#include "./comps/editor.c"
#include "./comps/palettes.c"
#include "./comps/colors.c"
#include "./comps/rom_table.c"
#include "./comps/status_bar.c"


void comp_init() {
	comp_target = rom_table;
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
}

void comp_update() {
	if (keys[SDL_SCANCODE_ESCAPE]) running = 0;
	if (keys[SDL_SCANCODE_TAB] == 1) {
		comp_target++;
		if (comp_target > 1) comp_target = 0;
	}
	if (keys_ctrl && keys[SDL_SCANCODE_S] == 1) {	
		printf("saveing\n");
		table_save("temp.chr");
	}
	if (keys_ctrl && keys[SDL_SCANCODE_1] == 1) {
		printf("opening source\n");
		SDL_OpenURL("https://github.com/langel/yance");
	}
	if (keys_ctrl && keys[SDL_SCANCODE_C] == 1) {
		if (comp_target == rom_table) {
			clipboard_pixels_copy((SDL_Rect) { 
				table_selection.x << 3,
				table_selection.y << 3,
				table_selection.w << 3,
				table_selection.h << 3,
			});
		}
		if (comp_target == editor) {
			clipboard_pixels_copy((SDL_Rect) {
				(table_selection.x << 3) + editor_selection.x,
				(table_selection.y << 3) + editor_selection.y,
				editor_selection.w,
				editor_selection.h,
			});
		}
	}
	if (keys_ctrl && keys[SDL_SCANCODE_V] == 1) {
		if (comp_target == rom_table) {
			clipboard_pixels_paste((SDL_Rect) {
				table_selection.x << 3, 
				table_selection.y << 3,
				0, 0 });
		}
		if (comp_target == editor) {
			clipboard_pixels_paste((SDL_Rect) {
				(table_selection.x << 3) + editor_selection.x,
				(table_selection.y << 3) + editor_selection.y,
				0, 0 });
		}
	}
	if (comp_target == rom_table) comps_rom_table_update();
	if (comp_target == editor) comps_editor_update();
}
