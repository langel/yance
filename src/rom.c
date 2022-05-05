/*
	rom functions responsible for raw graphical data
*/

char * rom_binary;
int rom_tile_count;

void rom_init() {
	// allocate enough memory for a 512kb ROM
	// thats 32,768 tiles
	int length = 
		// 524,288 bytes
		16 *	// 16 bytes per tile
		256 *	// 256 tiles per pattern table
		2 *	// 2 pattern tables per bank
		64;	// 64 max banks
	rom_binary = malloc(length);
}

void rom_load(char * filename) {
	FILE * file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);
	int file_length = ftell(file);
	rom_tile_count = file_length >> 4;
	fseek(file, 0, SEEK_SET);
	rom_binary = malloc(file_length);
	fread(rom_binary, file_length, 1, file);
	fclose(file);
}

void rom_save(char * filename) {
	// XXX hopefully some day!
}
