CC ?= gcc
BUILD_DIR := build
TARGET := $(BUILD_DIR)/scalar_dot_product
ML_TEST := $(BUILD_DIR)/test_ml_kernels
GEMM_TEST := $(BUILD_DIR)/test_gemm
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS := -Isoftware/include
UNITY_CPPFLAGS := -Ithird_party/unity/src -DsetUp=set_up -DtearDown=tear_down
UNITY_SRC := third_party/unity/src/unity.c
SCALAR_SRCS := software/benchmarks/scalar/main.c \
	software/benchmarks/scalar/dot_product.c \
	software/benchmarks/scalar/gemm.c
ML_KERNEL_SRCS := software/benchmarks/scalar/dot_product.c \
	software/benchmarks/scalar/gemv.c \
	software/benchmarks/scalar/quantized_dot_product.c \
	software/benchmarks/scalar/gemm.c
ML_TEST_SRCS := software/tests/test_ml_kernels.c $(ML_KERNEL_SRCS) $(UNITY_SRC)
GEMM_TEST_SRCS := software/tests/test_gemm.c $(ML_KERNEL_SRCS) $(UNITY_SRC)

.PHONY: all run test verify clean

all: $(TARGET)

$(TARGET): $(SCALAR_SRCS) software/include/ml_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SCALAR_SRCS) -o $@

$(ML_TEST): $(ML_TEST_SRCS) software/include/ml_kernels.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(ML_TEST_SRCS) -o $@

$(GEMM_TEST): $(GEMM_TEST_SRCS) software/include/ml_kernels.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(GEMM_TEST_SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

test: $(ML_TEST) $(GEMM_TEST)
	./scripts/run_tests ./$(ML_TEST)
	./scripts/run_tests ./$(GEMM_TEST)

verify:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" all
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" test
	./scripts/check_repo

clean:
	rm -rf $(BUILD_DIR)
