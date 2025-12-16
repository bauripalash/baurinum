#include "baurinum.h"
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// the digits are stored are for intger 123 is
// [3,2,1]
// array [a,b,c,d] will represent integer as
// a + b * BASE + c * BASE * BASE + d * BASE * BASE * BASE

typedef uint8_t u8;

typedef enum {
	BN_SIGN_NEG,
	BN_SIGN_POS,
}BnSign;

typedef struct BnInt_t{
	int sign;
	size_t cap;
	size_t len;
	u8 * digits;
}BnInt;

#define BnAlloc(sz) malloc(sz)
#define BnRealloc(ptr, sz) realloc(ptr, sz)
#define BnFree(ptr) free(ptr)

#define BN_DEF_BASE 10
#define BN_DEF_PREC 16

typedef enum BnIntError {
	BNI_ERR_OK = 0,
	BNI_ERR_MEM = 1,
	BNI_ERR_INVALID_CHAR = 2
}BnIntError;

BnInt * NewBnInt(BnIntError * err){
	BnInt * in = BnAlloc(sizeof(BnInt));
	if (in == NULL) {
		return NULL;
	}

	in->cap = BN_DEF_PREC;
	in->len = 0;
	in->digits = BnAlloc(sizeof(u8) * in->cap);

	if (in->digits == NULL) {
		*err = BNI_ERR_MEM;
		BnFree(in);
		return NULL;
	}

	memset(in->digits, 0, sizeof(u8) * in->cap);

	in->sign = 0;
	return in;
}

// Reset the intger, while keeping the capacity
BnIntError BnIntClear(BnInt * bint){
	if (bint == NULL) {
		return BNI_ERR_OK;
	}

	if (bint->digits != NULL) {
		for (size_t i = 0; i < bint->len; i++) {
			bint->digits[i] = 0;
		}
	}
	

	bint->len = 0;
	bint->sign = 0;

	return BNI_ERR_OK;
}

// Free the intger. First Reset than clear
void FreeBnInt(BnInt * bint){
	if (bint != NULL) {
		BnIntClear(bint);
		if (bint->digits != NULL) {
			BnFree(bint->digits);
		}
		BnFree(bint);
	}
}

// Grow BnInt so it can hold at least `c` digits
BnIntError BnIntGrow(BnInt * bint, size_t c){
	if (bint == NULL) {
		return BNI_ERR_MEM;
	}

	if (bint->cap >= (size_t)c) {
		return BNI_ERR_OK;
	}

	size_t newcap = bint->cap * 2; //Temporary, should handle `c`
	u8 * temp = BnRealloc(bint->digits, newcap);
	if (temp == NULL) {
		return BNI_ERR_MEM;
	}

	bint->digits = temp;
	bint->cap = newcap;

	return BNI_ERR_OK;
}

BnIntError bnIntPushDigit(BnInt * bi, u8 digit){
	if (bi == NULL || bi->digits == NULL) {
		return BNI_ERR_MEM;
	}

	if ((bi->len + 1) >= bi->cap) {
		BnIntError err = BnIntGrow(bi, bi->len + 1);
		if (err != BNI_ERR_OK){
			return err;
		}
	}

	bi->digits[bi->len++] = digit;
	return BNI_ERR_OK;
}

BnIntError BnIntSetStr(BnInt * bi, const char * str, size_t slen){
	if (bi == NULL || bi->digits == NULL) {
		return BNI_ERR_MEM;
	}

	if (str == NULL || str[0] == '\0') {
		return BNI_ERR_OK;
	}

	if (slen == 0) {
		return BNI_ERR_OK;
	}
	
	BnIntClear(bi);

	if (bi->cap <= slen) {
		BnIntGrow(bi, slen);
	}

	const char * ptr = str;

	if (*ptr == '-') {
		bi->sign = -1;
		ptr++;
	} else if (*ptr == '+'){
		bi->sign = 1;
		ptr++;
	}else{
		bi->sign = 1;
	}

	while (*ptr == '0') {
		ptr++;
	}

	// the string is filled with zeros
	if (*ptr == '\0') {
		bi->len = 1;
		bi->digits[0] = 0;
		bi->sign = 1; // zero is never negetive
		return BNI_ERR_OK;
	}

	size_t count = 0; //how many valid digits
	const char * temp = ptr;

	while (*temp) {
		if (!isdigit(*temp)) {
			return BNI_ERR_INVALID_CHAR;
		}

		count++;
		temp++;
	}

	bi->len = count;
	for (size_t i = 0; i < count; i++) {
		bi->digits[i] = (u8)(ptr[count - i - 1] - '0'); //reverse iteration
	}

	return BNI_ERR_OK;
}

