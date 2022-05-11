

uint8_t undo_pos = 0;
char** undo_log;

void undo_init() {
	undo_log = malloc(256 * sizeof(char*));
	for (int i = 0; i < 256; i++) {
		undo_log[i] = malloc(1000 * sizeof(char));
	}
}

void undo_record(const char * data) {
	char * ptr = undo_log[undo_pos];
	free(ptr);
	ptr = malloc((strlen(data) + 1) * sizeof(char));
	strcpy(ptr, data);
	printf("%d\t%d\t%s\n", undo_pos, strlen(ptr), ptr);
	undo_pos++;
}

void undo_rewind() {
	undo_pos--;
	pixel_struct pxl;
	printf("reconstruct\n");
	char * ptr = undo_log[undo_pos];
	printf("%s\n", ptr);
	pixel_state_reconstruct(&pxl, ptr);
	printf("%d\t%d\t%s\n", undo_pos, strlen(ptr), ptr);
	printf("plot\n");
	printf("\n%d\t%d\t%d\t%d\n", pxl.rect.x, pxl.rect.y, pxl.rect.w, pxl.rect.h);
	printf("%d\n", pxl.rect.x);
	pixel_state_plot(pxl);

}

void undo_redo() {
}
