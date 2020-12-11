/** @file hash.h
 *  @brief A simple hash dictionary header file
 *
 *  This is the header file for a
 *  simple hash dictionary.
 *
 *  @author Joshua Bas (jnbas)
 *  @date March 16, 2020
 *  @bug No know bugs.
 */

#ifndef _HASH_H_
#define _HASH_H_

typedef void *key_t;
typedef void *val_t;

typedef int (*key_equiv_fn_t)(key_t k1, key_t k2);
typedef int (*key_hash_fn_t)(key_t k);
typedef int (*item_print_fn_t)(key_t k, val_t v);

typedef struct lib_hash *hash_t;

hash_t hash_new(int capacity,
                key_equiv_fn_t equiv_fn,
                key_hash_fn_t hash_fn,
                item_print_fn_t print_fn);
void hash_insert(hash_t hash, key_t k, val_t v);
void hash_update(hash_t hash, key_t k, val_t v);
val_t hash_remove(hash_t hash, key_t k);
val_t hash_search(hash_t hash, key_t k);
void hash_print(hash_t hash);
void hash_free(hash_t hash);

#endif /* _HASH_H_ */
