#pragma once
#include <stdbool.h>

bool tui_enable_raw_mode(void);
void tui_disable_raw_mode(void);
bool tui_get_window_size(int *cols, int *rows);
void tui_hide_cursor(void);
void tui_show_cursor(void);
void tui_clear_screen(void);
void tui_enter_alt_screen(void);
void tui_exit_alt_screen(void);