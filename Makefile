CC ?= gcc
BUILD_DIR := build
TARGET := $(BUILD_DIR)/scalar_dot_product
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS := -Isoftware/include
SCALAR_SRCS := software/benchmarks/scalar/main.c software/benchmarks/scalar/dot_product.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SCALAR_SRCS) software/include/ml_kernels.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SCALAR_SRCS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
