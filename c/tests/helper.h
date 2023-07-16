#ifndef baurinum_tests_helper_h
#define baurinum_tests_helper_h

#include <stdbool.h>
#include <stdio.h>

#include "../lib/baurinum.h"

// test type
typedef enum { eq = 0, neq = 1, gt = 2, lt = 3 } tt;
typedef struct T {
    char* name;
    bnum got;
    bnum want;
    tt type;
} T;

T tc(char* name, tt type, bnum got, bnum want) {
    return (T){.name = name, .got = got, .want = want, .type = type};
}

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

bnum _add(bnum x, bnum y) {
    bnum result;
    bn_boot(&result);
    bn_add(&result, &x, &y);
    return result;
}

bnum _sub(bnum x, bnum y) {
    bnum result;
    bn_boot(&result);
    bn_sub(&result, &x, &y);
    return result;
}

bool test(T t) {
    switch (t.type) {
        case eq:
            return bn_cmp(&t.got, &t.want) == BN_EQ;
        case neq:
            return bn_cmp(&t.got, &t.want) != BN_EQ;
        case gt:
            return bn_cmp(&t.got, &t.want) == BN_GT;
        case lt:
            return bn_cmp(&t.got, &t.want) == BN_LT;
    }
}

char* gettype(tt t) {
    switch (t) {
        case eq:
            return "=";
        case neq:
            return "!=";
        case gt:
            return ">";
        case lt:
            return "<";
    }
}

// show result
void r(T cur) {
    bool res = test(cur);
    if (res) {
        printf("[+] PASS : %s\n", cur.name);
    } else {
        printf("[X] FAIL : %s\n", cur.name);
        printf("-> ");
        bn_print(&cur.want);
        printf(" not '%s' ", gettype(cur.type));
        bn_print(&cur.got);
        printf("\n");
    }

    bn_clear(&cur.got);
    bn_clear(&cur.got);
}

void it(T tcs[], int len) {
    for (int i = 0; i < len; i++) {
        T cur = tcs[i];
        r(cur);
    }
}

#endif
