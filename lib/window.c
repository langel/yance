int window_has_focus = 0;

SDL_Rect window_rect;
SDL_Window * window;

void window_init(char * name) {
	window = SDL_CreateWindow(name, window_rect.x, window_rect.y, window_rect.w, window_rect.h, SDL_WINDOW_RESIZABLE);
}

void window_event(SDL_Event event) {
	if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_MOVED) {
			window_rect.x = event.window.data1;
			window_rect.y = event.window.data2;
		}
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			window_rect.w = event.window.data1;
			window_rect.h = event.window.data2;
		}
	}
}

void window_update() {
	int flags = SDL_GetWindowFlags(window);
	window_has_focus = flags & SDL_WINDOW_INPUT_FOCUS;
}
