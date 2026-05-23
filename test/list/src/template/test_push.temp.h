#ifndef TEST_PUSH_TEMP_H
#define TEST_PUSH_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"
#include <stdlib.h>
#include <time.h>

/*--------------------------- Private Declarations ---------------------------*/
static void test_push_seq(size_t sz);
static void test_push_random(size_t sz);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_push)(void)
{
   for (size_t list_sz = 1; list_sz <= LIST_MAX_SZ; list_sz *= 2)
   {
      test_push_seq(list_sz);
      test_push_random(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_push_seq(size_t sz)
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

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      LIST_TYPE expected = LIST_VAL_FROM_IDX(--i);
      LIST_ASSERT_EQ(expected, (*iter)->value);
   }

   LIST__FUNC(deinit)(list);
}

static void test_push_random(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t i;
   LIST_TYPE *arr = malloc(sizeof(LIST_TYPE) * sz);
   TEST_ASSERT_NOT_NULL(arr);
   srand(time(NULL));
   for (i = 0; i < sz; i++)
      arr[i] = LIST_RAND();

   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   while (i)
   {
      LIST_TYPE val = arr[--i];
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, val));
   }
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (i = 0; i < sz; iter = &(*iter)->next)
   {
      LIST_TYPE val = arr[i++];
      LIST_ASSERT_EQ(val, (*iter)->value);
   }
   TEST_ASSERT_NULL(*iter);

   free(arr);
   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
