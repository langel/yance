
SDL_Surface * bmp_get_surface(const char * filename) {
	return SDL_LoadBMP(filename);
}

uint32_t bmp_get_pixel(SDL_Surface * surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	uint8_t * ptr = (uint8_t*)surface->pixels + y * surface->pitch + x * bpp;
	if (bpp == 1) return *ptr;
	if (bpp == 2) return *(uint16_t*)ptr;
	if (bpp == 3) {
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			return ptr[0] << 16 | ptr[1] << 8 | ptr[2];
		}
		else return ptr[0] | ptr[1] << 8 | ptr[2] << 16;
	}
	if (bpp == 4) return *(uint32_t*)ptr;
	return 0;
}

SDL_Color bmp_get_color(SDL_Surface * surface, int x, int y) {
	SDL_Color color;
	uint32_t pixel = bmp_get_pixel(surface, x, y);
	SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
	return color;
}

