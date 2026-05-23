#ifndef TEST_ERASE_TEMP_H
#define TEST_ERASE_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"

/*--------------------------- Private Declarations ---------------------------*/
static void test_erase_head(size_t sz);
static void test_erase_tail(size_t sz);
static void test_erase_mid(size_t sz);
static void test_erase_multi(size_t sz);
static void test_erase_all(size_t sz);
static void test_erase_n0(size_t sz);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_erase)(void)
{
   for (size_t list_sz = 1; list_sz <= LIST_MAX_SZ; list_sz *= 2)
   {
      test_erase_head(list_sz);
      test_erase_tail(list_sz);
      test_erase_mid(list_sz);
      test_erase_multi(list_sz);
      test_erase_all(list_sz);
      test_erase_n0(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_erase_head(size_t sz)
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

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 1);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);
   if (sz == 1)
      TEST_ASSERT_NULL(list->head);
   else
   {
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 2), list->head->value);

      LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
      for (size_t i = sz - 1; i; iter = &(*iter)->next)
      {
         TEST_ASSERT_NOT_NULL(*iter);
         LIST_TYPE expected = LIST_VAL_FROM_IDX(--i);
         LIST_ASSERT_EQ(expected, (*iter)->value);
      }
   }

   LIST__FUNC(deinit)(list);
}

static void test_erase_tail(size_t sz)
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

   LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
   for (size_t i = 0; i < sz - 1; i++)
      pos = &(*pos)->next;

   LIST__FUNC(erase)(list, pos, 1);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);

   if (sz == 1)
   {
      TEST_ASSERT_NULL(list->head);
   }
   else
   {
      LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
      for (size_t i = sz - 1; i; i--, iter = &(*iter)->next)
      {
         TEST_ASSERT_NOT_NULL(*iter);
         LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
      }
      TEST_ASSERT_NULL(*iter);
   }

   LIST__FUNC(deinit)(list);
}

static void test_erase_mid(size_t sz)
{
   if (sz < 3) return;
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

   LIST_TYPE erased_val = (*pos)->value;
   LIST__FUNC(erase)(list, pos, 1);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i;)
   {
      size_t val = --i;
      if (LIST_VALUE_IDENTITY(LIST_VAL_FROM_IDX(val), erased_val)) continue;
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(val), (*iter)->value);
      iter = &(*iter)->next;
   }

   LIST__FUNC(deinit)(list);
}

static void test_erase_multi(size_t sz)
{
   if (sz < 5) return;
   size_t n = 3;

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

   LIST_TYPE e1 = (*pos)->value;
   LIST_TYPE e2 = (*pos)->next->value;
   LIST_TYPE e3 = (*pos)->next->next->value;

   LIST__FUNC(erase)(list, pos, n);
   TEST_ASSERT_EQUAL_size_t(sz - n, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i;)
   {
      size_t val = --i;
      LIST_TYPE idx_val = LIST_VAL_FROM_IDX(val);
      if (LIST_VALUE_IDENTITY(idx_val, e1) ||
          LIST_VALUE_IDENTITY(idx_val, e2) ||
          LIST_VALUE_IDENTITY(idx_val, e3)) continue;
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(idx_val, (*iter)->value);
      iter = &(*iter)->next;
   }

   LIST__FUNC(deinit)(list);
}

static void test_erase_all(size_t sz)
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

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), sz);

   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not NULL after erasing all elements");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not 0 after erasing all elements");

   LIST__FUNC(deinit)(list);
}

static void test_erase_n0(size_t sz)
{
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   if (sz)
      TEST_ASSERT_NOT_NULL(list->head);
   else
      TEST_ASSERT_NULL(list->head);

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 0);
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_TYPE expected = LIST_VAL_FROM_IDX(--i);
      LIST_ASSERT_EQ(expected, (*iter)->value);
   }

   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
