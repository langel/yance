

uint8_t * header;
char header_str[16];

enum file_types {
	unknown,
	iNES,
	NES2,
};
enum file_types file_type;

char * file_filename;
int file_has_trainer;
int file_prg_rom_size;
int file_chr_rom_size;
int file_chr_rom_start;

void file_load(char * filename) {
	file_filename = malloc(strlen(filename));
	strcpy(file_filename, filename);
	FILE * file = fopen(filename, "rb");
	header = malloc(16);
	fread(header, 16, 1, file);
	// zero out file properties 
	file_type = unknown;
	file_has_trainer = 0;
	file_prg_rom_size = 0;
	file_chr_rom_size = 0;
	file_chr_rom_start = 0;
	strcpy(header_str, "NO HEADER");
	if (header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1A) {
		file_type = iNES;
		strcpy(header_str, "iNES");
		// byte 4 PRG ROM data (16384 * x bytes)
		file_prg_rom_size = header[4] << 14;
		// byte 5 CHR ROM data, if present (8192 * y bytes)
		file_chr_rom_size = header[5] << 13;
		// byte 6 has misc but trainer important
		if (header[6] & 0x04) {
			file_has_trainer = 512;
		}
		// byte 7 has misc and also NES2 flag
		if ((header[7] & 0x0C) == 0x08) {
			file_type = NES2;
			strcpy(header_str, "NES2.0");
		}
		file_chr_rom_start = 16 + file_has_trainer;
		if (file_chr_rom_size) {
			file_chr_rom_start += file_prg_rom_size;
		}
		else {
			file_chr_rom_size = file_prg_rom_size;
		}
	}
	else {
		fseek(file, 0, SEEK_END);
		file_chr_rom_size = ftell(file);
	}
	fclose(file);
	free(header);
}
