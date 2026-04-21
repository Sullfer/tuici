#include "image.h"
#include "tui.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

static void fit_dimensions(int img_w, int img_h, int term_cols, int term_rows, int *out_w, int *out_h) {
    int max_w = term_cols;
    int max_h = term_rows * 2;

    float ratio_w = (float)max_w / (float)img_w;
    float ratio_h = (float)max_h / (float)img_h;
    float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;

    *out_w = (int)(img_w * ratio);
    *out_h = (int)(img_h * ratio);
    
    if (*out_w < 1) *out_w = 1;
    if (*out_h < 1) *out_h = 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    image_t orig_img = {0};
    if (!image_load(&orig_img, argv[1])) {
        fprintf(stderr, "failed to load image\n");
        return 1;
    }

    tui_enable_raw_mode();
    tui_enter_alt_screen();
    tui_hide_cursor();

    int last_cols = 0;
    int last_rows = 0;
    image_t resized_img = {0};
    bool running = true;

    // render loop
    while (running) {
        int cols, rows;
        if (tui_get_window_size(&cols, &rows)) {
            // prevent auto wrapping on windows edges
            cols -= 1;
            rows -= 1;

            // only recalculate and redraw if the size actually changed
            if (cols != last_cols || rows != last_rows) {
                last_cols = cols;
                last_rows = rows;

                tui_clear_screen();

                int tw, th;
                fit_dimensions(orig_img.w, orig_img.h, cols, rows, &tw, &th);

                // free the old resized image before making a new one to prevent memory leak
                image_free(&resized_img);
                
                if (image_resize(&resized_img, &orig_img, tw, th)) {
                    render_image(&resized_img);
                }
            }
        }

        // non blocking key check
        if (_kbhit()) {
            char c = _getch();
            if (c == 'q' || c == 27) {
                running = false;
            }
        }
        
        // sleep for 16ms to limit cpu
        Sleep(16);
    }

    // cleanup
    image_free(&resized_img);
    image_free(&orig_img);
    
    tui_show_cursor();
    tui_exit_alt_screen();
    tui_disable_raw_mode();

    return 0;
}