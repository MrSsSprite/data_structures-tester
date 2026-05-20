#ifndef __MACRO_UTILS_H
#define __MACRO_UTILS_H


// Concatenation indirection (required by C preprocessor)
// Inner layer: does the actual ## pasting (arguments NOT expanded)
#define LIST__CAT2(a, b)       a##b
#define LIST__CAT3(a, b, c)    a##b##c
#define LIST__CAT4(a, b, c, d) a##b##c##d

// Outer layer: arguments ARE expanded before reaching the inner layer
#define LIST__CAT2_EXPAND(a, b)       LIST__CAT2(a, b)
#define LIST__CAT3_EXPAND(a, b, c)    LIST__CAT3(a, b, c)
#define LIST__CAT4_EXPAND(a, b, c, d) LIST__CAT4(a, b, c, d)

// Type names
#define LIST__TYPE               LIST__CAT2_EXPAND(List__, LIST_SUFFIX)          // List__i
#define LIST__NODE_TYPE          LIST__CAT3_EXPAND(List_Node__, LIST_SUFFIX, )   // List_Node__i
#define LIST__STRUCT_TAG         LIST__CAT2_EXPAND(list__, LIST_SUFFIX)          // list__i
#define LIST__NODE_STRUCT_TAG    LIST__CAT3_EXPAND(list_node__, LIST_SUFFIX, )   // list_node__i

// Function names
#define LIST__FUNC(name)       LIST__CAT4_EXPAND(list__, LIST_SUFFIX, _, name) // list__i_init

// Test function names (public entry points)
#define LIST__TEST(name)       LIST__CAT3_EXPAND(name, __, LIST_SUFFIX)        // test_push__i

// Stringification helpers — two layers so LIST__FUNC et al. expand before stringify
#define LIST__STR(name)             #name
#define LIST__STR_EXPAND(name)      LIST__STR(name)


#endif
