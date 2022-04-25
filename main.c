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
		pixels[offset+0] = colors[(byte & 0b1100000) >> 6];
		pixels[offset+1] = colors[(byte & 0b0011000) >> 4];
		pixels[offset+2] = colors[(byte & 0b0001100) >> 2];
		pixels[offset+3] = colors[(byte & 0b0000011) >> 0];
		offset += 4;
	}
	// more normaller?
	offset = file_length * 16;
	int tile_count = file_length >> 4;
	printf("tile count: %d\n", tile_count);
	for (int i = 0; i < file_length; i++) {
		unsigned char lo = buffer[i];
		unsigned char hi = buffer[i+8];
		for (unsigned char bit = 0; bit < 8; bit++) {
			unsigned char b = 1 << bit;
			unsigned char color = (lo & b) ? 1 : 0;
			color += (hi & b) ? 2 : 0;
			pixels[offset] = colors[color];
			offset++;
		}
	}
	free(buffer);

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

	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_w, texture_h);

	SDL_UpdateTexture(texture, NULL, pixels, texture_w * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	int running = 1;
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					printf( "keydown: %8s\n", SDL_GetKeyName( event.key.keysym.sym ) );
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
