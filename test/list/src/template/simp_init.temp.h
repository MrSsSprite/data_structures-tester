#ifndef SIMP_INIT_TEMP_H
#define SIMP_INIT_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_simp_init)(void)
{
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");

   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Unit END -------------------------------*/

#endif
