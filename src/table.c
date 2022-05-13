/*
	table functions handle drawing rom data
*/

// texture must be large enough to hold 512kb 2bpp
// 16 x 2048 tiles or 128 x 16384 pixels  
// x 32bit color = 8,388,608 of GPU RAM

//#define table_tiles_max 2048 // 4 banks
// space for 256kb rom + header
//#define table_tiles_max 16385
// space for 4mb ROM
//#define table_tiles_max 262144

// how about as much space as we could possibly imagine
#define table_tiles_max 1000000


tile_struct table_tiles[table_tiles_max];

char * rom_binary;
int rom_tile_count;



void table_init() {
	for (int i = 0; i < table_tiles_max; i++) {
		table_tiles[i].color_data = NULL;
	}
}


void table_update_palette() {
	for (int t = 0; t < rom_tile_count; t++) {
		for (int i = 0; i < 64; i++) {
			table_tiles[t].color_data[i] = palette_rgb_get(palette_current, table_tiles[t].values[i]);
		}
		tile_update_texture(&table_tiles[t]);
	}
}


void table_load(char * filename) {
	// checakout header
	file_load(filename);
	free(window_title);
	window_title = calloc(strlen(app_title) + 3 + strlen(filename + 1), 1);
	strcat(window_title, app_title);
	strcat(window_title, " - ");
	strcat(window_title, filename);
	SDL_SetWindowTitle(window, window_title);
	// actually load
	FILE * file = fopen(filename, "rb");
	rom_tile_count = file_chr_rom_size >> 4;
	fseek(file, file_chr_rom_start, SEEK_SET);
	rom_binary = malloc(file_chr_rom_size);
	fread(rom_binary, file_chr_rom_size, 1, file);
	fclose(file);
	// initialize required tiles 
	for (int t = 0; t < rom_tile_count; t++) {
		tile_initialize(&table_tiles[t]);
	}
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

void table_save() {
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
	// XXX this won't work if they rename the file while editing
	FILE * file = fopen(file_filename, "r+b");
	printf(" %d byte\n", size);
	fseek(file, file_chr_rom_start, SEEK_SET);
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
	tile_update_pixel(&table_tiles[tile], pos, value);
}
	


