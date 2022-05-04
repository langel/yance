
#include "./8x16font.c"

SDL_Texture * ascii_texture;
SDL_Rect ascii_rect;

void ascii_init(uint32_t * pixels, SDL_Renderer * renderer) {
	unsigned char char_w = 9;
	unsigned char char_h = 16;
	int temp_w = 256 * 9;
	ascii_rect = (SDL_Rect) { 0, 0, temp_w, char_h };
	uint32_t * temp = malloc(temp_w * char_h * 4);
	memset(temp, 0, sizeof(&temp));
	for (int i = 0; i < 256; i++) {
		for (int l = 0; l < char_h; l++) {
			unsigned char byte = __8x16font_bin[i * char_h + l];
			for (uint16_t bit = 0; bit < char_w; bit++) {
				// handling 9 pixel wide fonts defined here:
				// https://en.wikipedia.org/wiki/VGA_text_mode#Fonts
				uint16_t b = 1 << bit;
				// only extend font if certain range (box characters)
				if (bit >= 8) {
					if (i >= 0xc0 && i <= 0xdf) b >>= 1;
					else b = byte = 0xff;
				}
				// XXX if true place pixel
				int color = ((unsigned char) b & byte) ? 0x0f : 21;
				int pos = 240 + (i % 32) * char_w + bit + ((i >> 5) * char_h + l + 220) * texture_w;
				pixels[pos] = colors[color];
				// create reference texture
				if (!((unsigned char) b & byte)) {
					int pixel = i * char_w + bit + l * temp_w;
					temp[pixel] = 0xffffffff;
				}
			}
		}
	}
	ascii_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, temp_w, char_h);
	SDL_SetTextureBlendMode(ascii_texture, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(ascii_texture, NULL, temp, temp_w * 4);
}

void ascii_color_set(uint32_t color_value) {
	SDL_SetTextureColorMod(ascii_texture, 
		color_value >> 24, 
		color_value >> 16 & 0xff, 
		color_value >> 8 & 0xff);
}

void ascii_text_render(SDL_Renderer * renderer, char * text, int x, int y) {
	int length = strlen(text);
	SDL_Rect src = { 0, 0, 9, 16 };
	SDL_Rect dest = { x, y, 9, 16 };
	for (int i = 0; i < length; i++) {
		int id = (int) text[i];
		src.x = id * 9;
		dest.x = i * 9 + x;
		SDL_RenderCopy(renderer, ascii_texture, &src, &dest);
	}

}
