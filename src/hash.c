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
#include "llist.h"
#include "debug.h"

typedef struct lib_hash lhash;
struct lib_hash {
    size_t size;
    size_t capacity;
    linked_list_t* table;
    key_equiv_fn_t equiv_fn;
    key_hash_fn_t hash_fn;
    item_print_fn_t print_fn;
};

/*---------------------------------------------------------------------------*/

/**
 * @brief hash table allocation function.
 * @param capacity
 * @param equiv_fn
 * @param hash_fn
 * @param print_fn
 * @return hash
 */
lhash* hash_create(size_t capacity,
                   key_equiv_fn_t equiv_fn,
                   key_hash_fn_t hash_fn,
                   item_print_fn_t print_fn) {
    dbg_assert(equiv_fn != NULL);
    dbg_assert(hash_fn != NULL);
    dbg_assert(print_fn != NULL);

    lhash *hash = (lhash *)malloc(sizeof(lhash));
    linked_list_t* table = (linked_list_t*)malloc(capacity*sizeof(linked_list_t));

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

void print_pass(data_t data) { return; }

/**
 * @brief hash table insertion function.
 * @param hash
 * @param k
 * @param v
 * @return void
 */
void hash_insert(lhash *hash, key_t k, val_t v) {
    dbg_assert(hash != NULL);

    data_t* kv_pair = (data_t*)malloc(2*sizeof(data_t));
    dbg_assert(kv_pair != NULL);

    kv_pair[0] = (data_t)k;
    kv_pair[1] = (data_t)v;
    hash->size++;

    int index = hash->hash_fn(k) % hash->capacity;

    if (hash->table[index] == NULL) {
        hash->table[index] = list_create((data_t)kv_pair, &print_pass);
        return;
    }

    list_append(hash->table[index], (data_t)kv_pair);
    return;
}

/**
 * @brief hash table removal function.
 * @param hash
 * @param k
 * @return v
 */
val_t hash_remove(hash_t hash, key_t k) {
    dbg_assert(hash != NULL);

    int index = hash->hash_fn(k) % hash->capacity;

    linked_list_t list = hash->table[index];
    dbg_assert(list != NULL);

    size_t len = list_getlen(list);

    for (size_t i = 0; i < len; i++) {
        data_t data = list_pop(list);

        data_t* kv_pair = (data_t*)data;
        key_t _k = (key_t)(kv_pair[0]);
        val_t _v = (val_t)(kv_pair[1]);

        if (hash->equiv_fn(_k, k)) {
            free(kv_pair);
            hash->size--;
            return _v;
        } else {
            list_prepend(list, data);
        }
    }

    return NULL;
}

/**
 * @brief hash table update function.
 * @param hash
 * @param k
 * @param v
 * @return void
 */
void hash_update(lhash *hash, key_t k, val_t v) {
    dbg_assert(hash != NULL);

    int index = hash->hash_fn(k) % hash->capacity;

    linked_list_t list = hash->table[index];
    dbg_assert(list != NULL);

    size_t len = list_getlen(list);

    for (size_t i = 0; i < len; i++) {
        data_t data = list_pop(list);
        list_prepend(list, data);

        data_t* kv_pair = (data_t*)data;
        key_t _k = (key_t)(kv_pair[0]);
        val_t _v = (val_t)(kv_pair[1]);

        if (hash->equiv_fn(_k, k)) {
            kv_pair[1] = (data_t)v;
            return;
        }
    }

    return;
}

/**
 * @brief hash table search function.
 * @param hash
 * @param k
 * @return v
 */
val_t hash_search(lhash *hash, key_t k) {
    dbg_assert(hash != NULL);

    int index = hash->hash_fn(k) % hash->capacity;

    linked_list_t list = hash->table[index];
    dbg_assert(list != NULL);

    size_t len = list_getlen(list);

    for (size_t i = 0; i < len; i++) {
        data_t data = list_pop(list);

        data_t* kv_pair = (data_t*)data;
        key_t _k = (key_t)(kv_pair[0]);
        val_t _v = (val_t)(kv_pair[1]);
        list_prepend(list, data);

        if (hash->equiv_fn(_k, k)) {
            return _v;
        }
    }

    return NULL;
}

/**
 * @brief hash table print function.
 * @param hash
 * @return void
 */
void hash_print(lhash *hash) {
    printf("\033[1m\033[31m|-------------------------------\n");
    for (int i = 0; i < hash->capacity; i++) {
        printf("|[%d]:\033[0m", i);

        linked_list_t list = hash->table[i];
        if (list == NULL) { continue; }

        size_t len = list_getlen(list);

        for (size_t i = 0; i < len; i++) {
            data_t data = list_pop(list);
            list_prepend(list, data);

            data_t* kv_pair = (data_t*)data;
            printf(" ");
            hash->print_fn((key_t)(kv_pair[0]), (val_t)(kv_pair[1]));
        }
        printf("\n");
    }
    printf("\033[1m\033[31m|-------------------------------\n\033[0m");
}

/**
 * @brief hash table free function.
 * @param hash
 * @return void
 */
void hash_destroy(lhash *hash) {
    dbg_assert(hash != NULL);

    for (int i = 0; i < hash->capacity; i++) {
        linked_list_t list = hash->table[i];
        if (list == NULL) { continue; }

        size_t len = list_getlen(list);

        for (size_t i = 0; i < len; i++) {
            data_t data = list_pop(list);

            data_t* kv_pair = (data_t*)data;
            free(kv_pair);
        }

        list_destroy(list);
    }

    free(hash);
}
