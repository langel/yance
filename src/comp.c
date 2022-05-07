
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
	if (keys_ctrl && keys[SDL_SCANCODE_S] == 1) {	
		printf("saveing\n");
		table_save("temp.chr");
	}
	if (keys_ctrl && keys[SDL_SCANCODE_1] == 1) {
		printf("opening source\n");
		SDL_OpenURL("https://github.com/langel/yance");
	}
	if (keys_ctrl && keys[SDL_SCANCODE_C] == 1) {
		unsigned char allchars[257] = {};
		uint8_t chars[257] = {};
		for (uint8_t i = 20; i < 100; i++) {
			chars[i] = i;
			printf("%c", i);
		}
		printf("\n");
		memcpy(allchars, chars, 256);
		printf("%s\n", allchars);
		printf("%d\n", strlen(allchars));
		printf("stashed in clipboard: %s\n", allchars);
		SDL_SetClipboardText(allchars);
	}
	if (keys_ctrl && keys[SDL_SCANCODE_V] == 1) {
		char * clipboard = SDL_GetClipboardText();
		printf("%s\n", clipboard);
		printf("%d\n", strlen(clipboard));
		SDL_free(clipboard);
	}
	if (comp_target == rom_table) comps_rom_table_update();
	if (comp_target == editor) comps_editor_update();
}
