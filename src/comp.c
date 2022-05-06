
SDL_Rect comp_space;
enum comp_targets {
	rom_table,
	editor,
};
enum comp_targets comp_target;

SDL_Rect table_selection = { 0, 0, 1, 1 };
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
	if (comp_target == rom_table) comps_rom_table_update();
	if (comp_target == editor) comps_editor_update();
}
