#include "./generic.c"
#include "./window.c"
#include "./audio.c"
#include "./fps.c"
#include "./keyboard.c"
#include "./mouse.c"


void lib_event(SDL_Event event) {
	window_event(event);
	mouse_event(event);
}

void lib_update() {
	fps_update();
	keyboard_update();
	mouse_update();
	window_update();
}
