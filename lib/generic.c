
int collision_detection(SDL_Rect a, SDL_Rect b) {
	if (a.x + a.w < b.x) return 0;
	if (a.x > b.x + b.w) return 0;
	if (a.y + a.h < b.y) return 0;
	if (a.y > b.y + b.h) return 0;
	return 1;
}

int rng8() {
	static uint8_t val = 1;
	int carry = val & 1;
	val >>= 1;
	if (carry) val ^= 0xd4;
	return (int) val;
}

void render_color_set(SDL_Renderer * renderer, uint32_t color) {
	SDL_SetRenderDrawColor(renderer,
		color >> 24, 
		color >> 16 & 0xff, 
		color >> 8 & 0xff,
		0xff);
}

SDL_Texture * texture_create_generic(SDL_Renderer * renderer, int w, int h) {
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return texture;
}
