#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int w;
    int h;
    int channels;
    uint8_t *data;
} image_t;

bool image_load(image_t *img, const char *path);
void image_free(image_t *img);
bool image_resize(image_t *dest, const image_t *src, int target_w, int target_h);