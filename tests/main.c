#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/baurinum.h"
#include "helper.h"

bnum _s(char* str) {
    bnum num;
    bn_boot(&num);
    bn_set_str(&num, str);
    return num;
}

bnum _i(int x) {
    bnum num;
    bn_boot(&num);
    bn_set_int(&num, x);
    return num;
}

bnum _d(double x) {
    bnum num;
    bn_boot(&num);
    bn_set_double(&num, x);
    return num;
}

bnum _add(bnum* x, bnum* y) {
    bnum result;
    bn_boot(&result);
    bn_add(&result, x, y);
    return result;
}

bool eq(bnum* a, bnum* b) { return bn_cmp(a, b) == BN_EQ; }

int main() {
    T tcs[] = {
        tc("1.int = 1.str", _i(1), _s("1")),
        tc("1.9.d = 2.int", _d(1.9), _i(2)),
    };

    int len = sizeof(tcs) / sizeof(tcs[0]);

    for (int i = 0; i < len; i++) {
        T cur = tcs[i];
        bool res = eq(&cur.got, &cur.want);
        if (res) {
            printf("[+] PASS : %s\n", cur.name);
        } else {
            printf("[X] FAIL : %s\n", cur.name);
            printf("Wanted -> ");
            bn_print(&cur.want);
            printf(" | Got -> ");
            bn_print(&cur.got);
            printf("\n");
        }

        bn_clear(&cur.got);
        bn_clear(&cur.got);
    }

    return EXIT_SUCCESS;
}
