
uint8_t palette[8][4] = {
	{ 0x02, 0x12, 0x22, 0x32 },
	{ 0x03, 0x13, 0x23, 0x33 },
	{ 0x04, 0x14, 0x24, 0x34 },
	{ 0x05, 0x15, 0x25, 0x35 },
	{ 0x07, 0x17, 0x27, 0x37 },
	{ 0x08, 0x18, 0x28, 0x38 },
	{ 0x0a, 0x1a, 0x2a, 0x3a },
	{ 0x0c, 0x1c, 0x2c, 0x3c },
};

int palette_current = 0;
int palette_current_color = 0;
int palette_current_color_id = 0;

// sets which palette is in use
void palette_current_set(int i) {
	palette_current = i;
	palette_current_color_id = palette[palette_current][palette_current_color];
}

// sets which palette color is in use
void palette_current_color_set(int i) {
	palette_current_color = i;
	palette_current_color_id = palette[palette_current][palette_current_color];
}
