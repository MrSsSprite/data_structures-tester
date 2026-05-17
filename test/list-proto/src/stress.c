/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
#include <stdlib.h>
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_stress_push_pop_cycle(size_t sz);
void test_stress_random_ops(size_t sz);
void test_stress_drain_refill(size_t sz);
void test_stress_churn(size_t sz);
void test_stress_find_large(size_t sz);
void test_stress_degenerate(size_t sz);
void test_stress_alloc_dealloc(size_t sz);
static int int_cmp(const void *a, const void *b);
static int never_match(const void *a, const void *b);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Unit ---------------------------------*/
void test_stress(void)
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
static int int_cmp(const void *a, const void *b)
{
   int va = *(const int *)a, vb = *(const int *)b;
   return (va > vb) - (va < vb);
}


static int never_match(const void *a, const void *b)
{
   (void)a; (void)b;
   return 1;
}
/*----------------------------- Comparators END ------------------------------*/


/*-------------------------------- Test Cases --------------------------------*/
void test_stress_push_pop_cycle(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (int cycle = 0; cycle < 3; cycle++)
    {
      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(sz - 1, list->head->value);

      for (size_t i = sz; i; i--)
       {
         TEST_ASSERT_NOT_NULL(list->head);
         TEST_ASSERT_EQUAL_INT(i - 1, list->head->value);
         TEST_ASSERT_EQUAL_size_t(i, list->size);
         list__i_pop(list);
       }
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);
    }

   list__i_deinit(list);
}


void test_stress_random_ops(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t n_ops = sz < 100 ? sz : 100;
   int *tracked = malloc(sizeof(int) * (sz + n_ops + 1));
   TEST_ASSERT_NOT_NULL(tracked);
   size_t tracked_cnt = 0;

   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
    {
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
      tracked[tracked_cnt++] = i;
    }
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   srand(0xDEAD);

   for (size_t i = 0; i < n_ops; i++)
    {
      int op = rand() % 6;
      int val;
      List_Node__i *result;

      switch (op)
       {
         case 0: /* push */
            val = rand();
            TEST_ASSERT_EQUAL_INT(0, list__i_push(list, val));
            tracked[tracked_cnt++] = val;
            break;

         case 1: /* pop */
            if (list->size)
             {
               list__i_pop(list);
               tracked_cnt--;
             }
            break;

         case 2: /* insert at head */
            val = rand();
            TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list),
                                                     &val, 1));
            tracked[tracked_cnt++] = val;
            break;

         case 3: /* erase from head */
            if (list->size)
             {
               list__i_erase(list, list__i_head(list), 1);
               tracked_cnt--;
             }
            break;

         case 4: /* find existing */
            if (tracked_cnt == 0) break;
            val = tracked[rand() % tracked_cnt];
            result = list__i_find(list__i_head(list), val, NULL);
            TEST_ASSERT_NOT_NULL(result);
            TEST_ASSERT_EQUAL_INT(val, (*result)->value);
            break;

         case 5: /* find missing */
            result = list__i_find(list__i_head(list), -(int)(i + 2), NULL);
            TEST_ASSERT_NULL(result);
            break;
       }

      /* size invariant: walk list and count nodes */
      List_Node__i *iter = list__i_head(list);
      size_t count = 0;
      while (*iter)
       {
         count++;
         iter = &(*iter)->next;
       }
      TEST_ASSERT_EQUAL_size_t(count, list->size);
    }

   free(tracked);
   list__i_deinit(list);
}


void test_stress_drain_refill(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (int cycle = 0; cycle < 3; cycle++)
    {
      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(sz - 1, list->head->value);

      for (size_t i = 0; i < sz; i++)
         list__i_pop(list);
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);
    }

   list__i_deinit(list);
}


void test_stress_churn(size_t sz)
{
   if (sz < 3) return;
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   size_t n_iters = sz < 100 ? sz : 100;
   srand(0xDEAD);

   for (size_t i = 0; i < n_iters; i++)
    {
      size_t skip = 1 + rand() % (sz - 2);
      List_Node__i *pos = list__i_head(list);
      for (size_t j = 0; j < skip; j++)
         pos = &(*pos)->next;

      int ival = -(int)(i + 1);
      TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, &ival, 1));
      TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

      list__i_erase(list, pos, 1);
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
    }

   /* verify list content is intact: [sz-1, sz-2, ..., 0] */
   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);
    }

   list__i_deinit(list);
}


