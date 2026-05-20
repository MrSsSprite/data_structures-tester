#ifndef TEST_POP_TEMP_H
#define TEST_POP_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"
#include <stdlib.h>

/*--------------------------- Private Declarations ---------------------------*/
static void test_pop_once(void);
static void test_pop_seq(size_t sz);
static void test_pop_all(size_t sz);
static void test_pop_interleaved(size_t sz);
static void test_pop_rebuild(size_t sz);
static void test_pop_after_insert(size_t sz);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_pop)(void)
{
   test_pop_once();

   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
   {
      test_pop_seq(list_sz);
      test_pop_all(list_sz);
      test_pop_interleaved(list_sz);
      test_pop_rebuild(list_sz);
      test_pop_after_insert(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_pop_once(void)
{
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(0)));
   TEST_ASSERT_EQUAL_size_t(1, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), list->head->value);

   LIST__FUNC(pop)(list);
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not NULL after popping last element");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not 0 after popping last element");

   LIST__FUNC(deinit)(list);
}

static void test_pop_seq(size_t sz)
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

   for (size_t i = sz; i; i--)
   {
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i - 1), list->head->value);
      TEST_ASSERT_EQUAL_size_t(i, list->size);
      LIST__FUNC(pop)(list);
   }
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   LIST__FUNC(deinit)(list);
}

static void test_pop_all(size_t sz)
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

   for (size_t i = 0; i < sz; i++)
      LIST__FUNC(pop)(list);

   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not NULL after popping all elements");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not 0 after popping all elements");

   LIST__FUNC(deinit)(list);
}

static void test_pop_interleaved(size_t sz)
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
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), list->head->value);

   for (size_t i = 0; i < sz / 2; i++)
   {
      LIST__FUNC(pop)(list);
      TEST_ASSERT_EQUAL_size_t(sz - 1 - i, list->size);
      if (sz - 1 - i)
         TEST_ASSERT_NOT_NULL(list->head);
      else
         TEST_ASSERT_NULL(list->head);
   }

   for (size_t i = 0; i < sz / 2; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(sz + i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz + sz / 2 - 1), list->head->value);

   for (size_t i = 0; i < sz / 2; i++)
   {
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz + sz / 2 - 1 - i), list->head->value);
      LIST__FUNC(pop)(list);
   }
   for (size_t i = sz - sz / 2; i; i--)
   {
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i - 1), list->head->value);
      TEST_ASSERT_EQUAL_size_t(i, list->size);
      LIST__FUNC(pop)(list);
   }
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   LIST__FUNC(deinit)(list);
}

static void test_pop_rebuild(size_t sz)
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

   for (size_t i = 0; i < sz; i++)
      LIST__FUNC(pop)(list);
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), list->head->value);

   for (size_t i = 0; i < sz; i++)
      LIST__FUNC(pop)(list);
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   LIST__FUNC(deinit)(list);
}

static void test_pop_after_insert(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t i;
   LIST_TYPE *arr = malloc(sz * sizeof(LIST_TYPE));
   TEST_ASSERT_NOT_NULL(arr);
   for (i = 0; i < sz; i++)
      arr[i] = LIST_VAL_FROM_IDX(i);

   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), arr, sz));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   for (i = 0; i < sz; i++)
   {
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), list->head->value);
      TEST_ASSERT_EQUAL_size_t(sz - i, list->size);
      LIST__FUNC(pop)(list);
   }
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   free(arr);
   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
