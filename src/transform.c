

void transform_flip(SDL_Rect src) {
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	undo_record(pixel_state_capture(pxl));

	pxl.size = pxl.rect.w * pxl.rect.h;
	free(pxl.values);
	pxl.values = malloc(pxl.size);

	for (int x = 0; x < pxl.rect.w; x++) {
		for (int y = 0; y < pxl.rect.h; y++) {
			pxl.values[x + y * pxl.rect.w] = table_pixel_get_value(pxl.rect.x + x, pxl.rect.y + pxl.rect.h - y - 1); 
		}
	}

	pixel_state_plot(pxl);
}

void transform_mirror(SDL_Rect src) {
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	undo_record(pixel_state_capture(pxl));

	pxl.size = pxl.rect.w * pxl.rect.h;
	free(pxl.values);
	pxl.values = malloc(pxl.size);

	for (int x = 0; x < pxl.rect.w; x++) {
		for (int y = 0; y < pxl.rect.h; y++) {
			pxl.values[x + y * pxl.rect.w] = table_pixel_get_value(pxl.rect.x + pxl.rect.w - x - 1, pxl.rect.y + y); 
		}
	}

	pixel_state_plot(pxl);
}

void transform_rotate(SDL_Rect src) {
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	undo_record(pixel_state_capture(pxl));

	pxl.size = pxl.rect.w * pxl.rect.h;
	free(pxl.values);
	pxl.values = malloc(pxl.size);

	// transpose
	for (int x = 0; x < pxl.rect.w; x++) {
		for (int y = 0; y < pxl.rect.h; y++) {
			int pos = y + x * pxl.rect.h;
			if (pos < 0 || pos >= pxl.size) pxl.values[x + y * pxl.rect.w] = 0;
			else {
				int xx = pxl.rect.x + pxl.rect.h - y - 1; 
				int yy = pxl.rect.y + x;
				pxl.values[x + y * pxl.rect.w] = table_pixel_get_value(xx, yy);
			}
		}
	}
	pixel_state_plot(pxl);
}
