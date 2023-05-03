#include <stdio.h>

#include "baurinum.h"
#include "stdlib.h"

static inline int is_endig(char c) { return c <= '9' && c >= '0'; }

bnerr bn_set_digit(bnum* b, bdigit dig) {
    if (b->cap <= b->len + 1) {
        bnerr err = bn_grow_by(b, 20);
        if (err != BN_OK) {
            return err;
        }
    }
    b->digits[b->len] = dig;
    b->len++;
    return BN_OK;
}

/*
 * Convert C types to Bnum
 */

// set a string (base 10)
bnerr bn_set_str(bnum* b, char* str) {
    char* ptr = str;
    //printf("->ptr->%c", *ptr);
    if (*ptr == '+') {
        b->sign = BN_POS;
        ptr++;
    } else if (*ptr == '-') {
        b->sign = BN_NEG;
        ptr++;
    } else {
        b->sign = BN_POS;
    }
    while (*ptr != '\0') {
        if (is_endig(*ptr)) {
            bnerr x = bn_set_digit(b, *ptr - '0');
            if (x != BN_OK) {
                return x;
            }
        }else if (*ptr == '.') {
           // do nothing
           // just skip
        } else {
            return BN_UNKNOWN_CHAR;
        }
        ptr++;
    }

    bn_rev(b);

    return BN_OK;
}

bnerr bn_set_double(bnum * b , double  n){
    int len = snprintf(NULL, 0, "%.0f" , n );
    char * temp_str = (char*)calloc(len + 1, sizeof(char));
    if (temp_str == NULL) {
    return BN_ERR_NOMEM;
    }
    int nlen = snprintf(temp_str,len+1, "%.0f" , n);
    if (nlen < len) {
    free(temp_str);
    return BN_STR_WRITE_FAIL ;
    }
    printf("SETD -> %s\n" , temp_str);
    bnerr err = bn_set_str(b, temp_str);

    if (err != BN_OK) {
    free(temp_str);
    return err;
    }

    return BN_OK; 
}

bnerr bn_set_ldouble(bnum * b ,long double  n){
    int len = snprintf(NULL, 0, "%.0Lf" , n );
    char * temp_str = (char*)calloc(len + 1, sizeof(char));
    if (temp_str == NULL) {
    return BN_ERR_NOMEM;
    }
    int nlen = snprintf(temp_str,len+1, "%.0Lf" , n);
    if (nlen < len) {
    free(temp_str);
    return BN_STR_WRITE_FAIL ;
    }
    bnerr err = bn_set_str(b, temp_str);

    if (err != BN_OK) {
    free(temp_str);
    return err;
    }

    return BN_OK; 
}

// set integer
bnerr bn_set_int(bnum* b, int n) {
    int tn = n;
    // printf("setting -> %d\n", n);
    if (n < 0) {
        b->sign = BN_NEG;
        tn = -tn;  // we must positive the int, otherwise the while loop won't
                   // work
    } else {
        b->sign = BN_POS;
    }

    while (tn > 0) {
        bnerr err = bn_set_digit(b, (bdigit)tn % 10);
        if (err != BN_OK) {
            return err;
        }
        tn /= 10;
    }

    // bn_rev(b);

    return BN_OK;
}

// set uint64_t
bnerr bn_set_uint64(bnum* b, uint64_t n) {
    if (n == 0) {
        b->sign = BN_ZERO;
        return BN_OK;
    }
    b->sign = BN_POS;
    uint64_t tn = n;
    while (tn > 0) {
        bnerr err = bn_set_digit(b, (bdigit)tn % 10);
        if (err != BN_OK) {
            return err;
        }
        tn /= 10;
    }

    // bn_rev(b);

    return BN_OK;
}

// set uint32_t
bnerr bn_set_uint32(bnum* b, uint32_t n) {
    if (n == 0) {
        b->sign = BN_ZERO;
        return BN_OK;
    }
    b->sign = BN_POS;
    uint32_t tn = n;
    while (tn > 0) {
        bnerr err = bn_set_digit(b, (bdigit)tn % 10);
        if (err != BN_OK) {
            return err;
        }
        tn /= 10;
    }

    // bn_rev(b);

    return BN_OK;
}

// set int64_t
bnerr bn_set_int64(bnum* b, int64_t n) {
    if (n == 0) {
        b->sign = BN_ZERO;
        return BN_OK;
    }

    int64_t tn = n;
    if (n < 0) {
        b->sign = BN_NEG;
        tn = -tn;  // we must positive the int, otherwise the while loop won't
                   // work
    } else {
        b->sign = BN_POS;
    }

    while (tn > 0) {
        bnerr err = bn_set_digit(b, (bdigit)tn % 10);
        if (err != BN_OK) {
            return err;
        }
        tn /= 10;
    }

    // bn_rev(b);

    return BN_OK;
}

// set long long integer
bnerr bn_set_lli(bnum* b, long long int n) {
    unsigned long long int tn;
    if (n == 0) {
        b->sign = BN_ZERO;
        return BN_OK;
    } else if (n < 0) {
        b->sign = BN_NEG;
        tn = (unsigned long long int)llabs(n);
    } else {
        b->sign = BN_POS;
        tn = (unsigned long long int)n;
    }
    // printf("-------> %lld\n", tn);

    while (tn > 0) {
        bnerr err = bn_set_digit(b, (bdigit)tn % 10);

        if (err != BN_OK) {
            return err;
        }
        tn /= 10;
    }

    // bn_rev(b);

    return BN_OK;
}

/*
 * End Convert Functions
 */
