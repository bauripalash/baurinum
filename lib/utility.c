#include <stdio.h>
#include <stdlib.h>

#include "baurinum.h"

/*
 * Utility Functions
 */

char* bnsign_to_str(bnsign s) {
    if (s == BN_ZERO) {
        return "0";
    } else if (s == BN_POS) {
        return "+";
    } else {
        return "-";
    }
}

char* cmpres_to_str(bn_comp_result cr) {
    if (cr == BN_LT) {
        return "LT";
    } else if (cr == BN_GT) {
        return "GT";
    } else {
        return "EQ";
    }
}

void bn_print(bnum* b) {
    if (b->cap == 0 || b->len == 0 || b->digits == NULL) {
        return;
    }

    printf("%c", b->sign != BN_NEG ? '+' : '-');
    for (int i = b->len - 1; i >= 0; i--) {
        printf("%lu", (uint64_t)b->digits[i]);
    }
}

void bn_println(bnum* b) {
    bn_print(b);
    printf("\n");
}

void bn_debug(bnum* b) {
    printf("[ len->%d | cap->%d | sign->%c | digits->(", b->len, b->cap,
           b->sign == BN_POS ? '+' : '-');
    if (b->digits != NULL) {
        bn_print(b);
    } else {
        printf("NULL");
    }
    printf(")]\n");
}

// trim excess zeros
void bn_trim(bnum* b) {
    // printf("-trim-\n");
    while (b->len > 0 && b->digits[b->len - 1] == 0) {
        b->len -= 1;
    }

    if (b->len == 0) {
        b->sign = BN_ZERO;
    }
}

// Turn `res` into a clone of `input`
bnerr bn_clone(bnum* res, bnum* input) {
    if (res == input) {
        return BN_OK;
    }

    if (res->cap < input->len) {
        bnerr err = bn_grow_by(res, input->len);
        if (err != BN_OK) {
            return err;
        }
    }

    bdigit *t_inp, *t_res;
    t_inp = input->digits;
    t_res = res->digits;
    int i = 0;
    for (i = 0; i < input->len; i++) {
        *t_res++ = *t_inp++;
    }

    for (; i < res->len; i++) {
        *t_res++ = 0;
    }

    res->len = input->len;
    res->sign = input->sign;

    return BN_OK;
}

// Initialize l and make it a clone of r
bnerr bn_dup_boot(bnum* l, bnum* r) {
    bnerr err = bn_boot(l);
    errcheck;
    return bn_clone(l, r);
}

// Zero the digits
void bn_mkzero(bnum* b) {
    bdigit* ptr = b->digits;
    b->sign = BN_ZERO;
    b->cap = 0;

    for (int i = 0; i < b->cap; i++) {
        *ptr++ = 0;
    }
}

/*
 * End of Utility Functions
 */
