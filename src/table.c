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

char * rom_binary;
int rom_tile_count;



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
	// actually load
	FILE * file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);
	int file_length = ftell(file);
	rom_tile_count = file_length >> 4;
	fseek(file, 0, SEEK_SET);
	rom_binary = malloc(file_length);
	fread(rom_binary, file_length, 1, file);
	fclose(file);
	// convert data
	uint8_t sizteen_bytes[16];
	for (int t = 0; t < rom_tile_count; t++) {
		for (int i = 0; i < 16; i++) {
			sizteen_bytes[i] = rom_binary[(t << 4) + i];
		}
		_2bpp_to_tile(sizteen_bytes, &table_tiles[t]);
	}
	// cleanup
	free(rom_binary);
}

void table_save(char * filename) {
	// setup
	int size = rom_tile_count * 16;
	rom_binary = malloc(size);
	printf("malloc: %d\n", rom_tile_count * 16);
	// convert data
	uint8_t sizteen_bytes[16];
	for (int t = 0; t < rom_tile_count; t++) {
		_tile_to_2bpp(&table_tiles[t], sizteen_bytes);
		for (int i = 0; i < 16; i++) {
			rom_binary[(t << 4) + i] = sizteen_bytes[i]; 
		}
	}
	// save
	FILE * file = fopen(filename, "wb");
	printf(" %d byte\n", size);
	fwrite(rom_binary, size, 1, file); 
	fclose(file);
	// cleanup
	free(rom_binary);
}

uint8_t table_pixel_get_value(int x, int y) {
	int tile = (x >> 3) + ((y >> 3) << 4);
	int pos = (x % 8) + ((y % 8) << 3);
	return table_tiles[tile].values[pos];
}

void table_pixel_set_value(int x, int y, int value) {
	int tile = (x >> 3) + ((y >> 3) << 4);
	int pos = (x % 8) + ((y % 8) << 3);
	tile_update_pixel(&table_tiles[tile], pos, (uint8_t) value);
}
	


