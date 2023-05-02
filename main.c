#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/baurinum.h"

int main(void) {
    bnum b;
    char* x = "11111111111111111111111111111";
    //    char * y = "11111111111111111111111111111";
    bn_boot(&b);
    bnerr err = bn_set_str(&b, x);
    if (err != BN_OK) {
        fprintf(stderr, "set str failed");
    }
    bn_debug(&b);
    bn_clear(&b);
}
