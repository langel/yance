

typedef struct {
	uint8_t pal;
	uint8_t values[64];
	uint32_t * color_data;
	SDL_Texture * texture;
} tile_struct;

SDL_Rect tile_rect = { 0, 0, 8, 8 };

void _2bpp_to_tile(uint8_t data[16], tile_struct * tile) {
	// data[] = 16 bytes of 2bpp bit planed data
	// returns array of 64 color index values
	tile->pal = 0;
	int pos = 0;
	for (uint8_t l = 0; l < 8; l++) {
		uint8_t lo = data[l];
		uint8_t hi = data[8+l];
		for (uint8_t bit = 0; bit < 8; bit++) {
			uint8_t b = 1 << (7 - bit);
			uint8_t color = (lo & b) ? 1 : 0;
			color += (hi & b) ? 2 : 0;
			tile->values[pos] = color;
			tile->color_data[pos] = colors[palette[tile->pal][color]];
			pos++;
		}
	}
	SDL_UpdateTexture(tile->texture, NULL, tile->color_data, 8 * 4);
}

void _64px_to_surface(tile_struct tile, uint32_t * surface, int x, int y, int w) {
	int offset = x + y * w;
	for (int i = 0; i < 64; i++) {
		surface[offset + (i & 7) + (i >> 3) * w] = colors[4 + 16 * tile.values[i]];
	}
}
