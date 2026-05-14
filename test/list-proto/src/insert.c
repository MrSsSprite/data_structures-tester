/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
#include <stdlib.h>
#include <time.h>
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_insert_head_seq(size_t sz);
void test_insert_head_random(size_t sz);
void test_insert_mid_once(size_t sz);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Units --------------------------------*/
void test_insert_head(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
    {
      test_insert_head_seq(list_sz);
      test_insert_head_random(list_sz);
    }
}


void test_insert_mid(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
    {
      test_insert_mid_once(list_sz);
    }
}
/*------------------------------ Test Units END ------------------------------*/


/*-------------------------------- Test Cases --------------------------------*/
void test_insert_head_seq(size_t sz)
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
/*------------------------------ Test Cases END ------------------------------*/
