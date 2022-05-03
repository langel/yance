#include <stdio.h>
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

#include "lib/mouse.c"
#include "src/color.c"
#include "src/tile.c"



int main(int argc, char * args[]) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Event event;
	SDL_Window * window = SDL_CreateWindow("Yet Another NES Character Editor",
		100, 200, texture_w*2, texture_h*2, SDL_WINDOW_RESIZABLE);
	SDL_Rect window_rect = { 100, 200, texture_w*2, texture_h*2 };
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 
		SDL_RENDERER_PRESENTVSYNC);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	uint32_t * pixels = malloc(texture_w * texture_h * 4);
	for (int i = 0; i < texture_w * texture_h; i++) {
		pixels[i] = 0x1f1f1fff;
	}

	FILE * font = fopen("src/8x16font.bin", "rb");
	fseek(font, 0, SEEK_END);
	int font_length = ftell(font);
	fseek(font, 0, SEEK_SET);
	char * font_set = malloc(font_length);
	fread(font_set, font_length, 1, font);
	fclose(font);
	for (int i = 0; i < 256; i++) {
		for (int l = 0; l < 16; l++) {
			unsigned char byte = font_set[i*16+l];
			for (unsigned char bit = 0; bit < 8; bit++) {
				unsigned char b = 1 << bit;
				unsigned char color = (b & byte) ? 0x0d : 21;
				int pos = 240+ (i % 32) * 8 + bit + ((i >> 5) * 16 + l + 220) * texture_w;
				pixels[pos] = colors[color];
			}
		}
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

	// mouse init
	mouse_data mouse = mouse_init();
	//SDL_ShowCursor(SDL_DISABLE);
	int paint_color = colors[37];

	int running = 1;
	while (running) {

		SDL_UpdateTexture(texture, NULL, pixels, texture_w * 4);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		// mouse updates
		mouse_process(&mouse, &window_rect);
		if (mouse.button_left) {
			int pixel = mouse.x / 2 + texture_w * (mouse.y / 2);
			pixels[pixel] = paint_color;
			pixel++;
			pixels[pixel] = paint_color;
			pixel += texture_w;
			pixels[pixel] = paint_color;
			pixel--;
			pixels[pixel] = paint_color;
		}

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
		}
	}

	SDL_Quit();
	return 0;
}
