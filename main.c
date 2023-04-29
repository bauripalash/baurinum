#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_DIGIT 10
#define GROW_BY 1.5
#define GROW_SIZE(cap) (cap + ((int)cap * GROW_BY))

typedef uint8_t bdigit;

typedef enum {
  BN_POS = 0,
  BN_NEG = 1,
} bnsign;

typedef struct bnum {
  // 0 for >= 0 | 1 for < 0
  bnsign sign;
  int cap;
  int len;
  bdigit* digits;
} bnum;

typedef enum { BN_OK = 1, BN_ERR_NOMEM = 2, BN_UNKNOWN_CHAR = 3 } bnerr;

bnerr bn_boot(bnum* b) {
  b->digits = (bdigit*)calloc((size_t)DEFAULT_DIGIT, sizeof(bdigit));
  if (b->digits == NULL) {
    return BN_ERR_NOMEM;
  }
  b->cap = DEFAULT_DIGIT;
  b->len = 0;
  b->sign = BN_POS;
  return BN_OK;
}

void bn_clear(bnum* b) {
  if (b->digits != NULL) {
    free(b->digits);
    b->sign = BN_POS;
    b->len = 0;
    b->cap = 0;
    b->digits = NULL;
  }
}

bnerr bn_grow(bnum* b) {
  // printf("--alloc--\n");
  int oldsize = b->cap;
  b->cap = GROW_SIZE(oldsize);
  b->digits = realloc(b->digits, b->cap);
  if (b->digits == NULL) {
    return BN_ERR_NOMEM;
  }
  return BN_OK;
}

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

  printf("%c", b->sign == 0 ? '+' : '-');
  for (int i = 0; i < b->len; i++) {
    printf("%d", b->digits[i]);
  }
}

void bn_println(bnum * b){
  bn_print(b);
  printf("\n");
}

void bn_debug(bnum * b){
  printf("[ len->%d | cap->%d | sign->%c | digits->(" , b->len , b->cap , b->sign == 0 ? '+' : '-');
  if (b->digits != NULL) {
  bn_print(b);
  }else{
    printf("NULL");
  }
  printf(")]\n");

}

bnerr bn_set_str(bnum* b, char* str) {
  char* ptr = str;
  int is_neg = 0;
  if (*ptr == '-') {
    is_neg = 1;
    ptr++;
  }
  b->sign = is_neg;
  while (*ptr) {
    if (*ptr <= '9' && *ptr >= 0) {
      bnerr x = bn_set_digit(b, *ptr - '0');
      if (x != BN_OK) {
        return x;
      }
    } else {
      return BN_UNKNOWN_CHAR;
    }
    ptr++;
  }

  return BN_OK;
}

int main(void) {
  bnum x;
  bn_boot(&x);
  //  bn_set_digit(&x, 1);
  bn_set_str(&x, "123456999111111111111111111111111111111111111");
  bn_debug(&x);
  // bn_grow(&x);
  //   bn_grow(&x);
  bn_println(&x);
  bn_clear(&x);
}
