#ifndef LIST_TYPE_FLOAT_H
#define LIST_TYPE_FLOAT_H

#include <math.h>

// type/float.h — macros for float

#define LIST_SUFFIX       f
#define LIST_TYPE         float
#define LIST_EQ(a, b)     (fabsf((a) - (b)) < 1e-6f)
#define LIST_ASSERT_EQ(a, b)  do { LIST_TYPE _a_ = (a), _b_ = (b); TEST_ASSERT_EQUAL_FLOAT(_a_, _b_); } while(0)
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand() / (LIST_TYPE)RAND_MAX)
#define LIST_SENTINEL_NEG     (-1.0f)
#define LIST_SENTINEL_A       (3.14f)
#define LIST_SENTINEL_B       (99.0f)
#define LIST_SENTINEL_MULTI   { -3.0f, -2.0f, -1.0f }
#define LIST_SENTINEL_INSERT  { 10.0f, 20.0f, 30.0f }
#define LIST_SENTINEL_SIZE    { 100.0f, 200.0f, 300.0f }


#endif
