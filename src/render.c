#include "render.h"
#include <stdio.h>

static const char *half_block = "▀";

static void move_home(void) {
    printf("\x1b[H");
}

static void reset_colors(void) {
    printf("\x1b[0m");
}

void render_image(const image_t *img) {
    move_home();

    for (int y = 0; y < img->h; y += 2) {
        for (int x = 0; x < img->w; ++x) {
            int top_idx = (y * img->w + x) * 3;
            uint8_t tr = img->data[top_idx];
            uint8_t tg = img->data[top_idx + 1];
            uint8_t tb = img->data[top_idx + 2];

            if (y + 1 < img->h) {
                int bot_idx = ((y + 1) * img->w + x) * 3;
                uint8_t br = img->data[bot_idx];
                uint8_t bg = img->data[bot_idx + 1];
                uint8_t bb = img->data[bot_idx + 2];
                
                printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm%s", 
                       tr, tg, tb, br, bg, bb, half_block);
            } else {
                printf("\x1b[38;2;%d;%d;%dm\x1b[49m%s", tr, tg, tb, half_block);
            }
        }
        reset_colors();
        // clear the rest of the line to remove background blotches
        printf("\x1b[K\n"); 
    }
    // clear anything leftover below the image
    printf("\x1b[J");
    fflush(stdout);
}