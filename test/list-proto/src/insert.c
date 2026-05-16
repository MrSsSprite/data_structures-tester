/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
#include <stdlib.h>
#include <time.h>
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_push_seq(size_t sz);
void test_push_random(size_t sz);
void test_insert_mid_once(size_t sz);
void test_insert_head_seq(size_t sz);
void test_insert_head_random(size_t sz);
void test_insert_head_foreign_val(size_t sz);
void test_insert_mid_pos1(size_t sz);
void test_insert_mid_postail(size_t sz);
void test_insert_mid_2node(size_t sz);
void test_insert_mid_multi(size_t sz);
void test_insert_mid_multi_pos1(size_t sz);
void test_insert_mid_multi_postail(size_t sz);
void test_insert_mid_n0(size_t sz);
void test_insert_empty(size_t sz);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Units --------------------------------*/
void test_push(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
    {
      test_push_seq(list_sz);
      test_push_random(list_sz);
    }
}


void test_insert(void)
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
      test_insert_head_seq(list_sz);
      test_insert_head_random(list_sz);
      test_insert_head_foreign_val(list_sz);
    }
}
/*------------------------------ Test Units END ------------------------------*/


/*-------------------------------- Test Cases --------------------------------*/
void test_push_seq(size_t sz)
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

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = sz; i; iter = &(*iter)->next)
      TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);

   list__i_deinit(list);
}


void test_push_random(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t i;
   int *arr = malloc(sizeof(int) * sz);
   TEST_ASSERT_NOT_NULL(arr);
   srand(time(NULL));
   for (i = 0; i < sz; i++)
      arr[i] = rand();

   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   while (i)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, arr[--i]));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (i = 0; i < sz; iter = &(*iter)->next)
      TEST_ASSERT_EQUAL_INT(arr[i++], (*iter)->value);
   TEST_ASSERT_NULL(*iter);

   free(arr);
   list__i_deinit(list);
}


void test_insert_mid_once(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t skip_idx = sz / 2;
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
    {
      if (i == skip_idx) continue;
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
    }
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);
   if (sz - 1)
      TEST_ASSERT_NOT_NULL(list->head);
   else
      TEST_ASSERT_NULL(list->head);

   List_Node__i *iter = list__i_head(list), *ins_pt;
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = sz; i;)
    {
      if (--i == skip_idx) { ins_pt = iter; continue; }
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
      iter = &(*iter)->next;
    }

   int i_val = skip_idx;
   list__i_insert(list, ins_pt, &i_val, 1);

   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_NOT_NULL(iter);
   iter = list__i_head(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
      TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);

   list__i_deinit(list);
}


void test_insert_mid_pos1(size_t sz)
{
   if (sz < 2) return;
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

   List_Node__i *pos = list__i_head(list);
   pos = &(*pos)->next;

   int ival = -1;
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(sz - 1, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(-1, (*iter)->value);
   iter = &(*iter)->next;
   for (size_t i = sz - 2; ; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
      if (i == 0) break;
      i--;
    }

   list__i_deinit(list);
}


void test_insert_mid_postail(size_t sz)
{
   if (sz < 2) return;
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

   List_Node__i *pos = list__i_head(list);
   for (size_t i = 0; i < sz - 1; i++)
      pos = &(*pos)->next;

   int ival = -1;
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz - 1; i; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_NOT_NULL((*iter)->next);
      TEST_ASSERT_EQUAL_INT(i--, (*iter)->value);
    }
   TEST_ASSERT_EQUAL_INT(-1, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(0, (*iter)->value);

   list__i_deinit(list);
}


void test_insert_mid_2node(size_t sz)
{
   if (sz != 2) return;
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 0));
   TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 1));
   TEST_ASSERT_EQUAL_size_t(2, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *pos = list__i_head(list);
   pos = &(*pos)->next;

   int ival = -1;
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, &ival, 1));
   TEST_ASSERT_EQUAL_size_t(3, list->size);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(1, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(-1, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(0, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NULL(*iter);

   list__i_deinit(list);
}


void test_insert_mid_multi(size_t sz)
{
   if (sz < 2) return;
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

   List_Node__i *pos = list__i_head(list);
   for (size_t i = 0; i < sz / 2; i++)
      pos = &(*pos)->next;

   int vals[] = {-3, -2, -1};
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = 0; i < sz / 2; i++, iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(sz - 1 - i, (*iter)->value);
    }
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(-3, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-2, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-1, (*iter)->value);
   iter = &(*iter)->next;
   for (size_t i = sz - sz / 2 - 1; ; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
      if (i == 0) break;
      i--;
    }

   list__i_deinit(list);
}


void test_insert_mid_multi_pos1(size_t sz)
{
   if (sz < 2) return;
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

   List_Node__i *pos = list__i_head(list);
   pos = &(*pos)->next;

   int vals[] = {-3, -2, -1};
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(sz - 1, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-3, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-2, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-1, (*iter)->value);
   iter = &(*iter)->next;
   for (size_t i = sz - 2; ; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
      if (i == 0) break;
      i--;
    }

   list__i_deinit(list);
}


void test_insert_mid_multi_postail(size_t sz)
{
   if (sz < 2) return;
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

   List_Node__i *pos = list__i_head(list);
   for (size_t i = 0; i < sz - 1; i++)
      pos = &(*pos)->next;

   int vals[] = {-3, -2, -1};
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 3, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz - 1; i; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_NOT_NULL((*iter)->next);
      TEST_ASSERT_EQUAL_INT(i--, (*iter)->value);
    }
   TEST_ASSERT_EQUAL_INT(-3, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-2, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_EQUAL_INT(-1, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL_INT(0, (*iter)->value);

   list__i_deinit(list);
}


void test_insert_mid_n0(size_t sz)
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
   if (sz)
      TEST_ASSERT_NOT_NULL(list->head);
   else
      TEST_ASSERT_NULL(list->head);

   int dummy = 0;
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);
    }

   list__i_deinit(list);
}


