#include "baurinum.h"

bnerr bn_add(bnum* c, bnum* a, bnum* b) {
    int min;
    int max;
    bnum* bgx;
    bnerr err;
    if (a->len > b->len) {
        min = b->len;
        max = a->len;
        bgx = a;
    } else {
        min = a->len;
        max = b->len;
        bgx = b;
    }

    // printf("max->%d|min->%d\n", max, min);
    if (c->cap <= max + 1) {
        err = bn_grow_by(c, max + 1);
        if (err != BN_OK) {
            return err;
        }
    }

    int old_c_len = c->len;
    c->len = max + 1;

    bdigit u = 0;
    int i = 0;
    // printf("mask -> %ld" , N_MASK);
    for (i = 0; i < min; i++) {
        c->digits[i] = a->digits[i] + b->digits[i] + u;
        u = c->digits[i] / 10;
        c->digits[i] %= 10;
    }

    // printf("|carry->%ld | %ld|\n", u, c->digits[i]);

    if (min != max) {
        for (; i < max; i++) {
            c->digits[i] = bgx->digits[i] + u;
            u = c->digits[i] / 10;
            c->digits[i] %= 10;

            //      printf("|>carry->%ld | %ld<|\n", u, c->digits[i]);
        }
    }

    // c_ptr++;
    c->digits[i] = u;

    for (i = c->len; i < old_c_len; i++) {
        c->digits[i] = 0;
    }
    bn_trim(c);
    return BN_OK;
}
