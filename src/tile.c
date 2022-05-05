

typedef struct {
	uint8_t pal;
	uint8_t px[64];
	SDL_Texture * texture;
} tile_struct;


tile_struct _2bpp_to_64px(uint8_t data[16]) {
	// data[] = 16 bytes of 2bpp bit planed data
	// returns array of 64 color index values
	tile_struct tile;
	int pos = 0;
	for (uint8_t l = 0; l < 8; l++) {
		uint8_t lo = data[l];
		uint8_t hi = data[8+l];
		for (uint8_t bit = 0; bit < 8; bit++) {
			uint8_t b = 1 << (7 - bit);
			uint8_t color = (lo & b) ? 1 : 0;
			color += (hi & b) ? 2 : 0;
			tile.px[pos] = color;
			pos++;
		}
	}
	return tile;
}

void _64px_to_surface(tile_struct tile, uint32_t * surface, int x, int y, int w) {
	int offset = x + y * w;
	for (int i = 0; i < 64; i++) {
		surface[offset + (i & 7) + (i >> 3) * w] = colors[4 + 16 * tile.px[i]];
	}
}
