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

typedef enum image_kernel_status {
    IMAGE_KERNEL_OK = 0,
    IMAGE_KERNEL_INVALID_ARGUMENT = -1,
    IMAGE_KERNEL_INVALID_DIMENSIONS = -2,
} image_kernel_status;

void threshold_u8(image_u8_view src, image_u8 dst, uint8_t threshold);
int convolve_u8(image_u8_view src, image_u8 dst, const int16_t *kernel, size_t kernel_width,
                size_t kernel_height);
void convolve3x3_u8(image_u8_view src, image_u8 dst, const int8_t kernel[9]);
void sobel3x3_u8(image_u8_view src, image_u8 dst, uint8_t threshold);

#endif
