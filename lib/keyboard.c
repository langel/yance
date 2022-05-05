
int keys[256];
int keys_alt = 0;
int keys_ctrl = 0;
int keys_shift = 0;
int keys_pressed = 0;
const uint8_t * keyboard_state;

void keyboard_init() {
	for (int i = 0; i < 256; i++) keys[i] = 0;
	keyboard_state = SDL_GetKeyboardState(NULL);
}

void keyboard_update() {
	keys_pressed = 0;
	for (int i = 0; i < 256; i++) {
		if (keyboard_state[i]) {
			keys[i]++;
			keys_pressed++;
		}
		else keys[i] = 0;
	}
	keys_alt = keys[SDL_SCANCODE_LALT] || keys[SDL_SCANCODE_RALT];
	keys_ctrl = keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL];
	keys_shift = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
}