void test_insert_empty(size_t sz)
{
   if (sz != 1) return;

   /* n = 1 */
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   int ival = 42;
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), &ival, 1));
   TEST_ASSERT_EQUAL_size_t(1, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_EQUAL_INT(42, list->head->value);
   TEST_ASSERT_NULL(list->head->next);
   list__i_deinit(list);

   /* n > 1 */
   list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   int vals[] = {10, 20, 30};
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), vals, 3));
   TEST_ASSERT_EQUAL_size_t(3, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_EQUAL_INT(10, list->head->value);
   TEST_ASSERT_NOT_NULL(list->head->next);
   TEST_ASSERT_EQUAL_INT(20, list->head->next->value);
   TEST_ASSERT_NOT_NULL(list->head->next->next);
   TEST_ASSERT_EQUAL_INT(30, list->head->next->next->value);
   TEST_ASSERT_NULL(list->head->next->next->next);
   list__i_deinit(list);

   /* n = 0 */
   list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   int dummy = 0;
   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);
   list__i_deinit(list);
}


void test_insert_head_seq(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   int *arr = malloc(sz * sizeof(int));
   TEST_ASSERT_NOT_NULL(arr);
   for (size_t i = 0; i < sz; i++)
      arr[i] = i;

   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), arr, sz));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = 0; i < sz; i++, iter = &(*iter)->next)
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);

   list__i_deinit(list);
   free(arr);
}


void test_insert_head_random(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t i;
   int *arr = malloc(sizeof(int) * sz);
   TEST_ASSERT_NOT_NULL(arr);
   srand(time(NULL));
   for (i = 0; i < sz; i++)
      arr[i] = rand();

   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), arr, sz));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (i = 0; i < sz; iter = &(*iter)->next)
      TEST_ASSERT_EQUAL_INT(arr[i++], (*iter)->value);
   TEST_ASSERT_NULL(*iter);

   free(arr);
   list__i_deinit(list);
}


void test_insert_head_foreign_val(size_t sz)
{
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = sz; i;)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, --i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = 0; i < sz; iter = &(*iter)->next, i++)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
    }

   int ival = 0xAB;
   list__i_insert(list, list__i_head(list), &ival, 1);
   ival = 0xCD;
   list__i_insert(list, list__i_head(list), &ival, 1);

   iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL(0xCD, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   TEST_ASSERT_EQUAL(0xAB, (*iter)->value);
   iter = &(*iter)->next;
   TEST_ASSERT_NOT_NULL(iter);
   TEST_ASSERT_NOT_NULL(*iter);
   for (size_t i = 0; i < sz; iter = &(*iter)->next, i++)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
    }

   list__i_deinit(list);
}
/*------------------------------ Test Cases END ------------------------------*/
