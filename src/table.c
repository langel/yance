/*
	table functions handle drawing rom data
*/

// texture must be large enough to hold 512kb 2bpp
// 16 x 2048 tiles or 128 x 16384 pixels  
// x 32bit color = 8,388,608 of GPU RAM

// XXX currently space for 256kb rom + header
//#define table_tiles_max 16385

#define table_tiles_max 2048 // 4 banks

tile_struct table_tiles[table_tiles_max];

SDL_Rect table_selection = { 0, 0, 1, 1 };


void table_init() {
	// allocation should be based on needs
	// XXX not on possibilities
	for (int i = 0; i < table_tiles_max; i++) {
		table_tiles[i].color_data = malloc(256);
		table_tiles[i].texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 8, 8);
		SDL_SetTextureBlendMode(table_tiles[i].texture, SDL_BLENDMODE_BLEND);
	}
}


void table_load(char * filename) {
	rom_load(filename);
	uint8_t sizteen_bytes[16];
	for (int t = 0; t < rom_tile_count; t++) {
		for (int i = 0; i < 16; i++) {
			sizteen_bytes[i] = rom_binary[(t << 4) + i];
		}
		_2bpp_to_tile(sizteen_bytes, &table_tiles[t]);
		/*
		_64px_to_surface(table_tiles[t], pixels, 
			32 + (t * 8) % 128 + (t % 16),
			32 + (t >> 4) * 8 + (t >> 4),
			128);
			*/
	}
}
