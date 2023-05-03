#ifndef baurinum_h
#define baurinum_h

#include <stdint.h>

#define DEFAULT_DIGIT  10
#define GROW_BY        1.5
#define GROW_SIZE(cap) (cap + ((int)cap * GROW_BY))
#define errcheck     \
 if (err != BN_OK) { \
  return err;        \
 }

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

// convert sign to single char string
//
// convert sign to single char string
// BN_ZERO => `0`
// BN_POS  => `+`
// BN_NEG  => `-`
char* bnsign_to_str(bnsign s);

// Comparison Result
// LT -> <
// GT -> >
// EQ -> ==
typedef enum {
    BN_LT = 0,
    BN_GT = 1,
    BN_EQ = 2,
} bn_comp_result;

// convert bn_comp_result to 2 char string
// LT -> "LT"
// GT -> "GT"
// EQ -> "EQ"
char* cmpres_to_str(bn_comp_result cr);

// The Bnum struct aka `baurinum`
typedef struct bnum {
    // sign of the number (postive/negative/zero)
    // see `bnsign`
    bnsign sign;
    // capacity (total memory allocated)
    int cap;
    // currenly used memory block/how many digits
    int len;
    // pointer to digits
    bdigit* digits;
} bnum;

// Result of most functions
// BN_OK -> No error occured
// BN_ERR_NOMEM -> Failed to allocate memory for some operation(s)
// BN_UNKNOWN_CHAR -> Unknown char found while parsing the string
// to convert it to a `bnum`
typedef enum {
    BN_OK = 1,
    BN_ERR_NOMEM = 2,
    BN_UNKNOWN_CHAR = 3,
    BN_STR_WRITE_FAIL = 4,
    BN_NOT_INIT = 5,
} bnerr;

// Initialize a `bnnum`
// set sign to 'BN_ZERO'
// set cap to `DEFAULT_DIGIT`
// len to 0
bnerr bn_boot(bnum* b);

// Free a number
void bn_clear(bnum* b);

// set the capacity of `b` to b->capacity + size;
// and allocate memory for it.
bnerr bn_grow_by(bnum* b, int size);

// Increase `cap` and allocate more memeory
//  returns `BN_ERR_NOMEM` if failed to allocate memeory
//  else return BN_OK
bnerr bn_grow(bnum* b);

// Set a single digit in bnum
// grow capacity if needed
bnerr bn_set_digit(bnum* b, bdigit dig);

// reverse the endian-ness
void bn_rev(bnum* b);

// set a string (base 10) to `b`
bnerr bn_set_str(bnum* b, char* str);

// set double (wrapper for `bn_set_str`) to `b`
bnerr bn_set_double(bnum* b, double n);

// set long double (wrapper for `bn_set_str`) to `b`
bnerr bn_set_ldouble(bnum* b, long double n);

// set integer to `b`
bnerr bn_set_int(bnum* b, int n);

// set uint64_t to `b`
bnerr bn_set_uint64(bnum* b, uint64_t n);

// set uint32_t to `b`
bnerr bn_set_uint32(bnum* b, uint32_t n);

// set int64_t to `b`
bnerr bn_set_int64(bnum* b, int64_t n);

// set long long integer to `b`
bnerr bn_set_lli(bnum* b, long long int n);

// (unsigned) Compare L and R; does not take sign into account;
bn_comp_result bn_ucmp(const bnum* l, const bnum* r);

//(Signed) compare returns BN_LT , BN_GT , BN_EQ
// see `bn_comp_result`
bn_comp_result bn_cmp(const bnum* l, const bnum* r);

// res = |inp|
bnerr bn_abs(bnum* res, bnum* inp);

// res = -inp
bnerr bn_make_neg(bnum* res, bnum* inp);

// print `b` to stdout with leading sign in the case of positive or negative
void bn_print(bnum* b);

// `bn_print` with newline
void bn_println(bnum* b);

// debug `b` by printing all fields of `b`
void bn_debug(bnum* b);

// trim excess zeros from `b`
void bn_trim(bnum* b);

// Turn `res` into a clone of `input`
bnerr bn_clone(bnum* res, bnum* input);

// Initialize l and make it a clone of r
bnerr bn_dup_boot(bnum* l, bnum* r);

// Zero the digits
void bn_mkzero(bnum* b);
bnerr bn_u_add(bnum* c, const bnum* a, const bnum* b);
bnerr bn_add(bnum* c, const bnum* a, const bnum* b);
bnerr bn_u_sub(bnum* res, const bnum* a, const bnum* b);
bnerr bn_sub(bnum* res, const bnum* a, const bnum* b);
#endif
