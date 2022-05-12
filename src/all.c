
#include "./color.c"
#include "./palette.c"
#include "./sfx.c"
#include "./file.c"
#include "./tile.c"
#include "./table.c"
#include "./ascii_9x16.c"
#include "./pixel.c"
#include "./undo.c"
#include "./clipboard.c"
#include "./comp.c"

void src_all_init() {
	audio_init(32000, 2, 1024, AUDIO_F32SYS, &audio_callback);
	ascii_init();
	keyboard_init();
	mouse_init();
	comp_init();
	table_init();
	undo_init();
}

void src_event(SDL_Event event) {
	if (event.type == SDL_DROPFILE) {
		char * filename = event.drop.file;
		printf("%s\n", filename);
		table_load(filename);
		SDL_free(filename); 
		SDL_RaiseWindow(window);
	}
}
