#ifndef LIST_TYPE_DOUBLE_H
#define LIST_TYPE_DOUBLE_H

#include <math.h>

// type/double.h — macros for double

#define LIST_SUFFIX       d
#define LIST_TYPE         double
#define LIST_EQ(a, b)     (fabs((a) - (b)) < 1e-9)
#define LIST_ASSERT_EQ(a, b)  do { LIST_TYPE _a_ = (a), _b_ = (b); TEST_ASSERT_TRUE(LIST_EQ(_a_, _b_)); } while(0)
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand() / (LIST_TYPE)RAND_MAX)
#define LIST_SENTINEL_NEG     (-1.0)
#define LIST_SENTINEL_A       (3.141592653589793)
#define LIST_SENTINEL_B       (99.0)
#define LIST_SENTINEL_MULTI   { -3.0, -2.0, -1.0 }
#define LIST_SENTINEL_INSERT  { 10.0, 20.0, 30.0 }
#define LIST_SENTINEL_SIZE    { 100.0, 200.0, 300.0 }
#define LIST_VALUE_IDENTITY(a, b)  ((a) == (b))
#define LIST_SENTINEL_FROM_IDX(i)  ((LIST_TYPE)(-(int)(i)))
#define LIST_CMP_GT(a, b)          ((a) > (b))
#define LIST_CMP_LT(a, b)          ((a) < (b))


#endif
