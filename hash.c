/** @file hash.c
 *  @brief A simple hash dictionary
 *
 *  This is the implementation of a
 *  simple hash dictionary.
 *
 *  @author Joshua Bas (jnbas)
 *  @date March 16, 2020
 *  @bug No know bugs.
 */

#include <stdlib.h>
#include "hash.h"
#include "debug.h"

struct lib_chain {
    key_t k;                /**< Key for this chain */
    val_t v;                /**< Value for the chain */
    struct lib_chain *prev; /**< Previous chain */
    struct lib_chain *next; /**< Next chain */
};
typedef struct lib_chain lchain;

struct lib_hash {
    int size;
    int capacity;
    lchain **table;
    key_equiv_fn_t equiv_fn;
    key_hash_fn_t hash_fn;
    item_print_fn_t print_fn;
};
typedef struct lib_hash lhash;

lchain *chain_new(key_t k, val_t v);
void chain_insert(lchain *base, lchain *chain);
lchain *chain_pop(lchain *chain);
void chain_free(lchain *chain);

/*---------------------------------------------------------------------------*/

/**
 * chain constructor function
 * @param k
 * @param v
 * @return chain, a dynamically allocated lchain pointer
 */
lchain *chain_new(key_t k, val_t v) {
    lchain *chain = (lchain *)malloc(sizeof(lchain));
    chain->k = k;
    chain->v = v;
    chain->prev = NULL;
    chain->next = NULL;

    dbg_assert(chain != NULL);
    return chain;
}

void chain_insert(lchain *base, lchain *chain) {
    dbg_assert(base != NULL);
    dbg_assert(base->prev == NULL);
    dbg_assert(chain != NULL);
    
    base->prev = chain;
    chain->next = base;
    chain->prev = NULL;
    return;
}

lchain *chain_pop(lchain *chain) {
    dbg_assert(chain != NULL);
    
    if (chain->prev != NULL) {
        lchain *prev = chain->prev;
        lchain *next = chain->next;
        prev->next = next;
    }

    if (chain->next != NULL) {
        lchain *prev = chain->prev;
        lchain *next = chain->next;
        next->prev = prev;
    }

    return chain;
}

void chain_free(lchain *chain) {
    // does not free any of chain members,
    // because they can still be referenced
    // by other chains!
    dbg_assert(chain != NULL);

    free(chain);
}

/*---------------------------------------------------------------------------*/

lhash *hash_new(int capacity,
                key_equiv_fn_t equiv_fn,
                key_hash_fn_t hash_fn,
                item_print_fn_t print_fn) {
    dbg_assert(capacity > 0);
    dbg_assert(equiv_fn != NULL);
    dbg_assert(hash_fn != NULL);
    dbg_assert(print_fn != NULL);

    lhash *hash = (lhash *)malloc(sizeof(lhash));
    lchain **table = (lchain **)malloc(capacity*sizeof(lchain *));

    hash->size = 0;
    hash->capacity = capacity;
    hash->table = table;
    hash->equiv_fn = equiv_fn;
    hash->hash_fn = hash_fn;
    hash->print_fn = print_fn;

    for (int i = 0; i < capacity; i++) {
        hash->table[i] = NULL;
    }

    dbg_assert(hash != NULL);
    dbg_assert(hash->table != NULL);
    return hash;
}

void hash_insert(lhash *hash, key_t k, val_t v) {
    dbg_assert(hash != NULL);

    lchain *chain = chain_new(k, v);
    hash->size++;

    int index = hash->hash_fn(chain->k) % hash->capacity;
    
    if (hash->table[index] == NULL) {
        hash->table[index] = chain;
        return;
    }

    chain_insert(hash->table[index], chain);
    hash->table[index] = chain;
}

val_t hash_remove(hash_t hash, key_t k) {
    dbg_assert(hash != NULL);

    int index = hash->hash_fn(k) % hash->capacity;

    lchain *temp = hash->table[index];
    while (temp != NULL) {
        if (hash->equiv_fn(temp->k, k)) {
            val_t v = temp->v;
            chain_pop(temp);
            
            if (hash->table[index] == temp) {
                hash->table[index] = temp->next;
            }

            temp->prev = NULL;
            temp->next = NULL;

            chain_free(temp);
            return v;
        }

        temp = temp->next;
    }
    return NULL;
}

void hash_update(lhash *hash, key_t k, val_t v) {
    dbg_assert(hash != NULL);

    int index = hash->hash_fn(k) % hash->capacity;

    lchain *temp = hash->table[index];
    while (temp != NULL) {
        if (hash->equiv_fn(temp->k, k)) {

            if (hash->table[index] != temp) {
                // should improve temporal locality
                chain_pop(temp);
                chain_insert(hash->table[index], temp);
                hash->table[index] = temp;
            }
            temp->v = v;
            return;
        }

        temp = temp->next;
    }
    return;
}

val_t hash_search(lhash *hash, key_t k) {
    dbg_assert(hash != NULL);

    int index = hash->hash_fn(k) % hash->capacity;

    lchain *temp = hash->table[index];
    while (temp != NULL) {
        if (hash->equiv_fn(temp->k, k)) {

            if (hash->table[index] != temp) {
                // should improve temporal locality
                chain_pop(temp);
                chain_insert(hash->table[index], temp);
                hash->table[index] = temp;
            }
            return temp->v;
        }

        temp = temp->next;
    }
    return NULL;
}

void hash_print(lhash *hash) {
    printf("\033[1m\033[31m|-------------------------------\n");
    for (int i = 0; i < hash->capacity; i++) {
        printf("|[%d]:\033[0m", i);

        lchain *temp = hash->table[i];
        while (temp != NULL) {
            printf(" ");
            hash->print_fn(temp->k, temp->v);
            
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\033[1m\033[31m|-------------------------------\n\033[0m");
}

void hash_free(lhash *hash) {
    dbg_assert(hash != NULL);
    
    for (int i = 0; i < hash->capacity; i++) {
        lchain *temp = hash->table[i];
        while (temp != NULL) {
            chain_pop(temp);
            hash->table[i] = temp->next;

            temp->prev = NULL;
            temp->next = NULL;

            chain_free(temp);
            temp = hash->table[i];
        }
    }

    free(hash);
}
