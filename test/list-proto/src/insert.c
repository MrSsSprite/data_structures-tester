/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_insert_head_seq(size_t sz);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Units --------------------------------*/
void test_insert_head(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
      test_insert_head_seq(list_sz);
}
/*------------------------------ Test Units END ------------------------------*/


/*-------------------------------- Test Cases --------------------------------*/
void test_insert_head_seq(size_t sz)
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

   List_Node__i *iter = list__i_head(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = sz; i; iter = &(*iter)->next)
      TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);


   list__i_deinit(list);
}
/*------------------------------ Test Cases END ------------------------------*/
