
typedef struct {
	int x;
	int y;
	int rel_x;
	int rel_y;
	int button_left;
	int button_middle;
	int button_right;
	int button_x1;
	int button_x2;
} mouse_struct;

mouse_struct mouse;

void mouse_init() {
	mouse = (mouse_struct) { 0 };
}

void mouse_update() {
	int prev_x = mouse.x;
	int prev_y = mouse.y;
	uint32_t buttons = SDL_GetGlobalMouseState(&mouse.x, &mouse.y);
	mouse.x -= window_rect.x;
	mouse.y -= window_rect.y;
	mouse.rel_x = mouse.x - prev_x;
	mouse.rel_y = mouse.y - prev_y;
	if (buttons & SDL_BUTTON_LMASK) mouse.button_left++;
	else mouse.button_left = 0;
	if (buttons & SDL_BUTTON_MMASK) mouse.button_middle++;
	else mouse.button_middle = 0;
	if (buttons & SDL_BUTTON_RMASK) mouse.button_right++;
	else mouse.button_right = 0;
	if (buttons & SDL_BUTTON_X1MASK) mouse.button_x1++;
	else mouse.button_x1 = 0;
	if (buttons & SDL_BUTTON_X2MASK) mouse.button_x2++;
	else mouse.button_x2 = 0;
}
