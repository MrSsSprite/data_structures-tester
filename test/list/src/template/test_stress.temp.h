#ifndef TEST_STRESS_TEMP_H
#define TEST_STRESS_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"
#include <stdlib.h>

/*--------------------------- Private Declarations ---------------------------*/
static void test_stress_push_pop_cycle(size_t sz);
static void test_stress_random_ops(size_t sz);
static void test_stress_drain_refill(size_t sz);
static void test_stress_churn(size_t sz);
static void test_stress_find_large(size_t sz);
static void test_stress_degenerate(size_t sz);
static void test_stress_alloc_dealloc(size_t sz);
static int stress_cmp(const void *a, const void *b);
static int stress_never_match(const void *a, const void *b);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_stress)(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
   {
      test_stress_push_pop_cycle(list_sz);
      test_stress_random_ops(list_sz);
      test_stress_drain_refill(list_sz);
      test_stress_churn(list_sz);
      test_stress_find_large(list_sz);
      test_stress_degenerate(list_sz);
      test_stress_alloc_dealloc(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*------------------------------- Comparators --------------------------------*/
static int stress_cmp(const void *a, const void *b)
{
   LIST_TYPE va = *(const LIST_TYPE *)a, vb = *(const LIST_TYPE *)b;
   return (va > vb) - (va < vb);
}

static int stress_never_match(const void *a, const void *b)
{
   (void)a; (void)b;
   return 1;
}
/*----------------------------- Comparators END ------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_stress_push_pop_cycle(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (int cycle = 0; cycle < 3; cycle++)
   {
      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), list->head->value);

      for (size_t i = sz; i; i--)
      {
         TEST_ASSERT_NOT_NULL(list->head);
         LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(i - 1), list->head->value);
         TEST_ASSERT_EQUAL_size_t(i, list->size);
         LIST__FUNC(pop)(list);
      }
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);
   }

   LIST__FUNC(deinit)(list);
}

static void test_stress_random_ops(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t n_ops = sz < 100 ? sz : 100;
   LIST_TYPE *tracked = malloc(sizeof(LIST_TYPE) * (sz + n_ops + 1));
   TEST_ASSERT_NOT_NULL(tracked);
   size_t tracked_cnt = 0;

   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
   {
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      tracked[tracked_cnt++] = LIST_VAL_FROM_IDX(i);
   }
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   srand(0xDEAD);

   for (size_t i = 0; i < n_ops; i++)
   {
      int op = rand() % 6;
      LIST_TYPE val;
      LIST__NODE_TYPE *result;

      switch (op)
      {
         case 0: /* push */
            val = LIST_RAND();
            TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, val));
            tracked[tracked_cnt++] = val;
            break;

         case 1: /* pop */
            if (list->size)
            {
               LIST__FUNC(pop)(list);
               tracked_cnt--;
            }
            break;

         case 2: /* insert at head */
            val = LIST_RAND();
            TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list),
                                                         &val, 1));
            tracked[tracked_cnt++] = val;
            break;

         case 3: /* erase from head */
            if (list->size)
            {
               LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 1);
               tracked_cnt--;
            }
            break;

         case 4: /* find existing */
            if (tracked_cnt == 0) break;
            val = tracked[rand() % tracked_cnt];
            result = LIST__FUNC(find)(LIST__FUNC(head)(list), val, NULL);
            TEST_ASSERT_NOT_NULL(result);
            LIST_ASSERT_EQ(val, (*result)->value);
            break;

         case 5: /* find missing */
            result = LIST__FUNC(find)(LIST__FUNC(head)(list),
                                      (LIST_TYPE)(-(int)(i + 2)), NULL);
            TEST_ASSERT_NULL(result);
            break;
      }

      /* size invariant: walk list and count nodes */
      LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
      size_t count = 0;
      while (*iter)
      {
         count++;
         iter = &(*iter)->next;
      }
      TEST_ASSERT_EQUAL_size_t(count, list->size);
   }

   free(tracked);
   LIST__FUNC(deinit)(list);
}

static void test_stress_drain_refill(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (int cycle = 0; cycle < 3; cycle++)
   {
      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), list->head->value);

      for (size_t i = 0; i < sz; i++)
         LIST__FUNC(pop)(list);
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);
   }

   LIST__FUNC(deinit)(list);
}

static void test_stress_churn(size_t sz)
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

   size_t n_iters = sz < 100 ? sz : 100;
   srand(0xDEAD);

   for (size_t i = 0; i < n_iters; i++)
   {
      size_t skip = 1 + rand() % (sz - 2);
      LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
      for (size_t j = 0; j < skip; j++)
         pos = &(*pos)->next;

      LIST_TYPE ival = (LIST_TYPE)(-(int)(i + 1));
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
      TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

      LIST__FUNC(erase)(list, pos, 1);
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
   }

   /* verify list content is intact: [sz-1, sz-2, ..., 0] */
   LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
   {
      TEST_ASSERT_NOT_NULL(*iter);
      LIST_TYPE expected = LIST_VAL_FROM_IDX(--i);
      LIST_ASSERT_EQ(expected, (*iter)->value);
   }

   LIST__FUNC(deinit)(list);
}

