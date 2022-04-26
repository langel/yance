#include <stdio.h>
#include <SDL2/SDL.h>

int texture_w = 640;
int texture_h = 360;

uint32_t colors[4] = {
	0x000000ff,
	0x555555ff,
	0xaaaaaaff,
	0xffffffff,
};


typedef struct {
	int pal;
	int px[64];
} tile_array;


tile_array _2bpp_to_64px(uint8_t data[]) {
	// data[] = 16 bytes of 2bpp bit planed data
	// returns array of 64 color index values
	tile_array tile;
	int pos = 0;
	for (uint8_t l = 0; l < 8; l++) {
		uint8_t lo = data[l];
		uint8_t hi = data[8+l];
		for (uint8_t bit = 0; bit < 8; bit++) {
			uint8_t b = 1 << bit;
			uint8_t color = (lo & b) ? 1 : 0;
			color += (hi & b) ? 2 : 0;
			tile.px[pos] = color;
			pos++;
		}
	}
	return tile;
}



int main(int argc, char * args[]) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Event event;
	SDL_Window * window = SDL_CreateWindow("Yet Another NES Character Editor",
		100, 200, texture_w*2, texture_h*2, SDL_WINDOW_RESIZABLE);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 
		SDL_RENDERER_PRESENTVSYNC);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	uint32_t * pixels = malloc(texture_w * texture_h * 4);

	FILE * file = fopen("guntner.chr", "rb");
	fseek(file, 0, SEEK_END);
	int file_length = ftell(file);
	printf("rom size: %d\n", file_length);
	fseek(file, 0, SEEK_SET);
	char * buffer = malloc(file_length);
	fread(buffer, file_length, 1, file);
	fclose(file);
	// garble across
	for (int i = 0; i < file_length; i++) {
		unsigned char byte = buffer[i];
		for (unsigned char x = 0; x < 8; x++) {
			if ((1 << (7 - x)) & byte) {
				pixels[i * 8 + x] = 0xffffffff;
			}
		}
	}
	// more normal?
	int offset = file_length * 8 + texture_h * 10;
	for (int i = 0; i < file_length; i++) {
		unsigned char byte = buffer[i];
		pixels[offset+0] = colors[(byte & 0b11000000) >> 6];
		pixels[offset+1] = colors[(byte & 0b00110000) >> 4];
		pixels[offset+2] = colors[(byte & 0b00001100) >> 2];
		pixels[offset+3] = colors[(byte & 0b00000011) >> 0];
		offset += 4;
	}
	// more normaller?
	offset = file_length * 16;
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
				pixels[offset + pos] = colors[color];
			}
		}
	}

	int lower = texture_w * 300;
	pixels[lower + 10] = colors[3];
	pixels[lower + 11] = colors[3];
	pixels[lower + 12] = colors[3];
	pixels[lower + 13] = colors[2];
	pixels[lower + 14] = colors[2];
	pixels[lower + 15] = colors[2];
	pixels[lower + 16] = colors[1];
	pixels[lower + 17] = colors[1];
	pixels[lower + 18] = colors[1];

	uint8_t sizteen_bytes[16];
	for (int i = 0; i < 16; i++) {
		sizteen_bytes[i] = buffer[i];
	}
	tile_array tile = _2bpp_to_64px(sizteen_bytes);
	for (int i = 0; i < 64; i++) {
		pixels[340 * texture_w + i] = colors[tile.px[i]];
	}

	free(buffer);

	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_w, texture_h);

	int running = 1;
	while (running) {

		SDL_UpdateTexture(texture, NULL, pixels, texture_w * 4);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
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
		}
	}

	SDL_Quit();
	return 0;
}
