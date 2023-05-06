# Baurinum Documentation

## Get Started
Using the library in your own project is very simple, just copy all the files 
in `lib` folder; the `baurinum.h` header contains all the function definitions.
Use it as is, or move it to your Include directory, and you can use `-I..` flag
to compile, then compile and link the `.c` files to your own project. If you 
want you can also convert to a static `.a` library using `ar` tool.

#### Basic Example
```c 
#include <stdio.h>
#include "baurinum.h"   // <-- 1
int main(){

    bnum a;
    bnum b;
    
    bn_boot(&a);        // <-- 2
    bn_boot(&b);        // <-- 3

    bn_set_int(&a , 123);   // <-- 4
    bn_set_str(&b , "456"); // <-- 5

    bnum c;

    bn_boot(&c);

    bn_add(&c , &a , &b);
        
    char * result_string = bn_get_str(&c , true); // <-- 6

    printf("%s" , result_string); //Output: 579

    free(result_string); // <-- 7

    bn_clear(&a);  // <-- 8
    bn_clear(&b);
    bn_clear(&c);

    return 0;
}
```

> **(1)** -> Include the header file; which contains function definitions.
> **(2)**/**(3)**... -> Initialize `a` , `b` bnums;
> **(4)** -> Set the value of `a` to 123; from int;
> **(5)** -> Set the value of `b` to 456; from string;
> **(6)** -> Convert `c` to string 
> **(7)** -> Free the string from **(6)**
> **(8)** -> Clear and Free `a`, `b` and `c`;
