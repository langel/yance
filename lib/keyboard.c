
char keys[128];
char keys_pressed = 0;
const uint8_t * keyboard_state;

void keyboard_init() {
	for (int i = 0; i < 128; i++) keys[i] = 0;
	keyboard_state = SDL_GetKeyboardState(NULL);
}

void keyboard_update() {
	keys_pressed = 0;
	for (int i = 0; i < 128; i++) {
		if (keyboard_state[i]) {
			keys[i]++;
			keys_pressed++;
		}
		else keys[i] = 0;
	}
}

