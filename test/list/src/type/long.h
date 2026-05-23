#ifndef LIST_TYPE_LONG_H
#define LIST_TYPE_LONG_H

// type/long.h — macros for long

#define LIST_SUFFIX       l
#define LIST_TYPE         long
#define LIST_EQ(a, b)     ((a) == (b))
#define LIST_ASSERT_EQ(a, b)  do { LIST_TYPE _a_ = (a), _b_ = (b); TEST_ASSERT_TRUE(LIST_EQ(_a_, _b_)); } while(0)
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand())
#define LIST_SENTINEL_NEG     ((LIST_TYPE)-1L)
#define LIST_SENTINEL_A       ((LIST_TYPE)0xABL)
#define LIST_SENTINEL_B       ((LIST_TYPE)0xCDL)
#define LIST_SENTINEL_MULTI   { -3L, -2L, -1L }
#define LIST_SENTINEL_INSERT  { 10L, 20L, 30L }
#define LIST_SENTINEL_SIZE    { 100L, 200L, 300L }
#define LIST_VALUE_IDENTITY(a, b)  ((a) == (b))
#define LIST_SENTINEL_FROM_IDX(i)  ((LIST_TYPE)(-(long)(i)))
#define LIST_CMP_GT(a, b)          ((a) > (b))
#define LIST_CMP_LT(a, b)          ((a) < (b))


#endif
