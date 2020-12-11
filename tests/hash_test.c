
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hash.h"

int print_fn(key_t k, val_t v) {
    printf("(%c, %c)\n", (char)k, (char)v);
    return 0;
}

int equiv_fn(key_t k1, key_t k2) {
    return (char)k1 == (char)k2;
}

int hash_fn(key_t k) {
    double ck = 0.55*(double)(char)k;
    int hash = (int)floor(21*(ck - floor(ck)));
    return hash;
}

int main() {
    hash_t hash = hash_create(21, &equiv_fn, &hash_fn, &print_fn);

    hash_insert(hash, (key_t)'J', (val_t)18213);
    hash_insert(hash, (key_t)'B', (val_t)18100);
    hash_insert(hash, (key_t)'a', (val_t)18341);
    hash_insert(hash, (key_t)'s', (val_t)18290);

    hash_print(hash);

    //hash_destroy(hash);

}
