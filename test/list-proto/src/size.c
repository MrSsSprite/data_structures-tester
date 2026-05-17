/*----------------------------- Private Includes -----------------------------*/
#include "ds.h"
#include "unity.h"
/*--------------------------- Private Includes END ---------------------------*/


/*--------------------------- Private Declarations ---------------------------*/
void test_size_noop_empty(void);
void test_size_per_op(size_t sz);
void test_size_noop(size_t sz);
void test_size_consistency(size_t sz);
void test_size_rebuild(size_t sz);
void test_size_mixed(size_t sz);
/*------------------------- Private Declarations END -------------------------*/


/*-------------------------------- Test Unit ---------------------------------*/
void test_size(void)
{
	test_size_noop_empty();

	for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
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
void test_size_noop_empty(void)
{
	List__i list;
	int dummy = 0;

	/* --- insert n=0 on empty list --- */
	list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
	TEST_ASSERT_NULL_MESSAGE(list->head,
	                         "`list->head' not initialized to NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
	                               "`list->size' not initialized to 0");

	TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, list__i_head(list),
	                                        &dummy, 0));
	TEST_ASSERT_EQUAL_size_t(0, list->size);
	TEST_ASSERT_NULL(list->head);
	list__i_deinit(list);

	/* --- erase n=0 on empty list --- */
	list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
	TEST_ASSERT_NULL_MESSAGE(list->head,
	                         "`list->head' not initialized to NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
	                               "`list->size' not initialized to 0");

	list__i_erase(list, list__i_head(list), 0);
	TEST_ASSERT_EQUAL_size_t(0, list->size);
	TEST_ASSERT_NULL(list->head);
	list__i_deinit(list);
}


void test_size_per_op(size_t sz)
{
	TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
	List__i list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
	TEST_ASSERT_NULL_MESSAGE(list->head,
	                         "`list->head' not initialized to NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
	                               "`list->size' not initialized to 0");

	/* push: each call increments size by 1 */
	for (size_t i = 0; i < sz; i++)
	{
		TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
		TEST_ASSERT_EQUAL_size_t(i + 1, list->size);
	}
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	/* pop: decrements size by 1 */
	list__i_pop(list);
	TEST_ASSERT_EQUAL_size_t(sz - 1, list->size);

	/* insert n=3 at head: size += 3 */
	int vals[] = {100, 200, 300};
	TEST_ASSERT_EQUAL_INT(0,
	    list__i_insert(list, list__i_head(list), vals, 3));
	TEST_ASSERT_EQUAL_size_t(sz + 2, list->size);

	/* erase n=2 from head: size -= 2 */
	list__i_erase(list, list__i_head(list), 2);
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	list__i_deinit(list);
}


void test_size_noop(size_t sz)
{
	TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
	List__i list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
	TEST_ASSERT_NULL_MESSAGE(list->head,
	                         "`list->head' not initialized to NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
	                               "`list->size' not initialized to 0");

	/* build list of size sz */
	for (size_t i = 0; i < sz; i++)
		TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	/* insert n=0: size unchanged, returns 0 */
	int dummy = 0;
	TEST_ASSERT_EQUAL_INT(0,
	    list__i_insert(list, list__i_head(list), &dummy, 0));
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	/* erase n=0: size unchanged */
	list__i_erase(list, list__i_head(list), 0);
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	list__i_deinit(list);
}


void test_size_consistency(size_t sz)
{
	TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
	List__i list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");

	/* invariant: size==0 implies head==NULL */
	TEST_ASSERT_EQUAL_size_t(0, list->size);
	TEST_ASSERT_NULL(list->head);

	/* build list */
	for (size_t i = 0; i < sz; i++)
		TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	/* invariant: size>0 implies head!=NULL */
	TEST_ASSERT_NOT_NULL(list->head);

	/* manual traversal count matches size field */
	size_t count = 0;
	for (struct list_node__i **pos = list__i_head(list);
	     *pos; pos = &(*pos)->next)
		count++;
	TEST_ASSERT_EQUAL_size_t(sz, count);
	TEST_ASSERT_EQUAL_size_t(list->size, count);

	/* invariant again: pop all, size==0 => head==NULL */
	for (size_t i = 0; i < sz; i++)
		list__i_pop(list);
	TEST_ASSERT_EQUAL_size_t(0, list->size);
	TEST_ASSERT_NULL_MESSAGE(list->head,
	    "`list->head' not NULL after popping all elements");

	list__i_deinit(list);
}


void test_size_rebuild(size_t sz)
{
	TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
	List__i list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
	TEST_ASSERT_NULL_MESSAGE(list->head,
	                         "`list->head' not initialized to NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
	                               "`list->size' not initialized to 0");

	/* first build */
	for (size_t i = 0; i < sz; i++)
		TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
	TEST_ASSERT_EQUAL_size_t(sz, list->size);

	/* drain to empty */
	for (size_t i = 0; i < sz; i++)
		list__i_pop(list);
	TEST_ASSERT_EQUAL_size_t(0, list->size);
	TEST_ASSERT_NULL(list->head);

	/* rebuild: verify size recovers */
	for (size_t i = 0; i < sz; i++)
		TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
	TEST_ASSERT_EQUAL_size_t(sz, list->size);
	TEST_ASSERT_NOT_NULL(list->head);

	/* drain again */
	for (size_t i = 0; i < sz; i++)
		list__i_pop(list);
	TEST_ASSERT_EQUAL_size_t(0, list->size);
	TEST_ASSERT_NULL(list->head);

	list__i_deinit(list);
}


void test_size_mixed(size_t sz)
{
	TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
	List__i list = list__i_init();
	TEST_ASSERT_MESSAGE(list, "`list__i_init' failure");
	TEST_ASSERT_NULL_MESSAGE(list->head,
	                         "`list->head' not initialized to NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
	                               "`list->size' not initialized to 0");

	size_t expected = 0;

	/* push sz elements, verify at each step */
	for (size_t i = 0; i < sz; i++)
	{
		TEST_ASSERT_EQUAL_INT(0, list__i_push(list, i));
		expected++;
		TEST_ASSERT_EQUAL_size_t(expected, list->size);
	}

	/* pop floor(sz/2) elements, verify at each step */
	size_t n_pop = sz / 2;
	for (size_t i = 0; i < n_pop; i++)
	{
		list__i_pop(list);
		expected--;
		TEST_ASSERT_EQUAL_size_t(expected, list->size);
	}

	/* find tail position */
	struct list_node__i **pos = list__i_head(list);
	while (*pos)
		pos = &(*pos)->next;

	/* insert 3 at tail */
	int vals[] = {-3, -2, -1};
	TEST_ASSERT_EQUAL_INT(0, list__i_insert(list, pos, vals, 3));
	expected += 3;
	TEST_ASSERT_EQUAL_size_t(expected, list->size);

	/* erase 1 from head */
	list__i_erase(list, list__i_head(list), 1);
	expected--;
	TEST_ASSERT_EQUAL_size_t(expected, list->size);

	/* push 2 more at head */
	TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 200));
	expected++;
	TEST_ASSERT_EQUAL_size_t(expected, list->size);
	TEST_ASSERT_EQUAL_INT(0, list__i_push(list, 201));
	expected++;
	TEST_ASSERT_EQUAL_size_t(expected, list->size);

	/* pop 1 */
	list__i_pop(list);
	expected--;
	TEST_ASSERT_EQUAL_size_t(expected, list->size);

	list__i_deinit(list);
}
/*------------------------------ Test Cases END ------------------------------*/
