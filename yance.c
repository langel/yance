#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

int texture_w = 640;
int texture_h = 360;

/*
uint32_t colors[4] = {
	0x000000ff,
	0x555555ff,
	0xaaaaaaff,
	0xffffffff,
};
*/

#include "lib/all.c"
#include "src/color.c"
#include "src/tile.c"
#include "src/ascii_9x16.c"

void sfx_plot() {
	audio_amp = 0.1;
	audio_fade = 0.998;
	audio_hertz = ((float) rng8() + 420.0) / 32000.0;
	audio_bend = 0.9991;
}


int main(int argc, char * args[]) {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_Event event;

	audio_init(32000, 2, 1024, AUDIO_F32SYS, &audio_callback);
	window_rect = (SDL_Rect) { 100, 200, texture_w*2, texture_h*2 };
	window_init("Yet Another NES Character Editor");

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	uint32_t * pixels = malloc(texture_w * texture_h * 4);
	for (int i = 0; i < texture_w * texture_h; i++) {
		pixels[i] = 0x1f1f1fff;
	}

	FILE * file = fopen("guntner.chr", "rb");
	fseek(file, 0, SEEK_END);
	int file_length = ftell(file);
	printf("char rom size: %d\n", file_length);
	fseek(file, 0, SEEK_SET);
	char * buffer = malloc(file_length);
	fread(buffer, file_length, 1, file);
	fclose(file);

	int offset = texture_h * 10;
	int tilepage_offset = offset;
	int tile_count = file_length >> 4;
	printf("tile count: %d\n", tile_count);
	for (int i = 0; i < tile_count; i++) {
		if (i % 256 == 0) offset = tilepage_offset + 256 + (i >> 8) * 256;;
		int tile_x = (i % 16) * 8;
		int tile_y = ((i % 256) >> 4) * 8;
		for (int l = 0; l < 8; l++) {
			unsigned char lo = buffer[i*16+l];
			unsigned char hi = buffer[i*16+8+l];
			for (unsigned char bit = 0; bit < 8; bit++) {
				unsigned char b = 1 << bit;
				unsigned char color = (lo & b) ? 1 : 0;
				color += (hi & b) ? 2 : 0;
				int pos = tile_x + (7 - bit) + (tile_y + l) * texture_w;
				pixels[offset + pos] = colors[1 + 16 * color];
			}
		}
	}

	char status_text[80];
	sprintf(status_text, " TILE COUNT: %5d     ROM SIZE: %7d bytes     NO HEADER ", tile_count, file_length);
	printf(status_text);
	SDL_Texture * status_texture = texture_create_generic(renderer, 80 * 9, 16);
	ascii_text_render(renderer, status_texture, status_text);

	tile_struct tiles[tile_count];
	uint8_t sizteen_bytes[16];
	for (int t = 0; t < tile_count; t++) {
		for (int i = 0; i < 16; i++) {
			sizteen_bytes[i] = buffer[(t << 4) + i];
		}
		tiles[t] = _2bpp_to_64px(sizteen_bytes);
		_64px_to_surface(tiles[t], pixels, 
			32 + (t * 8) % 128 + (t % 16),
			32 + (t >> 4) * 8 + (t >> 4),
			texture_w);
	}

	for (int c = 0; c < 64; c++) {
		int xoff = (c % 16) * 24;
		int yoff = (c >> 4) * 24;
		for (int x = 0; x < 24; x++) {
			for (int y = 0; y < 24; y++) {
				pixels[200 + xoff + x + (100 + yoff + y) * texture_w] = colors[c];
			}
		}
	}

	free(buffer);

	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_w, texture_h);


	ascii_init(pixels, renderer);
	keyboard_init();
	mouse_init();
	//SDL_ShowCursor(SDL_DISABLE);

	int paint_color = colors[37];

	int running = 1;
	while (running) {

		SDL_UpdateTexture(texture, NULL, pixels, texture_w * 4);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		ascii_rect.y = window_rect.h - 32;
		SDL_RenderCopy(renderer, ascii_texture, NULL, &ascii_rect);
		SDL_RenderPresent(renderer);

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
		if ((mouse.button_left == 1 || (mouse.button_left && (mouse.rel_x != 0 || mouse.rel_y != 0)))
		&& mouse.x >= 0 && mouse.x < (int) ((float) texture_w * x_ratio)
		&& mouse.y >= 0 && mouse.y < (int) ((float) texture_h * y_ratio)) {
			int pixel = (int) ((float) mouse.x / x_ratio) + texture_w * (int) ((float) mouse.y / y_ratio);
			pixels[pixel] = paint_color;
			pixel++;
			pixels[pixel] = paint_color;
			pixel += texture_w;
			pixels[pixel] = paint_color;
			pixel--;
			pixels[pixel] = paint_color;
			sfx_plot();
		}

	}

	SDL_Quit();
	return 0;
}
