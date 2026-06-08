CC ?= gcc
BUILD_DIR := build
TARGET := $(BUILD_DIR)/scalar_dot_product
EDGE_DEMO := $(BUILD_DIR)/edge_detection_demo
ML_TEST := $(BUILD_DIR)/test_ml_kernels
IMAGE_TEST := $(BUILD_DIR)/test_image_kernels
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS := -Isoftware/include
UNITY_CPPFLAGS := -Ithird_party/unity/src -DsetUp=set_up -DtearDown=tear_down
UNITY_SRC := third_party/unity/src/unity.c
SCALAR_SRCS := software/benchmarks/scalar/main.c software/benchmarks/scalar/dot_product.c
ML_KERNEL_SRCS := software/benchmarks/scalar/dot_product.c \
	software/benchmarks/scalar/gemv.c \
	software/benchmarks/scalar/quantized_dot_product.c
ML_TEST_SRCS := software/tests/test_ml_kernels.c $(ML_KERNEL_SRCS) $(UNITY_SRC)
IMAGE_KERNEL_SRCS := software/benchmarks/scalar/image_kernels.c
EDGE_DEMO_SRCS := software/benchmarks/scalar/edge_detection_demo.c $(IMAGE_KERNEL_SRCS)
IMAGE_TEST_SRCS := software/tests/test_image_kernels.c $(IMAGE_KERNEL_SRCS) $(UNITY_SRC)

.PHONY: all run run-edge-demo test verify clean

all: $(TARGET)

$(TARGET): $(SCALAR_SRCS) software/include/ml_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SCALAR_SRCS) -o $@

$(EDGE_DEMO): $(EDGE_DEMO_SRCS) software/include/image_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(EDGE_DEMO_SRCS) -o $@

$(ML_TEST): $(ML_TEST_SRCS) software/include/ml_kernels.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(ML_TEST_SRCS) -o $@

$(IMAGE_TEST): $(IMAGE_TEST_SRCS) software/include/image_kernels.h third_party/unity/src/unity.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(UNITY_CPPFLAGS) $(CFLAGS) $(IMAGE_TEST_SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

run-edge-demo: $(EDGE_DEMO)
	./$(EDGE_DEMO)

test: $(ML_TEST) $(IMAGE_TEST)
	./scripts/run_tests ./$(ML_TEST) ./$(IMAGE_TEST)

verify:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" all
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" test
	./scripts/check_repo

clean:
	rm -rf $(BUILD_DIR)
