#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

int texture_w = 640;
int texture_h = 360;
SDL_Event event;
SDL_Renderer * renderer;

#include "lib/all.c"
#include "src/all.c"


int main(int argc, char * args[]) {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

	window_rect = (SDL_Rect) { 100, 200, texture_w*2, texture_h*2 };
	window_init("Yet Another NES CHR Editor");
	SDL_SetWindowMinimumSize(window, 600, 400);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	all_init();

	table_load("guntner.chr");
	int tile_size;



	//SDL_ShowCursor(SDL_DISABLE);


	int colors_x, colors_y;
	
	// STATUS BAR INIT
	char status_text[256];
	SDL_Rect status_rect = { 0, 0, window_rect.w, 16 };

	time_t fps_start = time(NULL);
	time_t fps_end;
	uint32_t fps_counter;
	float fps_current;

	float editor_x_ratio;
	float editor_y_ratio;

	uint32_t frame_counter;
	int running = 1;
	while (running) {
		frame_counter++;

		// clear background
		render_color_set(renderer, colors[71]);
		SDL_RenderFillRect(renderer, NULL);

		// SHOW TABLES
		tile_size = 24;
		tile_rect.w = tile_rect.h = tile_size;
		for (int i = 0; i < rom_tile_count; i++) {
			tile_rect.x = 10 + (i % 16) * tile_size;
			tile_rect.y = 10 + (i >> 4) * tile_size;
			SDL_RenderCopy(renderer, table_tiles[i].texture, NULL, &tile_rect);
		}
		render_color_set(renderer, colors[0x40]);
		SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
			8 + table_selection.x * tile_size,
			9 + table_selection.y * tile_size,
			table_selection.w * tile_size + 3,
			table_selection.h * tile_size + 1,
		});
		SDL_RenderDrawRect(renderer, &(SDL_Rect) { 
			9 + table_selection.x * tile_size,
			8 + table_selection.y * tile_size,
			table_selection.w * tile_size + 1,
			table_selection.h * tile_size + 3,
		});

		// SHOW TILE(S)
		editor_x_ratio = (float) 512 / (float) table_selection.w;
		editor_y_ratio = (float) 512 / (float) table_selection.h;
		for (int x = 0; x < table_selection.w; x++) {
			for (int y = 0; y < table_selection.h; y++) {
				SDL_RenderCopy(renderer, table_tiles[
					table_selection.x + x + (table_selection.y + y) * 16].texture,
					NULL, &(SDL_Rect) {
					8 + tile_size * 16 + 32 + (int) (editor_x_ratio * (float) x),
					32 + (int) (editor_y_ratio * (float) y),
					(int) editor_x_ratio,
					(int) editor_y_ratio,
				});
			}
		}


		// ALL COLORS
		colors_x = window_rect.w - 8 - 40 * 16;
		colors_y = window_rect.h - 8 - 16 - 40 * 4;
		for (int c = 0; c < 64; c++) {
			int xoff = (c % 16) * 40;
			int yoff = (c >> 4) * 40;
			render_color_set(renderer, colors[c]);
			SDL_RenderFillRect(renderer, &(SDL_Rect) { colors_x + xoff, colors_y + yoff, 40, 40 });
			ascii_color_set(colors[c] ^ 0xffffff0f);
			char hex[3];
			sprintf(hex, "%02X", c);
			ascii_text_render(hex, colors_x + 16 + xoff, colors_y + 4 + yoff);
		}

		// STATUS BAR
		// background
		render_color_set(renderer, colors[0x02]);
		status_rect.w = window_rect.w;
		status_rect.y = window_rect.h - 16;
		SDL_RenderFillRect(renderer, &status_rect);
		// text
		ascii_color_set(colors[0x41]);
		sprintf(status_text, " TILE COUNT: %5d     ROM SIZE: %7d bytes     NO HEADER     FPS: %7.3f", rom_tile_count, rom_tile_count * 16, fps_current);
		ascii_text_render(status_text, 0, window_rect.h - 16);

		// XXX need a dependable FPS throttler
		SDL_RenderPresent(renderer);

		// FPS
		fps_end = time(NULL);
		fps_counter++;
		fps_current = 1.f / ((float) (fps_end - fps_start) / (float) fps_counter);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					//printf( "keydown: %8s\n", SDL_GetKeyName( event.key.keysym.sym ) );
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
					}
					break;
			}
			window_event_process(event);
		}
		
		float x_ratio = (float) window_rect.w / (float) texture_w;
		float y_ratio = (float) window_rect.h / (float) texture_h;
		
		keyboard_update();
		mouse_update(window_rect);
		table_update();
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