static void test_stress_find_large(size_t sz)
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

   /* existing values at various depths */
   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(sz - 1), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(sz - 1 - sz / 4), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1 - sz / 4), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(sz - 1 - sz / 2), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1 - sz / 2), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(sz - 1 - 3 * sz / 4), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(sz - 1 - 3 * sz / 4), (*result)->value);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);

   /* missing values */
   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_SENTINEL_NEG, NULL);
   TEST_ASSERT_NULL(result);

   result = LIST__FUNC(find)(LIST__FUNC(head)(list), (LIST_TYPE)sz, NULL);
   TEST_ASSERT_NULL(result);

   /* custom comparator */
   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), stress_cmp);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);

   /* never-match comparator */
   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), stress_never_match);
   TEST_ASSERT_NULL(result);

   /* find from non-head start position */
   if (sz >= 2)
   {
      LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
      for (size_t i = 0; i < sz / 2; i++)
         pos = &(*pos)->next;

      result = LIST__FUNC(find)(pos, LIST_VAL_FROM_IDX(sz - 1), NULL);
      TEST_ASSERT_NULL(result);

      result = LIST__FUNC(find)(pos, LIST_VAL_FROM_IDX(0), NULL);
      TEST_ASSERT_NOT_NULL(result);
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);
   }

   /* duplicates */
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(0)));
   result = LIST__FUNC(find)(LIST__FUNC(head)(list), LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);
   TEST_ASSERT_EQUAL_PTR(list->head, *result);

   result = LIST__FUNC(find)(&(*result)->next, LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NOT_NULL(result);
   LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(0), (*result)->value);
   TEST_ASSERT_TRUE(list->head != *result);

   result = LIST__FUNC(find)(&(*result)->next, LIST_VAL_FROM_IDX(0), NULL);
   TEST_ASSERT_NULL(result);

   LIST__FUNC(deinit)(list);
}

static void test_stress_degenerate(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);

   /* (a) head-only push/pop — 3 cycles starting from empty */
   {
      LIST__TYPE list = LIST__FUNC(init)();
      TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);

      for (int cycle = 0; cycle < 3; cycle++)
      {
         for (size_t i = 0; i < sz; i++)
            TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
         TEST_ASSERT_EQUAL_size_t(sz, list->size);
         TEST_ASSERT_NOT_NULL(list->head);

         for (size_t i = 0; i < sz; i++)
            LIST__FUNC(pop)(list);
         TEST_ASSERT_NULL(list->head);
         TEST_ASSERT_EQUAL_size_t(0, list->size);
      }

      LIST__FUNC(deinit)(list);
   }

   /* (b) tail-only insert/erase — 10 iterations */
   {
      LIST__TYPE list = LIST__FUNC(init)();
      TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);

      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);

      for (int i = 0; i < 10; i++)
      {
         LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
         while (*pos)
            pos = &(*pos)->next;

         LIST_TYPE ival = (LIST_TYPE)(-(int)(i + 1));
         TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
         TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

         pos = LIST__FUNC(head)(list);
         while ((*pos)->next)
            pos = &(*pos)->next;
         LIST__FUNC(erase)(list, pos, 1);
         TEST_ASSERT_EQUAL_size_t(sz, list->size);
      }

      /* list should still be [sz-1, sz-2, ..., 0] */
      LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
      for (size_t i = sz; i; iter = &(*iter)->next)
      {
         TEST_ASSERT_NOT_NULL(*iter);
         LIST_TYPE expected = LIST_VAL_FROM_IDX(--i);
         LIST_ASSERT_EQ(expected, (*iter)->value);
      }

      LIST__FUNC(deinit)(list);
   }

   /* (c) position-1 insert/erase — 10 iterations */
   if (sz >= 2)
   {
      LIST__TYPE list = LIST__FUNC(init)();
      TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);

      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);

      for (int i = 0; i < 10; i++)
      {
         LIST__NODE_TYPE *pos = LIST__FUNC(head)(list);
         pos = &(*pos)->next;

         LIST_TYPE ival = (LIST_TYPE)(-(int)(i + 1));
         TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, &ival, 1));
         TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

         LIST__FUNC(erase)(list, pos, 1);
         TEST_ASSERT_EQUAL_size_t(sz, list->size);
      }

      /* list should still be [sz-1, sz-2, ..., 0] */
      LIST__NODE_TYPE *iter = LIST__FUNC(head)(list);
      for (size_t i = sz; i; iter = &(*iter)->next)
      {
         TEST_ASSERT_NOT_NULL(*iter);
         LIST_TYPE expected = LIST_VAL_FROM_IDX(--i);
         LIST_ASSERT_EQ(expected, (*iter)->value);
      }

      LIST__FUNC(deinit)(list);
   }
}

static void test_stress_alloc_dealloc(size_t sz)
{
   for (int cycle = 0; cycle < 5; cycle++)
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
      TEST_ASSERT_NOT_NULL(list->head);

      LIST__FUNC(deinit)(list);
   }
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
