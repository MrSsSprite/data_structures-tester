#ifndef TEST_INSERT_TEMP_H
#define TEST_INSERT_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"
#include <stdlib.h>
#include <time.h>

/*--------------------------- Private Declarations ---------------------------*/
static void test_insert_mid_once(size_t sz);
static void test_insert_head_seq(size_t sz);
static void test_insert_head_random(size_t sz);
static void test_insert_head_foreign_val(size_t sz);
static void test_insert_mid_pos1(size_t sz);
static void test_insert_mid_postail(size_t sz);
static void test_insert_mid_2node(size_t sz);
static void test_insert_mid_multi(size_t sz);
static void test_insert_mid_multi_pos1(size_t sz);
static void test_insert_mid_multi_postail(size_t sz);
static void test_insert_mid_n0(size_t sz);
static void test_insert_empty(size_t sz);
static void test_insert_tail(size_t sz);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_insert)(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
   {
      test_insert_mid_once(list_sz);
      test_insert_mid_pos1(list_sz);
      test_insert_mid_postail(list_sz);
      test_insert_mid_2node(list_sz);
      test_insert_mid_multi(list_sz);
      test_insert_mid_multi_pos1(list_sz);
      test_insert_mid_multi_postail(list_sz);
      test_insert_mid_n0(list_sz);
      test_insert_empty(list_sz);
      test_insert_tail(list_sz);
      test_insert_head_seq(list_sz);
      test_insert_head_random(list_sz);
      test_insert_head_foreign_val(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_insert_mid_once(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t skip_idx = sz / 2;
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
   {
      if (i == skip_idx) continue;
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   }
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);
   if (sz - 1)
      TEST_ASSERT_NOT_NULL(list->head);
   else
      TEST_ASSERT_NULL(list->head);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list), *ins_pt;
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = sz; i;)
   {
      if (--i == skip_idx) { ins_pt = iter; continue; }
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
      iter = &(*iter)->next;
   }

   LIST_TYPE i_val = LIST_VAL_FROM_IDX(skip_idx);
   LIST__FUNC(insert)(list, ins_pt, &i_val, 1);

   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_NOT_NULL(iter);
   iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(--i), (*iter)->value);

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_pos1(size_t sz)
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
   pos = &(*pos)->next;

   LIST_TYPE ival = LIST_SENTINEL_NEG;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_SENTINEL_NEG, (*iter)->value);
   iter = &(*iter)->next;
   for (size_t i = sz - 2; ; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
      if (i == 0) break;
      i--;
   }

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_postail(size_t sz)
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
   for (size_t i = 0; i < sz - 1; i++)
      pos = &(*pos)->next;

   LIST_TYPE ival = LIST_SENTINEL_NEG;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz - 1; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_NOT_NULL((*iter)->next);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i--), (*iter)->value);
   }
   LIST_ASSERT_EQ(LIST_SENTINEL_NEG, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*iter)->value);

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_2node(size_t sz)
{
   if (sz != 2) return;
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(0)));
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(1)));
   TEST_ASSERT_EQUAL_size_t(2, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
   pos = &(*pos)->next;

   LIST_TYPE ival = LIST_SENTINEL_NEG;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(3, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(1), (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_SENTINEL_NEG, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NULL(*iter);

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_multi(size_t sz)
{
   if (sz < 2) return;
   LIST_TYPE vals[] = LIST_SENTINEL_MULTI;
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

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = 0; i < sz / 2; i++, iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1 - i), (*iter)->value);
   }
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(vals[0], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[1], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[2], (*iter)->value);
   iter = &(*iter)->next;
   for (size_t i = sz - sz / 2 - 1; ; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
      if (i == 0) break;
      i--;
   }

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_multi_pos1(size_t sz)
{
   if (sz < 2) return;
   LIST_TYPE vals[] = LIST_SENTINEL_MULTI;
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
   pos = &(*pos)->next;

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[0], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[1], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[2], (*iter)->value);
   iter = &(*iter)->next;
   for (size_t i = sz - 2; ; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
      if (i == 0) break;
      i--;
   }

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_multi_postail(size_t sz)
{
   if (sz < 2) return;
   LIST_TYPE vals[] = LIST_SENTINEL_MULTI;
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

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz - 1; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_NOT_NULL((*iter)->next);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i--), (*iter)->value);
   }
   LIST_ASSERT_EQ(vals[0], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[1], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[2], (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*iter)->value);

   LIST__FUNC(deinit)(list);
}

