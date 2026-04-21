// implement stb single-file libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#include "image.h"
#include <stdlib.h>

bool image_load(image_t *img, const char *path) {
    // force 3 channels for rgb
    img->data = stbi_load(path, &img->w, &img->h, &img->channels, 3);
    if (!img->data) {
        return false;
    }
    img->channels = 3;
    return true;
}

void image_free(image_t *img) {
    if (img && img->data) {
        stbi_image_free(img->data);
        img->data = NULL;
    }
}

bool image_resize(image_t *dest, const image_t *src, int target_w, int target_h) {
    dest->w = target_w;
    dest->h = target_h;
    dest->channels = src->channels;
    dest->data = malloc((size_t)(target_w * target_h * dest->channels));

    if (!dest->data) {
        return false;
    }

    // resize using linear interpolation
  stbir_resize_uint8_srgb(
        src->data, src->w, src->h, 0,
        dest->data, dest->w, dest->h, 0,
        (stbir_pixel_layout)dest->channels
    );

    return true;
}