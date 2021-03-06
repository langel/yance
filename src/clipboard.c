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
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	SDL_SetClipboardText(pixel_state_capture(pxl));
}


void clipboard_pixels_cut(SDL_Rect src) {
	pixel_struct pxl = pixel_new();
	pxl.rect = src;
	undo_record(pixel_state_capture(pxl));
	SDL_SetClipboardText(pixel_state_capture(pxl));
	pxl.size = pxl.rect.w * pxl.rect.h;
	free(pxl.values);
	pxl.values = malloc(pxl.size);
	for (int i = 0; i < pxl.size; i++) {
		pxl.values[i] = 0;
	}
	pixel_state_plot(pxl);
}


void clipboard_pixels_paste(SDL_Rect dest) {
	pixel_struct pxl = pixel_new();
	char * string = SDL_GetClipboardText();
	int status = pixel_state_reconstruct(&pxl, string);
	SDL_free(string);
	if (status > 0) return;
	pxl.rect.x = dest.x;
	pxl.rect.y = dest.y;
	
	// store undo information
	pixel_struct pxl_undo = pixel_new();
	pxl_undo.rect = pxl.rect;
	undo_record(pixel_state_capture(pxl_undo));

	// plot clipboard to rom
	pixel_state_plot(pxl);
}
