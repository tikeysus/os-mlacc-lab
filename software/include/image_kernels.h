#ifndef IMAGE_KERNELS_H
#define IMAGE_KERNELS_H

#include <stddef.h>
#include <stdint.h>

typedef struct image_u8 {
    uint8_t *data;
    size_t width;
    size_t height;
    size_t stride;
} image_u8;

typedef struct image_u8_view {
    const uint8_t *data;
    size_t width;
    size_t height;
    size_t stride;
} image_u8_view;

void threshold_u8(image_u8_view src, image_u8 dst, uint8_t threshold);
void convolve3x3_u8(image_u8_view src, image_u8 dst, const int8_t kernel[9]);
void sobel3x3_u8(image_u8_view src, image_u8 dst, uint8_t threshold);

#endif
