#ifndef LIST_TYPE_UINT_H
#define LIST_TYPE_UINT_H

#include <limits.h>

// type/unsigned_int.h — macros for unsigned int

#define LIST_SUFFIX       u
#define LIST_TYPE         unsigned int
#define LIST_EQ(a, b)     ((a) == (b))
#define LIST_ASSERT_EQ(a, b)  TEST_ASSERT_EQUAL_UINT((a), (b))
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand())
#define LIST_SENTINEL_NEG     ((LIST_TYPE)(UINT_MAX))
#define LIST_SENTINEL_A       ((LIST_TYPE)(UINT_MAX - 1))
#define LIST_SENTINEL_B       ((LIST_TYPE)(UINT_MAX - 2))
#define LIST_SENTINEL_MULTI   { (unsigned int)(UINT_MAX - 3), (unsigned int)(UINT_MAX - 4), (unsigned int)(UINT_MAX - 5) }
#define LIST_SENTINEL_INSERT  { 10u, 20u, 30u }
#define LIST_SENTINEL_SIZE    { 100u, 200u, 300u }
#define LIST_VALUE_IDENTITY(a, b)  ((a) == (b))
#define LIST_SENTINEL_FROM_IDX(i)  ((LIST_TYPE)(UINT_MAX - 1000u - (unsigned int)(i)))
#define LIST_CMP_GT(a, b)          ((a) > (b))
#define LIST_CMP_LT(a, b)          ((a) < (b))


#endif
