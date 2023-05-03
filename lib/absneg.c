#include "baurinum.h"
// res = |inp|
bnerr bn_abs(bnum* res, bnum* inp) {
    if (res != inp) {
        bnerr err = bn_clone(res, inp);
        errcheck;
    }

    res->sign = BN_ZERO;
    return BN_OK;
}

// res = -inp
bnerr bn_make_neg(bnum* res, bnum* inp) {
    if (res != inp) {
        bnerr err = bn_clone(res, inp);
        errcheck;
    }

    if (inp->len == 0) {
        return BN_OK;
    }

    if (inp->sign == BN_ZERO) {
        res->sign = BN_NEG;
    } else {
        res->sign = BN_ZERO;
    }

    return BN_OK;
}
