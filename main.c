#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/baurinum.h"

#define mask  ((((bdigit)1) << ((bdigit)60)) - ((bdigit)1))
#define CB    __CHAR_BIT__
#define SZ(t) ((size_t)__CHAR_BIT__ * sizeof(t))

int main(void) {
    bnum a;
    bnum b;
    bnum c;

    bnerr err;

    bn_boot(&a);
    bn_boot(&b);
    bn_boot(&c);

    err = bn_set_int(&a, 11234);
    if (err != BN_OK) {
        printf("failed to set a\n");
    }

    char* sa = bn_as_str(&a, true);

    if (sa != NULL) {
        printf("tostr -> %s\n", sa);
    }
    free(sa);

    // bdigit x = (bdigit)23 >> (bdigit)(__CHAR_BIT__ * sizeof(bdigit) - 1u);

    // bdigit y = x & mask;

    bn_set_int(&b, 23);
    bn_sub(&c, &b, &a);
    // printf("->->%d | %d | %lu\n" , CB ,  x , y);
    bn_debug(&a);
    bn_debug(&b);
    bn_debug(&c);

    bn_clear(&a);
    bn_clear(&b);
    bn_clear(&c);
}
