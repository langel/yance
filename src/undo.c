
#define undo_max_steps 256

char ** undo_log;
int undo_head = 0;
int undo_tail = 0;
int undo_top = 0;

void undo_init() {
	undo_log = malloc(undo_max_steps * sizeof(char*));
	for (int i = 0; i < undo_max_steps; i++) {
		undo_log[i] = NULL;
	}
}

void undo_record(const char * data) {
	char * ptr = undo_log[undo_head];
	free(ptr);
	ptr = malloc((strlen(data) + 1) * sizeof(char));
	printf("%zu\n", sizeof(ptr));
	strcpy(ptr, data);
	undo_log[undo_head] = ptr;
	undo_head = (undo_head + 1) % undo_max_steps;
	undo_top = undo_head;
	if (undo_tail == undo_head) {
		undo_tail = (undo_head + 1) % undo_max_steps;
	}
}

int undo_rewind() {
	if (undo_head == undo_tail) return -1;
	undo_head--;
	if (undo_head < 0) undo_head += undo_max_steps;
	pixel_struct pxl = pixel_new();
	char * ptr = undo_log[undo_head];
	pixel_state_reconstruct(&pxl, ptr);
	pixel_state_read(pxl, ptr);
	pixel_state_plot(pxl);
	return 1;
}

int undo_forward() {
	if (undo_head == undo_top) return -1;
	if (undo_head >= undo_max_steps) undo_head -= undo_max_steps;
	pixel_struct pxl = pixel_new();
	char * ptr = undo_log[undo_head];
	printf("%zu\n", sizeof(ptr));
	if (ptr == NULL) printf("ptr is null\n");
	printf("before reconstruct\n");
	for (int i = 0; i < 64; i++) {
		printf("%2x ", ptr[0x12 + i]);
	}
	if (pixel_state_reconstruct(&pxl, ptr) == 1) printf("failed ident\n");
	printf("\n");
	printf("after reconstruct\n");
	for (int i = 0; i < 64; i++) {
		printf("%2x ", pxl.values[i]);
	}
	printf("%zu\n", sizeof(ptr));
	printf("redo pixel state  x: %3d  y: %3d  h: %3d  w: %3d\n", pxl.rect.x, pxl.rect.y, pxl.rect.w, pxl.rect.h);
	printf("redo size %d\n", pxl.size);
	pixel_state_plot(pxl);
	undo_head = (undo_head + 1) % undo_max_steps;
	return 1;
}
