#ifndef TEST_SIZE_TEMP_H
#define TEST_SIZE_TEMP_H

#include "../ds.h"
#include "unity.h"
#include "../macro_utils.h"

/*--------------------------- Private Declarations ---------------------------*/
static void test_size_noop_empty(void);
static void test_size_per_op(size_t sz);
static void test_size_noop(size_t sz);
static void test_size_consistency(size_t sz);
static void test_size_rebuild(size_t sz);
static void test_size_mixed(size_t sz);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_size)(void)
{
   test_size_noop_empty();

   for (size_t list_sz = 1; list_sz <= LIST_MAX_SZ; list_sz *= 2)
   {
      test_size_per_op(list_sz);
      test_size_noop(list_sz);
      test_size_consistency(list_sz);
      test_size_rebuild(list_sz);
      test_size_mixed(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_size_noop_empty(void)
{
   LIST__TYPE list;
   LIST_TYPE dummy = LIST_VAL_FROM_IDX(0);

   /* --- insert n=0 on empty list --- */
   list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, LIST__FUNC(head)(list),
                                               &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);
   LIST__FUNC(deinit)(list);

   /* --- erase n=0 on empty list --- */
   list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 0);
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);
   LIST__FUNC(deinit)(list);
}

static void test_size_per_op(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
   {
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      TEST_ASSERT_EQUAL_size_t(i + 1, list->size);
   }
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   LIST__FUNC(pop)(list);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);

   LIST_TYPE vals[] = LIST_SENTINEL_SIZE;
   TEST_ASSERT_EQUAL_INT(0,
       LIST__FUNC(insert)(list, LIST__FUNC(head)(list), vals, 3));
   TEST_ASSERT_EQUAL_size_t(sz + 2, list->size);

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 2);
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   LIST__FUNC(deinit)(list);
}

static void test_size_noop(size_t sz)
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

   LIST_TYPE dummy = LIST_VAL_FROM_IDX(0);
   TEST_ASSERT_EQUAL_INT(0,
       LIST__FUNC(insert)(list, LIST__FUNC(head)(list), &dummy, 0));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 0);
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   LIST__FUNC(deinit)(list);
}

static void test_size_consistency(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");

   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   TEST_ASSERT_NOT_NULL(list->head);

   size_t count = 0;
   for (struct LIST__NODE_STRUCT_TAG **pos = LIST__FUNC(head)(list);
        *pos; pos = &(*pos)->next)
      count++;
   TEST_ASSERT_EQUAL_size_t(sz, count);
   TEST_ASSERT_EQUAL_size_t(list->size, count);

   for (size_t i = 0; i < sz; i++)
      LIST__FUNC(pop)(list);
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL_MESSAGE(list->head,
       "`list->head' not NULL after popping all elements");

   LIST__FUNC(deinit)(list);
}

static void test_size_rebuild(size_t sz)
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

   for (size_t i = 0; i < sz; i++)
      LIST__FUNC(pop)(list);
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   for (size_t i = 0; i < sz; i++)
      LIST__FUNC(pop)(list);
   TEST_ASSERT_EQUAL_size_t(0, list->size);
   TEST_ASSERT_NULL(list->head);

   LIST__FUNC(deinit)(list);
}

static void test_size_mixed(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPE list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" LIST__STR_EXPAND(LIST__FUNC(init)) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   size_t expected = 0;

   for (size_t i = 0; i < sz; i++)
   {
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
      expected++;
      TEST_ASSERT_EQUAL_size_t(expected, list->size);
   }

   size_t n_pop = sz / 2;
   for (size_t i = 0; i < n_pop; i++)
   {
      LIST__FUNC(pop)(list);
      expected--;
      TEST_ASSERT_EQUAL_size_t(expected, list->size);
   }

   struct LIST__NODE_STRUCT_TAG **pos = LIST__FUNC(head)(list);
   while (*pos)
      pos = &(*pos)->next;

   LIST_TYPE vals[] = LIST_SENTINEL_MULTI;
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(insert)(list, pos, vals, 3));
   expected += 3;
   TEST_ASSERT_EQUAL_size_t(expected, list->size);

   LIST__FUNC(erase)(list, LIST__FUNC(head)(list), 1);
   expected--;
   TEST_ASSERT_EQUAL_size_t(expected, list->size);

   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(200)));
   expected++;
   TEST_ASSERT_EQUAL_size_t(expected, list->size);
   TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(201)));
   expected++;
   TEST_ASSERT_EQUAL_size_t(expected, list->size);

   LIST__FUNC(pop)(list);
   expected--;
   TEST_ASSERT_EQUAL_size_t(expected, list->size);

   LIST__FUNC(deinit)(list);
}
/*------------------------------ Test Cases END ------------------------------*/

#endif
