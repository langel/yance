
enum comp_targets comps_manual_cache_src_comp;

void comps_manual_set_as_target() {
	if (comp_target != manual) {
		comps_manual_cache_src_comp = comp_target;
		comp_target = manual;
	}
}

/*
	min screen size = 640x480
	font size = 9x16
	71 x 30 characters on min window
	for centering and status bar: 70x28 char grid
*/

#define manual_rows 28
#define manual_cols 70

SDL_Rect manual_rect = { 0, 0, 70 * 9, 28 * 16 };
unsigned char manual_screen[manual_rows][manual_cols+1];

char manual_commands_global[][65] = {
	"             Yet  Another  NES  CHR  Editor  Manual            ",
	"                                                                 ",
	"                             ",
	"    Open        Drag + Drop     Navigate          Tab",
	"    New         tbd Ctrl-N   ",
	"    Save        Ctrl-S          Move Cursor       Arrows",
	"                                                             ",
	"    Copy        Ctrl-C          Edit Selection    Shift-Arrows",
	"    Cut         Ctrl-X                        ",
	"    Paste       Ctrl-V          Deselect          Ctrl-D",
	"",
	"    Undo        Ctrl-Z          Page Up/Down in Rom View",
	"    Redo        Ctrl-Y          Toggle 8x16 Mode  T",
	"                                Plot Pixels       Space",
	"",
	"    Flip        F               Current Color     1,2,3,4",
	"    Mirror      M               Change Color      Ctrl-Arrows",
	"    Translate   CtrlShftArrows  Current Palette   Ctrl-(1..8)",
	"    Rotate CW   R               Grid Toggle       G",
	"    Rotate CCW  Shift-R         View Source       Ctrl-G",
	"",
	"",
	"    Quit        Ctrl-Q                 Close this Manual   ESC",
	"",
};


void comps_manual_init() {
	for (int r = 0; r < manual_rows; r++) {
		for (int c = 0; c < manual_cols; c++) {
			if (c == 0 || c == manual_cols - 1) manual_screen[r][c] = 0xba;
			else if (r == 0 || r == manual_rows - 1) manual_screen[r][c] = 0xcd;
			else manual_screen[r][c] = 0x20;
		}
	}
	manual_screen[0][0] = 0xc9;
	manual_screen[0][manual_cols - 1] = 0xbb;
	manual_screen[manual_rows - 1][0] = 0xc8;
	manual_screen[manual_rows - 1][manual_cols - 1] = 0xbc;
	for (int r = 0; r < 24; r++) {
		for (int c = 0; c < 64; c++) {
			if ((int) manual_commands_global[r][c] != 0) {
				manual_screen[r + 2][c + 3] = (unsigned char) manual_commands_global[r][c];
			}
		}
	}
}

void comps_manual_render() {
	manual_rect.x = (window_rect.w - manual_rect.w) / 2;
	manual_rect.y = (window_rect.h - manual_rect.h) / 2 - 8;
	render_color_set(renderer, colors[0x00]);
	SDL_RenderFillRect(renderer, &manual_rect);
	ascii_color_set(colors[0x40]);
	for (int i = 0; i < manual_rows; i++) {
		ascii_text_render(manual_screen[i], manual_rect.x, manual_rect.y + i * 16);
	}
}


void comps_manual_update() {
	if (keys[SDL_SCANCODE_ESCAPE] == 1) {
		comp_target = comps_manual_cache_src_comp;
	}
}
