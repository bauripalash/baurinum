#ifndef baurinum_tests_helper_h
#define baurinum_tests_helper_h

#include "../lib/baurinum.h"

typedef struct T {
    char* name;
    bnum got;
    bnum want;
} T;

T tc(char* name, bnum got, bnum want) {
    return (T){.name = name, .got = got, .want = want};
}
#endif
