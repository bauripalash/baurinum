#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/baurinum.h"
#include "helper.h"

int main() {
    T tcs[] = {
        tc("1.int = 1.str", eq, _i(1), _s("1")),
        tc("1.9.d = 2.int", eq, _d(1.9), _i(2)),
        tc("123.9.d != 123.9.str (1239)", neq, _d(123.9), _s("123.9")),
        tc("100-50 == 50", eq, _sub(_d(100), _d(50)), _d(50)),

        tc("999-123 == 876", eq, _sub(_d(999), _d(123)), _d(876)),

        tc("BIG RANDOM - ANOTHER BIG RANDOM == CORRECT RESULT", eq, _sub(_s("113100395116878660387503642222766133132459372641329388"), _s("113100395116878660387503642222")), _s("113100395116878660387503529122371016253798985137687166")),
    };

    int len = sizeof(tcs) / sizeof(tcs[0]);

    it(tcs, len);

    return EXIT_SUCCESS;
}
