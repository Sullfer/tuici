#include "tui.h"
#include <windows.h>
#include <stdio.h>

static DWORD orig_input_mode;
static DWORD orig_output_mode;
static UINT orig_cp;

bool tui_enable_raw_mode(void) {
    HANDLE h_in = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);

    orig_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);

    GetConsoleMode(h_in, &orig_input_mode);
    GetConsoleMode(h_out, &orig_output_mode);

    DWORD raw_input = orig_input_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    DWORD raw_output = orig_output_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(h_in, raw_input);
    SetConsoleMode(h_out, raw_output);
    return true;
}

void tui_disable_raw_mode(void) {
    SetConsoleOutputCP(orig_cp);
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), orig_input_mode);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), orig_output_mode);
}

bool tui_get_window_size(int *cols, int *rows) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return true;
    }
    return false;
}

void tui_hide_cursor(void) {
    printf("\x1b[?25l");
}

void tui_show_cursor(void) {
    printf("\x1b[?25h");
}

void tui_clear_screen(void) {
    printf("\x1b[2J\x1b[H");
}

void tui_enter_alt_screen(void) {
    printf("\x1b[?1049h\x1b[2J\x1b[H");
    fflush(stdout);
}

void tui_exit_alt_screen(void) {
    printf("\x1b[?1049l");
    fflush(stdout);
}