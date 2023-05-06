#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baurinum.h"

char* bn_as_str(bnum* b, bool sign) {
    int len = b->len + 1;
    if (sign) {
        len++;
    }
    int slen = 0;
    int blen = b->len;

    for (int i = 0; i < b->len; i++) {
        slen += snprintf(NULL, 0, "%lu", b->digits[i]);
    }

    if (sign) {
        slen += snprintf(NULL, 0, "%s", bnsign_to_str(b->sign));
    }

    char* str = (char*)calloc(len, sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    char* ptr = str;

    if (sign) {
        int w = snprintf(ptr, slen + 1 - (ptr - str), "%c",
                         b->sign != BN_NEG ? '+' : '-');
        if (w < 0) {
            free(str);
            return NULL;
        }

        ptr++;
    }

    for (int i = 0; i < blen; i++) {
        int wrote = snprintf(ptr, slen + 1 - (ptr - str), "%d",
                             (int)b->digits[blen - i - 1]);
        if (wrote < 0) {
            free(str);
            return NULL;
        }

        ptr += wrote;
    }
    *ptr = '\0';
    return str;
}
