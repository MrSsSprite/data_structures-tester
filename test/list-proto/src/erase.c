/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_erase_head(size_t sz);
void test_erase_tail(size_t sz);
void test_erase_mid(size_t sz);
void test_erase_multi(size_t sz);
void test_erase_all(size_t sz);
void test_erase_n0(size_t sz);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Unit ---------------------------------*/
void test_erase(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
    {
      test_erase_head(list_sz);
      test_erase_tail(list_sz);
      test_erase_mid(list_sz);
      test_erase_multi(list_sz);
      test_erase_all(list_sz);
      test_erase_n0(list_sz);
    }
}
/*------------------------------ Test Unit END -------------------------------*/


/*-------------------------------- Test Cases --------------------------------*/
void test_erase_head(size_t sz)
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

   list__i_erase(list, list__i_head(list), 1);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);
   if (sz == 1)
      TEST_ASSERT_NULL(list->head);
   else
    {
      TEST_ASSERT_NOT_NULL(list->head);
      TEST_ASSERT_EQUAL_INT(sz - 2, list->head->value);

      List_Node__i *iter = list__i_head(list);
      for (size_t i = sz - 1; i; iter = &(*iter)->next)
       {
         TEST_ASSERT_NOT_NULL(*iter);
         TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);
       }
    }

   list__i_deinit(list);
}


void test_erase_tail(size_t sz)
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

   List_Node__i *pos = list__i_head(list);
   for (size_t i = 0; i < sz - 1; i++)
      pos = &(*pos)->next;

   list__i_erase(list, pos, 1);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);

   if (sz == 1)
    {
      TEST_ASSERT_NULL(list->head);
    }
   else
    {
      List_Node__i *iter = list__i_head(list);
      for (size_t i = sz - 1; i; i--, iter = &(*iter)->next)
       {
         TEST_ASSERT_NOT_NULL(*iter);
         TEST_ASSERT_EQUAL_INT(i, (*iter)->value);
       }
      TEST_ASSERT_NULL(*iter);
    }

   list__i_deinit(list);
}


void test_erase_mid(size_t sz)
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

   List_Node__i *pos = list__i_head(list);
   for (size_t i = 0; i < sz / 2; i++)
      pos = &(*pos)->next;

   size_t erased_val = (*pos)->value;
   list__i_erase(list, pos, 1);
   TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz; i;)
    {
      size_t val = --i;
      if (val == erased_val) continue;
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(val, (*iter)->value);
      iter = &(*iter)->next;
    }

   list__i_deinit(list);
}


void test_erase_multi(size_t sz)
{
   if (sz < 5) return;
   size_t n = 3;

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

   size_t e1 = (*pos)->value;
   size_t e2 = (*pos)->next->value;
   size_t e3 = (*pos)->next->next->value;

   list__i_erase(list, pos, n);
   TEST_ASSERT_EQUAL_size_t(sz - n, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz; i;)
    {
      size_t val = --i;
      if (val == e1 || val == e2 || val == e3) continue;
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(val, (*iter)->value);
      iter = &(*iter)->next;
    }

   list__i_deinit(list);
}


void test_erase_all(size_t sz)
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

   list__i_erase(list, list__i_head(list), sz);

   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not NULL after erasing all elements");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not 0 after erasing all elements");

   list__i_deinit(list);
}


void test_erase_n0(size_t sz)
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

   list__i_erase(list, list__i_head(list), 0);
   TEST_ASSERT_EQUAL_size_t(sz, list->size);

   List_Node__i *iter = list__i_head(list);
   for (size_t i = sz; i; iter = &(*iter)->next)
    {
      TEST_ASSERT_NOT_NULL(*iter);
      TEST_ASSERT_EQUAL_INT(--i, (*iter)->value);
    }

   list__i_deinit(list);
}
/*------------------------------ Test Cases END ------------------------------*/
