#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/baurinum.h"

int main(void) {
    bnum a;
    bnum b;
    bnum c;
    char* x = "1234567890";

    //    char * y = "11111111111111111111111111111";
    bn_boot(&a);
    bn_boot(&b);
    bn_boot(&c);
    bnerr err = bn_set_str(&b, x);
    if (err != BN_OK) {
        fprintf(stderr, "set str failed");
    }
    err = bn_set_double(&c, 16473647634763746376476347637463.499);
    bn_add(&a, &b, &c);

    bn_debug(&a);
    bn_debug(&b);
    bn_debug(&c);
    bn_clear(&a);
    bn_clear(&b);
    bn_clear(&c);
}
