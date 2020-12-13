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
#include <stdio.h>
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

void data_print_fn(data_t data) { (void)data; return; }

// @TODO: need to free keys and values!
void data_free_fn(data_t data) {
    data_t* kv_pair = (data_t *)data;
    free(kv_pair);
    return;
}

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

    lhash* hash = (lhash *)malloc(sizeof(lhash));
    linked_list_t* table = (linked_list_t *)malloc(capacity*sizeof(linked_list_t));

    hash->size = 0;
    hash->capacity = capacity;
    hash->table = table;
    hash->equiv_fn = equiv_fn;
    hash->hash_fn = hash_fn;
    hash->print_fn = print_fn;

    for (size_t i = 0; i < capacity; i++) {
        hash->table[i] = list_create(&data_print_fn, &data_free_fn);
    }

    dbg_assert(hash != NULL);
    dbg_assert(hash->table != NULL);
    return hash;
}

/**
 * @brief hash table insertion function.
 * @param hash
 * @param k
 * @param v
 * @return void
 */
void hash_insert(lhash* hash, key_t k, val_t v) {
    dbg_assert(hash != NULL);

    data_t* kv_pair = (data_t *)malloc(2*sizeof(data_t));
    dbg_assert(kv_pair != NULL);

    kv_pair[0] = (data_t)k;
    kv_pair[1] = (data_t)v;
    hash->size++;

    size_t index = hash->hash_fn(k) % hash->capacity;

    dbg_assert(hash->table != NULL);
    dbg_assert(hash->table[index] != NULL);
    list_append(hash->table[index], (data_t)kv_pair);
    return;
}

/**
 * @brief hash table removal function.
 * @param hash
 * @param k
 * @return v
 */
val_t hash_remove(lhash* hash, key_t k) {
    dbg_assert(hash != NULL);

    size_t index = hash->hash_fn(k) % hash->capacity;

    dbg_assert(hash->table != NULL);
    linked_list_t list = hash->table[index];
    dbg_assert(list != NULL);

    size_t len = list_getlen(list);

    for (size_t i = 0; i < len; i++) {
        data_t data = list_peek_end(list);

        data_t* kv_pair = (data_t *)data;
        dbg_assert(kv_pair != NULL);

        key_t _k = (key_t)(kv_pair[0]);
        val_t _v = (val_t)(kv_pair[1]);

        if (hash->equiv_fn(_k, k)) {
            list_pop(list);
            data_free_fn(data);
            hash->size--;
            return _v;
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
void hash_update(lhash* hash, key_t k, val_t v) {
    dbg_assert(hash != NULL);

    size_t index = hash->hash_fn(k) % hash->capacity;

    dbg_assert(hash->table != NULL);
    linked_list_t list = hash->table[index];
    dbg_assert(list != NULL);

    size_t len = list_getlen(list);

    for (size_t i = 0; i < len; i++) {
        data_t data = list_peek_end(list);

        data_t* kv_pair = (data_t *)data;
        dbg_assert(kv_pair != NULL);

        key_t _k = (key_t)(kv_pair[0]);

        if (hash->equiv_fn(_k, k)) {
            kv_pair[1] = (data_t)v;
            list_pop(list);
            list_prepend(list, data);
            return;
        }
    }

    hash_insert(hash, k, v);

    return;
}

/**
 * @brief hash table search function.
 * @param hash
 * @param k
 * @return v
 */
val_t hash_search(lhash* hash, key_t k) {
    dbg_assert(hash != NULL);

    size_t index = hash->hash_fn(k) % hash->capacity;

    dbg_assert(hash->table != NULL);
    linked_list_t list = hash->table[index];
    dbg_assert(list != NULL);

    size_t len = list_getlen(list);

    for (size_t i = 0; i < len; i++) {
        data_t data = list_peek_end(list);

        data_t* kv_pair = (data_t*)data;
        dbg_assert(kv_pair != NULL);

        key_t _k = (key_t)(kv_pair[0]);
        val_t _v = (val_t)(kv_pair[1]);

        if (hash->equiv_fn(_k, k)) {
            return _v;
        }

        list_cycle(list);
    }

    return NULL;
}

/**
 * @brief hash table print function.
 * @param hash
 * @return void
 */
void hash_print(lhash* hash) {
    dbg_assert(hash != NULL);

    if (hash->print_fn == NULL) { return; }

    printf("|-------------------------------\n");
    for (size_t i = 0; i < hash->capacity; i++) {
        printf("|[%zu]:", i);

        dbg_assert(hash->table != NULL);
        linked_list_t list = hash->table[i];
        dbg_assert(list != NULL);

        size_t len = list_getlen(list);

        for (size_t j = 0; j < len; j++) {
            data_t data = list_peek_end(list);

            data_t* kv_pair = (data_t*)data;
            dbg_assert(kv_pair != NULL);

            printf(" ");
            hash->print_fn((key_t)(kv_pair[0]), (val_t)(kv_pair[1]));

            list_cycle(list);
        }
        printf("\n");
    }
    printf("|-------------------------------\n");
}

/**
 * @brief hash table free function.
 * @param hash
 * @return void
 */
void hash_destroy(lhash* hash) {
    dbg_assert(hash != NULL);

    for (size_t i = 0; i < hash->capacity; i++) {

        dbg_assert(hash->table != NULL);
        linked_list_t list = hash->table[i];

        if (list == NULL) { continue; }
        list_destroy(list);
    }

    free(hash);
}
