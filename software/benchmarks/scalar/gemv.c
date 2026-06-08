#include "ml_kernels.h"

void gemv_f32(const float *matrix, const float *vector, float *output, ml_matrix_shape shape) {
    for (size_t row = 0; row < shape.rows; row++) {
        output[row] = shape.cols == 0
                          ? 0.0f
                          : dot_product_f32(matrix + (row * shape.cols), vector, shape.cols);
    }
}
