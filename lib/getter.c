#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "baurinum.h"

char* bn_as_str(bnum* b, bool sign) {
    int len = b->len + 1;
    //
    int slen = 0;

    for (int i = 0; i < b->len; i++) {
        slen += snprintf(NULL, 0, "%lu", b->digits[i]);
    }

    if (sign) {
        len++;
        slen += snprintf(NULL, 0, "%s", bnsign_to_str(b->sign));
    }

    char* str = (char*)calloc(len, sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    char* ptr = str;

    if (sign) {
        int w =
            snprintf(ptr, slen + 1 - (ptr - str), "%s", bnsign_to_str(b->sign));
        // printf("w->%d\n" , w);
        if (w < 0) {
            free(str);
            return NULL;
        }

        ptr++;
        // off += w;
    }

    for (int i = 0; i < len; i++) {
        printf("_s_t_r->'%s'\n", str);
        int wrote = snprintf(ptr, slen + 1 - (ptr - str), "%lu", b->digits[i]);
        if (wrote < 0 || wrote > 1) {
            free(str);
            return NULL;
        }

        // off += wrote;
        ptr += wrote;
    }
    return str;
}
