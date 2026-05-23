#ifndef LIST_TYPE_CHAR_H
#define LIST_TYPE_CHAR_H

// type/char.h — macros for unsigned char

#define LIST_SUFFIX       c
#define LIST_TYPE         unsigned char
#define LIST_EQ(a, b)     ((a) == (b))
#define LIST_ASSERT_EQ(a, b)  TEST_ASSERT_EQUAL_INT((a), (b))
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)((i) & 0x7F))
#define LIST_RAND()           ((LIST_TYPE)(rand() & 0x7F))
#define LIST_SENTINEL_NEG     ((LIST_TYPE)0xFF)
#define LIST_SENTINEL_A       ((LIST_TYPE)0xAB)
#define LIST_SENTINEL_B       ((LIST_TYPE)0xCD)
#define LIST_SENTINEL_MULTI   { (unsigned char)0xFD, (unsigned char)0xFE, (unsigned char)0xFF }
#define LIST_SENTINEL_INSERT  { 10, 20, 30 }
#define LIST_SENTINEL_SIZE    { 100, 110, 120 }
#define LIST_VALUE_IDENTITY(a, b)  ((a) == (b))
#define LIST_SENTINEL_FROM_IDX(i)  ((LIST_TYPE)(0xFE - (unsigned int)((i) & 0xFF)))
#define LIST_CMP_GT(a, b)          ((a) > (b))
#define LIST_CMP_LT(a, b)          ((a) < (b))

#define LIST_MAX_SZ 1


#endif
