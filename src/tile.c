




typedef struct {
	int pal;
	int px[64];
} tile_struct;


tile_struct _2bpp_to_64px(uint8_t data[]) {
	// data[] = 16 bytes of 2bpp bit planed data
	// returns array of 64 color index values
	tile_struct tile;
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


