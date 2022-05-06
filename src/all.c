
#include "./color.c"
#include "./palette.c"
#include "./sfx.c"
#include "./tile.c"
#include "./rom.c"
#include "./table.c"
#include "./ascii_9x16.c"

SDL_Rect comp_space;
enum comp_targets {
	rom_table,
	editor,
};
enum comp_targets comp_target;
#include "./comps/editor.c"
#include "./comps/palettes.c"
#include "./comps/colors.c"
#include "./comps/rom_table.c"
#include "./comps/status_bar.c"
#include "./comp.c"

void all_init() {
	audio_init(32000, 2, 1024, AUDIO_F32SYS, &audio_callback);
	ascii_init();
	keyboard_init();
	mouse_init();
	comp_init();
	rom_init();
	table_init();
}
