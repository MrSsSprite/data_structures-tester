#ifndef TEST_FIND_TEMP_H
#define TEST_FIND_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"
#include <stdlib.h>

/*--------------------------- Private Declarations ---------------------------*/
static void test_find_hit(size_t sz);
static void test_find_miss(size_t sz);
static void test_find_empty(size_t sz);
static void test_find_null_cmp(size_t sz);
static void test_find_custom_cmp(size_t sz);
static void test_find_dupes(size_t sz);
static void test_find_start_at(size_t sz);
static int find_cmp(const void *a, const void *b);
static int find_never_match(const void *a, const void *b);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_find)(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
   {
      test_find_hit(list_sz);
      test_find_miss(list_sz);
      test_find_empty(list_sz);
      test_find_null_cmp(list_sz);
      test_find_custom_cmp(list_sz);
      test_find_dupes(list_sz);
      test_find_start_at(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*------------------------------- Comparators --------------------------------*/
static int find_cmp(const void *a, const void *b)
{
   LIST_TYPE va = *(const LIST_TYPE *)a, vb = *(const LIST_TYPE *)b;
   return (va > vb) - (va < vb);
}

static int find_never_match(const void *a, const void *b)
{
   (void)a; (void)b;
   return 1;
}
/*----------------------------- Comparators END ------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_find_hit(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *result;

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(sz - 1), NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result, "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' missed head value");
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(sz / 2), NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result, "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' missed middle value");
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz / 2), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result, "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' missed tail value");
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);

   LIST__FUNC(deinit)(list);
}

static void test_find_miss(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *result;

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_SENTINEL_NEG, NULL);
   TEST_ASSERT_NULL_MESSAGE(result, "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' returned non-NULL for foreign value");

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), (LIST_TYPE)sz, NULL);
   TEST_ASSERT_NULL_MESSAGE(result, "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' returned non-NULL for foreign value");

   LIST__FUNC(deinit)(list);
}

static void test_find_empty(size_t sz)
{
   if (sz != 1) return;

   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   LIST__NODE_TYPE *result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' returned non-NULL on empty list");

   LIST__FUNC(deinit)(list);
}

static void test_find_null_cmp(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *result;

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_SENTINEL_NEG, NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' with NULL cmp returned non-NULL for non-existing value");

   LIST__FUNC(deinit)(list);
}

static void test_find_custom_cmp(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *result;

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), find_cmp);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), find_never_match);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' with never_match cmp returned non-NULL");

   LIST__FUNC(deinit)(list);
}

static void test_find_dupes(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(0)));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   LIST__NODE_TYPE *result;

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);
   TEST_ASSERT_EQUAL_PTR(list->head, *result);

   result = LIST__FUNC(find)(&(*result)->next, LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);
   TEST_ASSERT_TRUE(list->head != *result);

   result = LIST__FUNC(find)(&(*result)->next, LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' returned non-NULL after last duplicate");

   LIST__FUNC(deinit)(list);
}

static void test_find_start_at(size_t sz)
{
   if (sz < 2) return;
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
   for (size_t i = 0; i < sz / 2; i++)
      pos = &(*pos)->next;

   LIST__NODE_TYPE *result;

   result = LIST__FUNC(find)(pos, LIST_VAL_FROM_IDX(sz - 1), NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' found value before start position");

   result = LIST__FUNC(find)(pos, LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result,
                                "`" LIST__STR_EXPAND(LIST__FUNC(find)) "' missed value after start position");
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);

   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