static void test_insert_mid_n0(size_t sz)
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

   LIST_TYPE dummy = LIST_VAL_FROM_IDX(0);
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(--i), (*iter)->value);
   }

   LIST__FUNC(deinit)(list);
}

static void test_insert_empty(size_t sz)
{
   if (sz != 1) return;

   /* n = 1 */
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   LIST_TYPE ival = LIST_VAL_FROM_IDX(42);
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), &ival, 1));
   TEST_ASSERT_EQUAL_size_t(1, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(42), list->head->value);
   TEST_ASSERT_NULL(list->head->next);
   LIST__FUNC(deinit)(list);

   /* n > 1 */
   list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   LIST_TYPE vals[] = LIST_SENTINEL_INSERT;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), vals, 3));
   TEST_ASSERT_EQUAL_size_t(3, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   LIST_ASSERT_EQ(vals[0], list->head->value);
   TEST_ASSERT_NOT_NULL(list->head->next);
   LIST_ASSERT_EQ(vals[1], list->head->next->value);
   TEST_ASSERT_NOT_NULL(list->head->next->next);
   LIST_ASSERT_EQ(vals[2], list->head->next->next->value);
   TEST_ASSERT_NULL(list->head->next->next->next);
   LIST__FUNC(deinit)(list);

   /* n = 0 */
   list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   LIST_TYPE dummy = LIST_VAL_FROM_IDX(0);
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);
   LIST__FUNC(deinit)(list);
}

static void test_insert_tail(size_t sz)
{
   /* n = 1 */
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

   LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
   for (size_t i = 0; i < sz; i++)
   {
      TEST_ASSERT_NOT_NULL(*pos);
      pos = &(*pos)->next;
   }

   LIST_TYPE ival = LIST_SENTINEL_NEG;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(--i), (*iter)->value);
   }
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_SENTINEL_NEG, (*iter)->value);
   TEST_ASSERT_NULL((*iter)->next);
   LIST__FUNC(deinit)(list);

   /* n > 1 */
   list = LIST__FUNC(init)();
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

   pos = LIST__FUNC(head)(list);
   for (size_t i = 0; i < sz; i++)
   {
      TEST_ASSERT_NOT_NULL(*pos);
      pos = &(*pos)->next;
   }

   LIST_TYPE vals[] = LIST_SENTINEL_MULTI;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(--i), (*iter)->value);
   }
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(vals[0], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[1], (*iter)->value);
   iter = &(*iter)->next;
   LIST_ASSERT_EQ(vals[2], (*iter)->value);
   TEST_ASSERT_NULL((*iter)->next);
   LIST__FUNC(deinit)(list);

   /* n = 0 */
   list = LIST__FUNC(init)();
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

   pos = LIST__FUNC(head)(list);
   for (size_t i = 0; i < sz; i++)
      pos = &(*pos)->next;

   LIST_TYPE dummy = LIST_VAL_FROM_IDX(0);
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(--i), (*iter)->value);
   }
   LIST__FUNC(deinit)(list);
}

static void test_insert_head_seq(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   LIST_TYPE *arr = malloc(sz * sizeof(LIST_TYPE));
   TEST_ASSERT_NOT_NULL(arr);
   for (size_t i = 0; i < sz; i++)
      arr[i] = LIST_VAL_FROM_IDX(i);

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), arr, sz));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = 0; i < sz; i++, iter = &(*iter)->next)
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);

   LIST__FUNC(deinit)(list);
   free(arr);
}

static void test_insert_head_random(size_t sz)
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

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list), arr, sz));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (i = 0; i < sz; iter = &(*iter)->next)
      LIST_ASSERT_EQ(arr[i++], (*iter)->value);
   TEST_ASSERT_NULL(*iter);

   free(arr);
   LIST__FUNC(deinit)(list);
}

static void test_insert_head_foreign_val(size_t sz)
{
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = sz; i;)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(--i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = 0; i < sz; iter = &(*iter)->next, i++)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
   }

   LIST_TYPE ival = LIST_SENTINEL_A;
   LIST__FUNC(insert)(list, LIST__FUNC(head)(list), &ival, 1);
   ival = LIST_SENTINEL_B;
   LIST__FUNC(insert)(list, LIST__FUNC(head)(list), &ival, 1);

   iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_SENTINEL_B, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   LIST_ASSERT_EQ(LIST_SENTINEL_A, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   for (size_t i = 0; i < sz; iter = &(*iter)->next, i++)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i), (*iter)->value);
   }

   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
