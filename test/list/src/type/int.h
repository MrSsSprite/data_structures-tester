#ifndef LIST_TYPE_INT_H
#define LIST_TYPE_INT_H


// type/int.h — macros for int

#define LIST_SUFFIX       i
#define LIST_TYPE         int
#define LIST_EQ(a, b)     ((a) == (b))
#define LIST_ASSERT_EQ(a, b)  TEST_ASSERT_EQUAL_INT((a), (b))
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand())
#define LIST_SENTINEL_NEG     ((LIST_TYPE)-1)
#define LIST_SENTINEL_A       ((LIST_TYPE)0xAB)
#define LIST_SENTINEL_B       ((LIST_TYPE)0xCD)
#define LIST_SENTINEL_MULTI   { -3, -2, -1 }
#define LIST_SENTINEL_INSERT  { 10, 20, 30 }
#define LIST_SENTINEL_SIZE    { 100, 200, 300 }


#endif
