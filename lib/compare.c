#include "baurinum.h"

// (unsigned) Compare L and R; does not take sign into account;
bn_comp_result bn_ucmp(bnum* l, bnum* r) {
    if (l->len > r->len) {
        return BN_GT;
    } else if (l->len < r->len) {
        return BN_LT;
    }
    // If reaches here that means both are same length

    for (int i = l->len - 1; i >= 0; i--) {
        if (l->digits[i] > r->digits[i]) {
            return BN_GT;
        } else if (l->digits[i] < r->digits[i]) {
            return BN_LT;
        }
    }

    return BN_EQ;
}

//(Signed) compare
bn_comp_result bn_cmp(bnum* l, bnum* r) {
    if (l->sign != r->sign) {
        if (l->sign == BN_NEG) {
            return BN_LT;
        } else {
            return BN_GT;
        }
    }
    if (l->sign == BN_NEG) {
        return bn_ucmp(r, l);
    } else {
        return bn_ucmp(l, r);
    }
}
