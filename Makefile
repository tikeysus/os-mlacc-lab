CC=gcc
CFLAGS=-Iinclude

all:
	$(CC) src/main.c src/dot_product.c $(CFLAGS) -o dot_product_test