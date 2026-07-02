CC ?= gcc
BUILD_DIR := build
TARGET := $(BUILD_DIR)/scalar_dot_product
GEMM_BENCHMARK := $(BUILD_DIR)/gemm_benchmark
ML_TEST := $(BUILD_DIR)/test_ml_kernels
GEMM_TEST := $(BUILD_DIR)/test_gemm
BENCH_HARNESS_TEST := $(BUILD_DIR)/test_benchmark_harness
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS := -Isoftware/include -Isoftware/benchmarks/scalar
UNITY_CPPFLAGS := -Ithird_party/unity/src -DsetUp=set_up -DtearDown=tear_down -DUNITY_INCLUDE_DOUBLE
UNITY_SRC := third_party/unity/src/unity.c
SCALAR_SRCS := software/benchmarks/scalar/main.c \
	software/benchmarks/scalar/dot_product.c \
	software/benchmarks/scalar/gemm.c
ML_KERNEL_SRCS := software/benchmarks/scalar/dot_product.c \
	software/benchmarks/scalar/gemv.c \
	software/benchmarks/scalar/quantized_dot_product.c \
	software/benchmarks/scalar/gemm.c
GEMM_BENCHMARK_SRCS := software/benchmarks/scalar/gemm_benchmark_main.c \
	software/benchmarks/scalar/benchmark_harness.c \
	$(ML_KERNEL_SRCS)
ML_TEST_SRCS := software/tests/test_ml_kernels.c $(ML_KERNEL_SRCS) $(UNITY_SRC)
GEMM_TEST_SRCS := software/tests/test_gemm.c $(ML_KERNEL_SRCS) $(UNITY_SRC)
BENCH_HARNESS_TEST_SRCS := software/tests/test_benchmark_harness.c \
	software/benchmarks/scalar/benchmark_harness.c \
	$(ML_KERNEL_SRCS) $(UNITY_SRC)

.PHONY: all run bench test verify clean

all: $(TARGET) $(GEMM_BENCHMARK)

$(TARGET): $(SCALAR_SRCS) software/include/ml_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SCALAR_SRCS) -o $@

$(GEMM_BENCHMARK): $(GEMM_BENCHMARK_SRCS) software/include/ml_kernels.h \
	software/benchmarks/scalar/benchmark_harness.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(GEMM_BENCHMARK_SRCS) -o $@

$(ML_TEST): $(ML_TEST_SRCS) software/include/ml_kernels.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(ML_TEST_SRCS) -o $@

$(GEMM_TEST): $(GEMM_TEST_SRCS) software/include/ml_kernels.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(GEMM_TEST_SRCS) -o $@

$(BENCH_HARNESS_TEST): $(BENCH_HARNESS_TEST_SRCS) software/include/ml_kernels.h \
	software/benchmarks/scalar/benchmark_harness.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(BENCH_HARNESS_TEST_SRCS) -o $@ -lm

run: $(TARGET)
	./$(TARGET)

bench: $(GEMM_BENCHMARK)
	./$(GEMM_BENCHMARK)

test: $(ML_TEST) $(GEMM_TEST) $(BENCH_HARNESS_TEST)
	./scripts/run_tests ./$(ML_TEST)
	./scripts/run_tests ./$(GEMM_TEST)
	./scripts/run_tests ./$(BENCH_HARNESS_TEST)

verify:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" all
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" test
	./scripts/check_repo

clean:
	rm -rf $(BUILD_DIR)
