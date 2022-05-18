
#define undo_max_steps 256

char ** undo_log;
int undo_head = 0;
int undo_tail = 0;
int undo_count = 0;

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
	strcpy(ptr, data);
	undo_log[undo_head] = ptr;
	undo_head = (undo_head + 1) % undo_max_steps;
	if (undo_count == undo_max_steps) {
		undo_tail = (undo_tail + 1) % undo_max_steps;
	}
	else undo_count++;
}

int undo_rewind() {
	if (undo_count == 0) return -1;
	undo_count--;
	undo_head--;
	if (undo_head == -1) undo_head += undo_max_steps;
	pixel_struct pxl = pixel_new();
	char * ptr = undo_log[undo_head];
	pixel_state_reconstruct(&pxl, ptr);
	pixel_state_plot(pxl);
	return undo_count;
}

void undo_redo() {
}
