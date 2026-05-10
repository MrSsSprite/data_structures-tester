/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
/*--------------------------- Private Includes END ---------------------------*/


/*-------------------------------- Test Unit ---------------------------------*/
void test_simp_init(void)
{
   List__i list = list__i_init();
   TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");

   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   list__i_deinit(list);
}
/*------------------------------ Test Unit END -------------------------------*/
