#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

int running = 1;
int texture_w = 640;
int texture_h = 360;
int key_repeat = 5;
SDL_Event event;
SDL_Renderer * renderer;

#include "lib/all.c"
#include "src/all.c"


int main(int argc, char * args[]) {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

	window_rect = (SDL_Rect) { 100, 200, texture_w*2, texture_h*2 };
	window_init("Yet Another NES CHR Editor");
	SDL_SetWindowMinimumSize(window, 640, 480);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	
	// color is better than white on bootup
	render_color_set(renderer, colors[71]);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderPresent(renderer);

	all_init();

	table_load("guntner.chr");
	//table_load("milon.nes");
	//table_load("punchout.nes");

//	SDL_ShowCursor(SDL_DISABLE);

	while (running) {
		fps_update();

		// clear background
		render_color_set(renderer, colors[71]);
		SDL_RenderFillRect(renderer, NULL);

		comp_render();

		// XXX need a dependable FPS throttler
		SDL_RenderPresent(renderer);

		// USER INPUTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
			}
			window_event_process(event);
		}
		keyboard_update();
		mouse_update(window_rect);

		comp_update();

		float x_ratio = (float) window_rect.w / (float) texture_w;
		float y_ratio = (float) window_rect.h / (float) texture_h;
		if ((mouse.button_left == 1 || (mouse.button_left && (mouse.rel_x != 0 || mouse.rel_y != 0)))
		&& mouse.x >= 0 && mouse.x < (int) ((float) texture_w * x_ratio)
		&& mouse.y >= 0 && mouse.y < (int) ((float) texture_h * y_ratio)) {
			/*
			int pixel = (int) ((float) mouse.x / x_ratio) + texture_w * (int) ((float) mouse.y / y_ratio);
			pixels[pixel] = paint_color;
			pixel++;
			pixels[pixel] = paint_color;
			pixel += texture_w;
			pixels[pixel] = paint_color;
			pixel--;
			pixels[pixel] = paint_color;
			*/
			sfx_plot();
		}

	}

	SDL_Quit();
	return 0;
}
