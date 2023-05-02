#include "baurinum.h"

#include <stdio.h>
#include <stdlib.h>

// Initialize a `bnnum`
bnerr bn_boot(bnum* b) {
    b->digits = (bdigit*)calloc((size_t)DEFAULT_DIGIT, sizeof(bdigit));
    if (b->digits == NULL) {
        return BN_ERR_NOMEM;
    }
    b->cap = DEFAULT_DIGIT;
    b->len = 0;
    b->sign = BN_ZERO;
    return BN_OK;
}

void bn_clear(bnum* b) {
    if (b->digits != NULL) {
        for (int i = 0; i < b->len; i++) {
            b->digits[i] = (bdigit)0;
        }

        // free(b->digits);
        b->sign = BN_ZERO;
        b->len = 0;
        b->cap = 0;
        b->digits = NULL;
    }
}

// set the capacity of `b` to b->capacity + size;
// and allocate memory for it.
bnerr bn_grow_by(bnum* b, int size) {
    int oldsize = b->cap;
    b->cap = oldsize + size;
    b->digits = realloc(b->digits, b->cap);
    if (b->digits == NULL) {
        return BN_ERR_NOMEM;
    }
    return BN_OK;
}

bnerr bn_grow(bnum* b) { return bn_grow_by(b, GROW_SIZE(b->cap)); }

void bn_rev(bnum* b) {
    for (int i = 0; i < b->len / 2; i++) {
        int tmp = b->digits[i];
        b->digits[i] = b->digits[b->len - i - 1];
        b->digits[b->len - i - 1] = tmp;
    }
}
