#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_DIGIT 10
#define GROW_BY 1.5
#define GROW_SIZE(cap) (cap + ((int)cap * GROW_BY))

// A single digit
typedef uint64_t bdigit;

/*
 * Sign of a Number
 * BN_ZERO = `0` (value is zero) => 0
 * BN_POS  = `+` (positive) => 1
 * BN_NEG  = `-` (negative) => 2
 */
typedef enum {
  BN_ZERO = 0,
  BN_POS = 1,
  BN_NEG = 2,
} bnsign;

char* bnsign_to_str(bnsign s) {
  if (s == BN_ZERO) {
    return "0";
  } else if (s == BN_POS) {
    return "+";
  } else {
    return "-";
  }
}

typedef enum {
  BN_LT = 0,
  BN_GT = 1,
  BN_EQ = 2,
} bn_comp_result;

char* cmpres_to_str(bn_comp_result cr) {
  if (cr == BN_LT) {
    return "LT";
  } else if (cr == BN_GT) {
    return "GT";
  } else {
    return "EQ";
  }
}

typedef struct bnum {
  // 0 for >= 0 | 1 for < 0
  bnsign sign;
  // capacity (total memory allocated)
  int cap;
  // currenly used memory block/how many digits
  int len;
  // pointer to digits
  bdigit* digits;

} bnum;

typedef enum { BN_OK = 1, BN_ERR_NOMEM = 2, BN_UNKNOWN_CHAR = 3 } bnerr;

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

// Free a number
void bn_clear(bnum* b) {
  if (b->digits != NULL) {
    free(b->digits);
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

// Increase `cap` and allocate more memeory
//  returns `BN_ERR_NOMEM` if failed to allocate memeory
//  else return BN_OK
bnerr bn_grow(bnum* b) { return bn_grow_by(b, GROW_SIZE(b->cap)); }

// Set a single digit in bnum
// grow capacity if needed
bnerr bn_set_digit(bnum* b, bdigit dig) {
  if (b->cap <= b->len + 1) {
    bnerr err = bn_grow(b);
    if (err != BN_OK) {
      return err;
    }
  }
  b->digits[b->len] = dig;
  b->len++;
  return BN_OK;
}

void bn_print(bnum* b) {
  if (b->cap == 0 || b->len == 0 || b->digits == NULL) {
    return;
  }

  printf("%c", b->sign != BN_NEG ? '+' : '-');
  for (int i = b->len - 1; i >= 0; i--) {
    printf("%ld", b->digits[i]);
  }
}

void bn_println(bnum* b) {
  bn_print(b);
  printf("\n");
}

void bn_debug(bnum* b) {
  printf("[ len->%d | cap->%d | sign->%c | digits->(", b->len, b->cap,
         b->sign == 0 ? '+' : '-');
  if (b->digits != NULL) {
    bn_print(b);
  } else {
    printf("NULL");
  }
  printf(")]\n");
}

static inline int is_endig(char c) { return c <= '9' && c >= '0'; }

// reverse the endian-ness
void bn_rev(bnum* b) {
  for (int i = 0; i < b->len / 2; i++) {
    int tmp = b->digits[i];
    b->digits[i] = b->digits[b->len - i - 1];
    b->digits[b->len - i - 1] = tmp;
  }
}

// set a string (base 10)
bnerr bn_set_str(bnum* b, char* str) {
  char* ptr = str;
  int is_neg = 0;
  if (*ptr == '-') {
    is_neg = 1;
    ptr++;
  } else {
    b->sign = BN_POS;
  }
  b->sign = is_neg;
  while (*ptr) {
    if (is_endig(*ptr)) {
      bnerr x = bn_set_digit(b, *ptr - '0');
      if (x != BN_OK) {
        return x;
      }
    } else {
      return BN_UNKNOWN_CHAR;
    }
    ptr++;
  }

  bn_rev(b);

  return BN_OK;
}

// set integer
bnerr bn_set_int(bnum* b, int n) {
  int tn = n;
  // printf("setting -> %d\n", n);
  if (n < 0) {
    b->sign = BN_NEG;
    tn = -tn;  // we must positive the int, otherwise the while loop won't work
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
    tn = -tn;  // we must positive the int, otherwise the while loop won't work
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

/*
 * POSTPONED
 *
bnerr bn_set_d(bnum* b, double n) {
  // long long int tn = (long long int)ceil(n);
  // printf("->%g -> %lld\n", n, tn);

  union{
    double dbl;
    uint64_t bits;
  }cast;
  cast.dbl = n;
  uint64_t frac;

  printf("--> %f , %lu | %lu\n" , cast.dbl , cast.bits , UINT64_MAX);
  int exp_t;
  exp_t = (int)((unsigned)(cast.bits >> 52) & 0x7FFu);
  frac = (cast.bits & (((uint64_t)1 << 52) - (uint64_t)1)) | ((uint64_t)1 <<
52); if (exp_t == 0x7FF) { return BN_UNKNOWN_CHAR;
  }
  exp_t -= 1023 + 52;

  printf("->exp -> %d | %lu\n" ,  exp_t , frac);
  //if (n < 0) {
  //  b->sign = BN_NEG;
  //  tn = -n;
  //}



  return BN_OK;
}*/

// POSTPONED
bnerr bn_set_ld(bnum* b, long double n) {
  long long int tn = (long long int)ceill(n);
  return bn_set_lli(b, tn);
}

// POSTPONED
bnerr bn_set_f(bnum* b, float n) {
  long long int tn = (long long int)ceilf(n);
  return bn_set_lli(b, tn);
}

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
bnerr bn_dup_init(bnum* l, bnum* r) {
  bnerr err = bn_boot(l);
  if (err != BN_OK) {
    return err;
  }
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

// res = |inp|
bnerr bn_abs(bnum* res, bnum* inp) {
  if (res != inp) {
    bnerr err = bn_clone(res, inp);
    if (err != BN_OK) {
      return err;
    }
  }

  res->sign = BN_ZERO;
  return BN_OK;
}

// res = -inp
bnerr bn_make_neg(bnum* res, bnum* inp) {
  if (res != inp) {
    bnerr err = bn_clone(res, inp);
    if (err != BN_OK) {
      return err;
    }
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


// from tommath
#define N_DBIT 60
#define N_MASK ((((bdigit)1) << ((bdigit)N_DBIT)) - ((bdigit)1))

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
  if (c->cap < max + 1) {
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

int main(void) {
  bnum x;
  bnum y;
  bnum c;
  bn_boot(&y);
  bn_boot(&x);
  bn_boot(&c);
  bn_set_str(&x, "991");
  bn_set_int(&y, 123);
  bn_add(&c, &x, &y);
  // bn_set_int(&y, 100);
  // bn_clone(&y, &x);
  // bn_mkzero(&x);
  // bn_abs(&y, &x);
  bn_println(&x);
  bn_println(&y);
  bn_println(&c);
  // bn_println(&x);*/
  bn_clear(&y);
  bn_clear(&x);
  bn_clear(&c);
}
