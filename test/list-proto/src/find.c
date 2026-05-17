/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
#include <stdlib.h>
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_find_hit(size_t sz);
void test_find_miss(size_t sz);
void test_find_empty(size_t sz);
void test_find_null_cmp(size_t sz);
void test_find_custom_cmp(size_t sz);
void test_find_dupes(size_t sz);
void test_find_start_at(size_t sz);
static int int_cmp(const void *a, const void *b);
static int never_match(const void *a, const void *b);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Unit ---------------------------------*/
void test_find(void)
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
void test_find_hit(size_t sz)
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

   result = list__i_find(list__i_head(list), sz - 1, NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result, "`list__i_find' missed head value");
   TEST_ASSERT_EQUAL_INT(sz - 1, (*result)->value);

   result = list__i_find(list__i_head(list), sz / 2, NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result, "`list__i_find' missed middle value");
   TEST_ASSERT_EQUAL_INT(sz / 2, (*result)->value);

   result = list__i_find(list__i_head(list), 0, NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result, "`list__i_find' missed tail value");
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);

   list__i_deinit(list);
}


void test_find_miss(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *result;

   result = list__i_find(list__i_head(list), -1, NULL);
   TEST_ASSERT_NULL_MESSAGE(result, "`list__i_find' returned non-NULL for foreign value");

   result = list__i_find(list__i_head(list), (int)sz, NULL);
   TEST_ASSERT_NULL_MESSAGE(result, "`list__i_find' returned non-NULL for foreign value");

   list__i_deinit(list);
}


void test_find_empty(size_t sz)
{
   if (sz != 1) return;

   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   List_Node__i *result = list__i_find(list__i_head(list), 0, NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`list__i_find' returned non-NULL on empty list");

   list__i_deinit(list);
}


void test_find_null_cmp(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *result;

   result = list__i_find(list__i_head(list), 0, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);

   result = list__i_find(list__i_head(list), -1, NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`list__i_find' with NULL cmp returned non-NULL for non-existing value");

   list__i_deinit(list);
}


void test_find_custom_cmp(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_UINT(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   List_Node__i *result;

   result = list__i_find(list__i_head(list), 0, int_cmp);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);

   result = list__i_find(list__i_head(list), 0, never_match);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`list__i_find' with never_match cmp returned non-NULL");

   list__i_deinit(list);
}


void test_find_dupes(size_t sz)
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
   TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 0));
   TEST_ASSERT_EQUAL_size_t(sz + 1, list->size);

   List_Node__i *result;

   result = list__i_find(list__i_head(list), 0, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);
   TEST_ASSERT_EQUAL_PTR(list->head, *result);

   result = list__i_find(&(*result)->next, 0, NULL);
   TEST_ASSERT_NOT_NULL(result);
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);
   TEST_ASSERT_TRUE(list->head != *result);

   result = list__i_find(&(*result)->next, 0, NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`list__i_find' returned non-NULL after last duplicate");

   list__i_deinit(list);
}


void test_find_start_at(size_t sz)
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

   List_Node__i *result;

   result = list__i_find(pos, sz - 1, NULL);
   TEST_ASSERT_NULL_MESSAGE(result,
                            "`list__i_find' found value before start position");

   result = list__i_find(pos, 0, NULL);
   TEST_ASSERT_NOT_NULL_MESSAGE(result,
                                "`list__i_find' missed value after start position");
   TEST_ASSERT_EQUAL_INT(0, (*result)->value);

   list__i_deinit(list);
}
/*------------------------------ Test Cases END ------------------------------*/
