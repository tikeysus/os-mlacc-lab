#include "image_kernels.h"

static uint8_t clamp_i32_to_u8(int32_t value) {
    if (value < 0) {
        return 0;
    }

    if (value > UINT8_MAX) {
        return UINT8_MAX;
    }

    return (uint8_t)value;
}

static int image_shapes_are_valid(image_u8_view src, image_u8 dst) {
    if (src.data == 0 || dst.data == 0) {
        return 0;
    }

    if (src.width == 0 || src.height == 0 || dst.width < src.width || dst.height < src.height) {
        return 0;
    }

    if (src.stride < src.width || dst.stride < dst.width) {
        return 0;
    }

    if (src.height - 1 > (SIZE_MAX - src.width) / src.stride) {
        return 0;
    }

    return dst.height - 1 <= (SIZE_MAX - dst.width) / dst.stride;
}

static int32_t abs_i32(int32_t value) { return value < 0 ? -value : value; }

static size_t image_span(size_t width, size_t height, size_t stride) {
    return ((height - 1) * stride) + width;
}

static int image_buffers_overlap(image_u8_view src, image_u8 dst) {
    const uintptr_t src_start = (uintptr_t)src.data;
    const uintptr_t dst_start = (uintptr_t)dst.data;
    const size_t src_span = image_span(src.width, src.height, src.stride);
    const size_t dst_span = image_span(dst.width, dst.height, dst.stride);

    if (src_span > UINTPTR_MAX - src_start || dst_span > UINTPTR_MAX - dst_start) {
        return 1;
    }

    return src_start < dst_start + dst_span && dst_start < src_start + src_span;
}

int threshold_u8(image_u8_view src, image_u8 dst, uint8_t threshold) {
    if (!image_shapes_are_valid(src, dst)) {
        return IMAGE_KERNEL_INVALID_ARGUMENT;
    }

    if (image_buffers_overlap(src, dst) && (src.data != dst.data || src.stride != dst.stride)) {
        return IMAGE_KERNEL_OVERLAPPING_BUFFERS;
    }

    const size_t width = src.width < dst.width ? src.width : dst.width;
    const size_t height = src.height < dst.height ? src.height : dst.height;

    for (size_t y = 0; y < height; y++) {
        const uint8_t *src_row = src.data + (y * src.stride);
        uint8_t *dst_row = dst.data + (y * dst.stride);

        for (size_t x = 0; x < width; x++) {
            dst_row[x] = src_row[x] >= threshold ? UINT8_MAX : 0;
        }
    }

    return IMAGE_KERNEL_OK;
}

int convolve_u8(image_u8_view src, image_u8 dst, const int16_t *kernel, size_t kernel_width,
                size_t kernel_height) {
    /* Each stride must span at least the logical row width before row offsets are used. */
    if (kernel == 0 || !image_shapes_are_valid(src, dst)) {
        return IMAGE_KERNEL_INVALID_ARGUMENT;
    }

    if (kernel_width == 0 || kernel_height == 0) {
        return IMAGE_KERNEL_INVALID_DIMENSIONS;
    }

    if ((kernel_width % 2) == 0 || (kernel_height % 2) == 0) {
        return IMAGE_KERNEL_INVALID_DIMENSIONS;
    }

    if (kernel_width > src.width || kernel_height > src.height) {
        return IMAGE_KERNEL_INVALID_DIMENSIONS;
    }

    if (image_buffers_overlap(src, dst)) {
        return IMAGE_KERNEL_OVERLAPPING_BUFFERS;
    }

    const size_t kernel_x_center = kernel_width / 2;
    const size_t kernel_y_center = kernel_height / 2;

    for (size_t y = 0; y < src.height; y++) {
        uint8_t *dst_row = dst.data + (y * dst.stride);

        for (size_t x = 0; x < src.width; x++) {
            int32_t sum = 0;

            for (size_t ky = 0; ky < kernel_height; ky++) {
                const size_t src_y = y + ky;

                if (src_y < kernel_y_center || src_y - kernel_y_center >= src.height) {
                    continue;
                }

                const uint8_t *src_row = src.data + ((src_y - kernel_y_center) * src.stride);

                for (size_t kx = 0; kx < kernel_width; kx++) {
                    const size_t src_x = x + kx;

                    if (src_x < kernel_x_center || src_x - kernel_x_center >= src.width) {
                        continue;
                    }

                    sum += (int32_t)src_row[src_x - kernel_x_center] *
                           kernel[(ky * kernel_width) + kx];
                }
            }

            dst_row[x] = clamp_i32_to_u8(sum);
        }
    }

    return IMAGE_KERNEL_OK;
}

int convolve3x3_u8(image_u8_view src, image_u8 dst, const int8_t kernel[9]) {
    int16_t wide_kernel[9];

    if (kernel == 0) {
        return IMAGE_KERNEL_INVALID_ARGUMENT;
    }

    for (size_t i = 0; i < 9; i++) {
        wide_kernel[i] = (int16_t)kernel[i];
    }

    return convolve_u8(src, dst, wide_kernel, 3, 3);
}

int sobel3x3_u8(image_u8_view src, image_u8 dst, uint8_t threshold) {
    if (!image_shapes_are_valid(src, dst)) {
        return IMAGE_KERNEL_INVALID_ARGUMENT;
    }

    if (image_buffers_overlap(src, dst)) {
        return IMAGE_KERNEL_OVERLAPPING_BUFFERS;
    }

    for (size_t y = 0; y < src.height; y++) {
        uint8_t *dst_row = dst.data + (y * dst.stride);

        for (size_t x = 0; x < src.width; x++) {
            dst_row[x] = 0;
        }
    }

    if (src.width < 3 || src.height < 3) {
        return IMAGE_KERNEL_OK;
    }

    for (size_t y = 1; y + 1 < src.height; y++) {
        const uint8_t *top = src.data + ((y - 1) * src.stride);
        const uint8_t *mid = src.data + (y * src.stride);
        const uint8_t *bottom = src.data + ((y + 1) * src.stride);
        uint8_t *dst_row = dst.data + (y * dst.stride);

        for (size_t x = 1; x + 1 < src.width; x++) {
            const int32_t gx = -(int32_t)top[x - 1] + (int32_t)top[x + 1] -
                               (2 * (int32_t)mid[x - 1]) + (2 * (int32_t)mid[x + 1]) -
                               (int32_t)bottom[x - 1] + (int32_t)bottom[x + 1];
            const int32_t gy = (int32_t)top[x - 1] + (2 * (int32_t)top[x]) + (int32_t)top[x + 1] -
                               (int32_t)bottom[x - 1] - (2 * (int32_t)bottom[x]) -
                               (int32_t)bottom[x + 1];
            const int32_t magnitude = abs_i32(gx) + abs_i32(gy);

            dst_row[x] = magnitude > threshold ? UINT8_MAX : 0;
        }
    }

    return IMAGE_KERNEL_OK;
}
