
#include "./color.c"
#include "./palette.c"
#include "./sfx.c"
#include "./tile.c"
#include "./table.c"
#include "./ascii_9x16.c"
#include "./comp.c"

void all_init() {
	audio_init(32000, 2, 1024, AUDIO_F32SYS, &audio_callback);
	ascii_init();
	keyboard_init();
	mouse_init();
	comp_init();
	table_init();
}
