#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/baurinum.h"

int main(void) {
    bnum a;
    bn_boot(&a);
    bn_set_int(&a, 100);
    bn_println(&a);
    bn_clear(&a);
}
