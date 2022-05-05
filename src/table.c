/*
	table functions handle drawing rom data
*/

// texture must be large enough to hold 512kb 2bpp
// 16 x 2048 tiles or 128 x 16384 pixels  
// x 32bit color = 8,388,608 of GPU RAM
SDL_Texture * table_texture;

void table_init() {
	table_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 128, 2048);
}

void table_load(char * filename) {
	rom_load(filename);
	tile_struct tiles[rom_tile_count];
	//uint32_t pixels[128 * 16384 * 4];
	uint32_t pixels[rom_tile_count * 64 * 4];
	uint8_t sizteen_bytes[16];
	return;
	for (int t = 0; t < rom_tile_count; t++) {
		for (int i = 0; i < 16; i++) {
			sizteen_bytes[i] = rom_binary[(t << 4) + i];
		}
		tiles[t] = _2bpp_to_64px(sizteen_bytes);
		_64px_to_surface(tiles[t], pixels, 
			32 + (t * 8) % 128 + (t % 16),
			32 + (t >> 4) * 8 + (t >> 4),
			texture_w);
	}
	SDL_UpdateTexture(table_texture, NULL, pixels, 128 * 4);
}
