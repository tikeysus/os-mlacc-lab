CC ?= gcc
BUILD_DIR := build
TARGET := $(BUILD_DIR)/scalar_dot_product
EDGE_DEMO := $(BUILD_DIR)/edge_detection_demo
IMAGE_TEST := $(BUILD_DIR)/test_image_kernels
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS := -Isoftware/include
SCALAR_SRCS := software/benchmarks/scalar/main.c software/benchmarks/scalar/dot_product.c
IMAGE_KERNEL_SRCS := software/benchmarks/scalar/image_kernels.c
EDGE_DEMO_SRCS := software/benchmarks/scalar/edge_detection_demo.c $(IMAGE_KERNEL_SRCS)
IMAGE_TEST_SRCS := software/tests/test_image_kernels.c $(IMAGE_KERNEL_SRCS)

.PHONY: all run run-edge-demo test verify clean

all: $(TARGET)

$(TARGET): $(SCALAR_SRCS) software/include/ml_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SCALAR_SRCS) -o $@

$(EDGE_DEMO): $(EDGE_DEMO_SRCS) software/include/image_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(EDGE_DEMO_SRCS) -o $@

$(IMAGE_TEST): $(IMAGE_TEST_SRCS) software/include/image_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(IMAGE_TEST_SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

run-edge-demo: $(EDGE_DEMO)
	./$(EDGE_DEMO)

test: $(IMAGE_TEST)
	./$(IMAGE_TEST)

verify:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" all
	$(MAKE) CFLAGS="$(CFLAGS) -Werror" test
	./scripts/check_repo

clean:
	rm -rf $(BUILD_DIR)