void BnIntPrint(const BnInt * bi){
	if (bi == NULL || bi->digits == NULL) {
		return;
	}

	if (bi->sign == -1) {
		printf("-");
	}

	for (size_t i = bi->len; i > 0; i--) {
		printf("%d", bi->digits[i - 1]);
	}
}

BnInt * BnIntAbsAdd(const BnInt * ba, const BnInt * bb, BnIntError * err){
	BnInt * result = NewBnInt(err);
	if (*err != BNI_ERR_OK) {
		return NULL;
	}

	u8 carry = 0;
	size_t maxLen = ba->len > bb->len ? ba->len : bb->len;

	for (size_t i = 0; i < maxLen; i++) {
		// get digit or 0 if count is smaller than maxLen 
		// aka. number with most digits
		u8 d1 = ba->len > i ? ba->digits[i] : 0; 		
		u8 d2 = bb->len > i ? bb->digits[i] : 0;

		u8 sum = (u8)(d1 + d2 + carry);
		u8 res = sum % 10;
		*err = bnIntPushDigit(result, res);
		if (*err != BNI_ERR_OK) {
			return NULL;
		}

		carry = sum / 10;
	}
	if (carry > 0) {
		*err = bnIntPushDigit(result, carry);
		if (*err != BNI_ERR_OK) {
			return NULL;
		}
	}

	return result;
}

// -1 => ba < bb
// 1 => ba > bb
// 0 => ba == bb
static inline bool IsIntNull(const BnInt * ba) {
	return (ba == NULL || ba->digits == NULL);
}

int BnIntAbsCompare(const BnInt * ba, const BnInt * bb){
	if (ba->len < bb->len) {
		return -1;
	}

	if (ba->len > bb->len) {
		return 1;
	}

	//same length
	for (size_t i = ba->len; i > 0; i--) {
		u8 digitA = ba->digits[i-1];
		u8 digitB = bb->digits[i-1];

		if (digitA < digitB) {
			return -1;
		}

		if (digitA > digitB) {
			return 1;
		}
	}

	return 0;
}

int BnIntCompare(const BnInt * ba, const BnInt * bb){
	if (IsIntNull(ba) || IsIntNull(bb)) {
		if (IsIntNull(ba) && IsIntNull(bb)) {
			return 0; // both are null. so equal I guess ?
		}

		if (IsIntNull(ba)) {
			//ba is null but bb is not null, then ofcouse ba is smaller
			return -1; 		
		}
		// ba is not null but bb is null, so ba is bigger
		return 1; 	
	}

	// -1 means negetive number
	// 0 means zero
	// +1 means positive number
	
	if (ba->sign > bb->sign) {
		return 1;
	}

	if (ba->sign < bb->sign){
		return -1;
	}

	// it means both have same sign
	int absCompare = BnIntAbsCompare(ba, bb);
	if (ba->sign == -1) {
		return -absCompare; // if both are negetive than reverse
	}


	return absCompare;
}

int main(void){
	BnIntError err;
	BnInt * it = NewBnInt(&err);
	BnInt * et = NewBnInt(&err);
	char * sint = "-123";
	err = BnIntSetStr(it, sint, strlen(sint));
	sint = "-124";
	err = BnIntSetStr(et, sint, strlen(sint));
	BnIntPrint(it);
	printf("\n");
	BnIntPrint(et);
	printf("\n");
	BnInt * result = BnIntAbsAdd(it, et, &err);

	printf("Compare -> %d\n", BnIntCompare(it, et));

	BnIntPrint(result);
	FreeBnInt(result);
	FreeBnInt(et);
	FreeBnInt(it);
	return 0;
}

