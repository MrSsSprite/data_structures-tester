/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_pop_once(void);
void test_pop_seq(size_t sz);
void test_pop_all(size_t sz);
void test_pop_interleaved(size_t sz);
void test_pop_rebuild(size_t sz);
void test_pop_after_insert(size_t sz);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Unit ---------------------------------*/
void test_pop(void)
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
void test_pop_once(void)
{
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 0));
   TEST_ASSERT_EQUAL_size_t(1, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_EQUAL_INT(0, list->head->value);

   list__i_pop(list);
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not NULL after popping last element");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not 0 after popping last element");

   list__i_deinit(list);
}


void test_pop_seq(size_t sz)
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

   for (size_t i = sz; i; i--)
    {
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(i - 1, list->head->value);
      TEST_ASSERT_EQUAL_size_t(i, list->size);
      list__i_pop(list);
    }
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   list__i_deinit(list);
}


void test_pop_all(size_t sz)
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

   for (size_t i = 0; i < sz; i++)
      list__i_pop(list);

   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not NULL after popping all elements");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not 0 after popping all elements");

   list__i_deinit(list);
}
void test_pop_interleaved(size_t sz)
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
   TEST_ASSERT_EQUAL_INT(sz - 1, list->head->value);

   for (size_t i = 0; i < sz / 2; i++)
    {
      list__i_pop(list);
      TEST_ASSERT_EQUAL_size_t(sz - 1 - i, list->size);
      if (sz - 1 - i)
         TEST_ASSERT_NOT_NULL(list->head);
      else
         TEST_ASSERT_NULL(list->head);
    }

   for (size_t i = 0; i < sz / 2; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, sz + i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_EQUAL_INT(sz + sz / 2 - 1, list->head->value);

   for (size_t i = 0; i < sz / 2; i++)
    {
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(sz + sz / 2 - 1 - i, list->head->value);
      list__i_pop(list);
    }
   for (size_t i = sz - sz / 2; i; i--)
    {
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(i - 1, list->head->value);
      TEST_ASSERT_EQUAL_size_t(i, list->size);
      list__i_pop(list);
    }
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   list__i_deinit(list);
}


void test_pop_rebuild(size_t sz)
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

   for (size_t i = 0; i < sz; i++)
      list__i_pop(list);
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);
   TEST_ASSERT_EQUAL_INT(sz - 1, list->head->value);

   for (size_t i = 0; i < sz; i++)
      list__i_pop(list);
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   list__i_deinit(list);
}


void test_pop_after_insert(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   size_t i;
   int *arr = malloc(sz * sizeof(int));
   TEST_ASSERT_NOT_NULL(arr);
   for (i = 0; i < sz; i++)
      arr[i] = i;

   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list), arr, sz));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   for (i = 0; i < sz; i++)
    {
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(i, list->head->value);
      TEST_ASSERT_EQUAL_size_t(sz - i, list->size);
      list__i_pop(list);
    }
   TEST_ASSERT_NULL(list->head);
   TEST_ASSERT_EQUAL_size_t(0, list->size);

   free(arr);
   list__i_deinit(list);
}
/*------------------------------ Test Cases END ------------------------------*/
