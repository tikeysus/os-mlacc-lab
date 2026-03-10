#include <stdio.h>
#include "dot_product.h"

int main() {

    float a[4] = {1,2,3,4};
    float b[4] = {5,6,7,8};

    float result = dot_product(a, b, 4);

    printf("dot product = %f\n", result);

    return 0;
}