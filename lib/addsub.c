#include <stdio.h>

#include "baurinum.h"

bnerr bn_add(bnum* res, const bnum* a, const bnum* b) {
    bnerr err;
    if (a->sign == b->sign) {
        res->sign = a->sign;

        err = bn_u_add(res, a, b);
        errcheck;
    } else if (bn_cmp(a, b) == BN_LT) {
        res->sign = b->sign;
        err = bn_u_sub(res, b, a);
        errcheck;
    } else {
        res->sign = a->sign;
        err = bn_u_sub(res, a, b);
        errcheck;
    }

    return BN_OK;
}

bnerr bn_u_add(bnum* c, const bnum* a, const bnum* b) {
    int min;
    int max;
    const bnum* bgx;
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
        errcheck;
    }

    int old_c_len = c->len;
    c->len = max + 1;

    bdigit u = 0;
    int i = 0;
    for (i = 0; i < min; i++) {
        c->digits[i] = a->digits[i] + b->digits[i] + u;
        u = c->digits[i] / 10;
        c->digits[i] %= 10;
    }
    if (min != max) {
        for (; i < max; i++) {
            c->digits[i] = bgx->digits[i] + u;
            u = c->digits[i] / 10;
            c->digits[i] %= 10;
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

bnerr bn_u_sub(bnum* res, const bnum* a, const bnum* b) {
    int minlen = b->len;
    int maxlen = a->len;
    int old_res_len = res->len;
    res->len = maxlen;

    if (res->cap < maxlen) {
        bnerr err = bn_grow_by(res, maxlen);
        errcheck;
    }
    bdigit c = 0;
    int i;
    for (i = 0; i < minlen; i++) {
        bdigit x = a->digits[i] - c;
        bdigit y = b->digits[i];
        if (x < y) {
            x += 10;
            c = 1;
        } else {
            c = 0;
        }
        res->digits[i] = x - y;
        // res->digits[i] = (a->digits[i] - b->digits[i]) - c;
        // printf("RD->%d| %lu | %lu | %lu \n" , i , a->digits[i] ,b->digits[i]
        // , res->digits[i] ); c = res->digits[i] / 10; res->digits[i] %= 10;
    }

    for (; i < maxlen; i++) {
        res->digits[i] = a->digits[i] - c;
        // printf("RD->%d| %lu | %lu \n", i, a->digits[i], res->digits[i]);
        c = res->digits[i] / 10;
        res->digits[i] %= 10;
    }

    for (i = res->len; i < old_res_len; i++) {
        res->digits[i] = 0;
    }

    bn_trim(res);

    return BN_OK;
}

bnerr bn_sub(bnum* res, const bnum* a, const bnum* b) {
    bnerr err;
    if (a->sign != b->sign) {
        res->sign = a->sign;
        err = bn_u_add(res, a, b);
        errcheck;
    } else if ((bn_ucmp(a, b) == BN_GT) || (bn_ucmp(a, b) == BN_EQ)) {
        res->sign = a->sign;
        err = bn_u_sub(res, a, b);
        errcheck;
    } else {
        res->sign = a->sign == BN_NEG ? BN_POS : BN_NEG;
        err = bn_u_sub(res, b, a);
        errcheck;
    }
    return BN_OK;
}
