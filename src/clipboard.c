/*
	yance clipboard structure

	offset  meaning
	------  -------
	0-4     "YANCE"
	5       type ("p" = pixels)
	6-9     count of pixels in 16bit base16
	a-b     width of pixels in 8bit base16
	c-n     data in 4bit base16
	n+1     \0 (string null terminator)
*/
/*
	SDL2 clipboard support only works on string
	this could change in the future
	https://wiki.libsdl.org/CategoryClipboard

	this library converts raw memory into hex strings
	and the back again

	windows and mac have standardized methods for
	different media/data types, but linux can depend
	on the gui system
*/

void clipboard_pixels_copy(SDL_Rect src) {

	uint16_t size = src.w * src.h;
	printf("%d\n", size);

	char string[0xc + size + 1];
	char * pout = string;
	const char * hex = "0123456789abcdef";
	// identifier
	strcpy(string, "YANCEp");
	pout += 6;
	// total pixels in 16bit base16
	*pout++ = hex[(size >> 12) & 0xf];
	*pout++ = hex[(size >> 8) & 0xf];
	*pout++ = hex[(size >> 4) & 0xf];
	*pout++ = hex[size & 0xf];
	// width of selection in 8bit base16
	*pout++ = hex[((uint8_t) src.w >> 4) & 0xf];
	*pout++ = hex[(uint8_t) src.w & 0xf];
	// pixel data in 4bit base16
	for (int y = 0; y < src.h; y++) {
		for (int x = 0; x < src.w; x++) {
			*pout++ = hex[table_pixel_get_value(x, y) & 0xF];
		}
	}
	// null terminator
	*pout++ = '\0';
	printf("%s\n", string);
	SDL_SetClipboardText((const char *) &string);
}


void clipboard_pixels_paste(SDL_Rect dest) {
	char * string = SDL_GetClipboardText();
	// identifier
	const char * ident = "YANCEp";
	for (int i = 0; i < strlen(ident); i++) {
		if (string[i] != ident[i]) return;
	}
	// total pixels in 16bit base16
	char buff[5];
	memcpy(buff, &string[0x6], 4);
	buff[4] = '\0';
	int size = (int) strtol(buff, NULL, 16);
	printf("size: %d\n", size);
	// width of selection in 8bit base16
	memcpy(buff, &string[0xa], 2);
	buff[2] = '\0';
	int width = (int) strtol(buff, NULL, 16);
	printf("width: %d\n", width);
	// pixel data in 4bit base16
	int rows = size / width;
	buff[1] = '\0';
	int i = 0;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < width; x++) {
			memcpy(buff, &string[0xc + i], 1);
			int value = (int) strtol(buff, NULL, 16);
			table_pixel_set_value(x + dest.x, y + dest.y, value);
			i++;
		}
	}

	printf("%s\n", string);
	printf("%d\n", strlen(string));
	SDL_free(string);
}