void test_stress_find_large(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *result;

   /* existing values at various depths */
   result = list__i_find(list__i_head(list), sz - 1, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(sz - 1, (*result)->value);

   result = list__i_find(list__i_head(list), sz - 1 - sz / 4, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(sz - 1 - sz / 4, (*result)->value);

   result = list__i_find(list__i_head(list), sz - 1 - sz / 2, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(sz - 1 - sz / 2, (*result)->value);

   result = list__i_find(list__i_head(list), sz - 1 - 3 * sz / 4, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(sz - 1 - 3 * sz / 4, (*result)->value);

   result = list__i_find(list__i_head(list), 0, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);

   /* missing values */
   result = list__i_find(list__i_head(list), -1, NULL);
   TEST_ASSERT_NULL(result);

   result = list__i_find(list__i_head(list), (int)sz, NULL);
   TEST_ASSERT_NULL(result);

   /* custom comparator */
   result = list__i_find(list__i_head(list), 0, int_cmp);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);

   /* never-match comparator */
   result = list__i_find(list__i_head(list), 0, never_match);
   TEST_ASSERT_NULL(result);

   /* find from non-head start position */
   if (sz >= 2)
    {
      List_Node__i *pos = list__i_head(list);
      for (size_t i = 0; i < sz / 2; i++)
         pos = &(*pos)->next;

      result = list__i_find(pos, sz - 1, NULL);
      TEST_ASSERT_NULL(result);

      result = list__i_find(pos, 0, NULL);
      TEST_ASSERT_NOT_NULL(result);
      TEST_ASSERT_EQUAL_INT(0, (*result)->value);
    }

   /* duplicates */
   TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 0));
   result = list__i_find(list__i_head(list), 0, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);
   TEST_ASSERT_EQUAL_PTR(list->head, *result);

   result = list__i_find(&(*result)->next, 0, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);
   TEST_ASSERT_TRUE(list->head != *result);

   result = list__i_find(&(*result)->next, 0, NULL);
   TEST_ASSERT_NULL(result);

   list__i_deinit(list);
}


void test_stress_degenerate(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);

   /* (a) head-only push/pop — 3 cycles starting from empty */
   {
      List__i list = list__i_init();
      TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);

      for (int cycle = 0; cycle < 3; cycle++)
       {
         for (size_t i = 0; i < sz; i++)
            TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
         TEST_ASSERT_EQUAL_size_t(sz, list->size);
         TEST_ASSERT_NOT_NULL(list->head);

         for (size_t i = 0; i < sz; i++)
            list__i_pop(list);
         TEST_ASSERT_NULL(list->head);
         TEST_ASSERT_EQUAL_size_t(0, list->size);
       }

      list__i_deinit(list);
   }

   /* (b) tail-only insert/erase — 10 iterations */
   {
      List__i list = list__i_init();
      TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);

      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);

      for (int i = 0; i < 10; i++)
       {
         List_Node__i *pos = list__i_head(list);
         while (*pos)
            pos = &(*pos)->next;

         int ival = -(int)(i + 1);
         TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, &ival, 1));
         TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

         pos = list__i_head(list);
         while ((*pos)->next)
            pos = &(*pos)->next;
         list__i_erase(list, pos, 1);
         TEST_ASSERT_EQUAL_size_t(sz, list->size);
       }

      /* list should still be [sz-1, sz-2, ..., 0] */
      List_Node__i *iter = list__i_head(list);
      for (size_t i = sz; i; iter = &(*iter)->next)
       {
         TEST_ASSERT_NOT_NULL(*iter);
         TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);
       }

      list__i_deinit(list);
   }

   /* (c) position-1 insert/erase — 10 iterations */
   if (sz >= 2)
    {
      List__i list = list__i_init();
      TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
      TEST_ASSERT_NULL(list->head);
      TEST_ASSERT_EQUAL_size_t(0, list->size);

      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);

      for (int i = 0; i < 10; i++)
       {
         List_Node__i *pos = list__i_head(list);
         pos = &(*pos)->next;

         int ival = -(int)(i + 1);
         TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, &ival, 1));
         TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

         list__i_erase(list, pos, 1);
         TEST_ASSERT_EQUAL_size_t(sz, list->size);
       }

      /* list should still be [sz-1, sz-2, ..., 0] */
      List_Node__i *iter = list__i_head(list);
      for (size_t i = sz; i; iter = &(*iter)->next)
       {
         TEST_ASSERT_NOT_NULL(*iter);
         TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);
       }

      list__i_deinit(list);
    }
}


void test_stress_alloc_dealloc(size_t sz)
{
   for (int cycle = 0; cycle < 5; cycle++)
    {
      List__i list = list__i_init();
      TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
      TEST_ASSERT_NULL_MESSAGE(list->head,
                               "`list->head' not initialized to NULL");
      TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                     "`list->size' not initialized to 0");

      for (size_t i = 0; i < sz; i++)
         TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
      TEST_ASSERT_EQUAL_size_t(sz, list->size);
      TEST_ASSERT_NOT_NULL(list->head);

      list__i_deinit(list);
    }
}
/*------------------------------ Test Cases END ------------------------------*/
