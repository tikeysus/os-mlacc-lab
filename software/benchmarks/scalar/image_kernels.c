#include "image_kernels.h"

void threshold_u8(image_u8_view src, image_u8 dst, uint8_t threshold) {
    (void)src;
    (void)dst;
    (void)threshold;
}

void convolve3x3_u8(image_u8_view src, image_u8 dst, const int8_t kernel[9]) {
    (void)src;
    (void)dst;
    (void)kernel;
}

void sobel3x3_u8(image_u8_view src, image_u8 dst, uint8_t threshold) {
    (void)src;
    (void)dst;
    (void)threshold;
}
