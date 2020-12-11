#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <assert.h>

#define DEBUG

#ifdef DEBUG
    #define dbg_assert(...) assert(__VA_ARGS__)
    #define dbg_print(...) printf(__VA_ARGS__)
#else
    #define dbg_assert(...)
    #define dbg_print(...)
#endif

#endif /* _DEBUG_H_ */

