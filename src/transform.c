

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


void transform_translate(SDL_Rect src, int dir) {
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	undo_record(pixel_state_capture(pxl));
	pxl.size = pxl.rect.w * pxl.rect.h;
	free(pxl.values);
	pxl.values = malloc(pxl.size);
	for (int x = 0; x < pxl.rect.w; x++) {
		for (int y = 0; y < pxl.rect.h; y++) {
			int val = 0;
			int xx = pxl.rect.x + x;
			int yy = pxl.rect.y + y;
			// right
			if (dir == 0) {
				if (x == 0) val = 0;
				else val = table_pixel_get_value(xx - 1, yy);
			}
			// up
			if (dir == 1) {
				if (y == pxl.rect.h - 1) val = 0;
				else val = table_pixel_get_value(xx, yy + 1);
			}
			// left
			if (dir == 2) {
				if (x == pxl.rect.w - 1) val = 0;
				else val = table_pixel_get_value(xx + 1, yy);
			}
			// down
			if (dir == 3) {
				if (y == 0) val = 0;
				else val = table_pixel_get_value(xx, yy - 1);
			}
			pxl.values[x + y * pxl.rect.w] = val;
		}
	}
	pixel_state_plot(pxl);
}


void transform_rotate(SDL_Rect src, int cw) {
	// square it off
	if (src.w > src.h) src.w = src.h;
	if (src.h > src.w) src.h = src.w;
	// setup data
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	pxl.size = pxl.rect.w * pxl.rect.h;
	free(pxl.values);
	pxl.values = malloc(pxl.size);
	// record undo step	
	undo_record(pixel_state_capture(pxl));
	// rotate
	for (int x = 0; x < pxl.rect.w; x++) {
		for (int y = 0; y < pxl.rect.h; y++) {
			int xx;
			int yy;
			int pos = y + x * pxl.rect.h;
			if (pos < 0 || pos >= pxl.size) pxl.values[x + y * pxl.rect.w] = 0;
			else {
				if (cw) {
					xx = pxl.rect.x + y; 
					yy = pxl.rect.y + pxl.rect.w - x - 1;
				}
				else {
					xx = pxl.rect.x + pxl.rect.h - y - 1; 
					yy = pxl.rect.y + x;
				}
				pxl.values[x + y * pxl.rect.w] = table_pixel_get_value(xx, yy);
			}
		}
	}
	pixel_state_plot(pxl);
}
