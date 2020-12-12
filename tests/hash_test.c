
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hash.h"

#define TABLE_SIZE 10

int print_fn(key_t k, val_t v) {
    printf("(%c, %d)", (char)k, (int)v);
    return 0;
}

int equiv_fn(key_t k1, key_t k2) {
    return (char)k1 == (char)k2;
}

int hash_fn(key_t k) {
    double ck = 0.55*(double)(char)k;
    int hash = (int)floor(TABLE_SIZE*(ck - floor(ck)));
    return hash;
}

int main() {
    hash_t hash = hash_create(TABLE_SIZE, &equiv_fn, &hash_fn, &print_fn);

    for (char i = 0; i < 26; i++) {
        hash_insert(hash, (key_t)(long)('a'+i), (val_t)(long)((i+13)*100 % 14));
    }
    hash_print(hash);

    hash_destroy(hash);
    return 0;
}
