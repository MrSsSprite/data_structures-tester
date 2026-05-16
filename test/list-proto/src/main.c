#include "unity.h"
#include "unity_internals.h"
#include "test_units.h"


void setUp(void) { }
void tearDown(void) { }


int main(void)
{
   UNITY_BEGIN();

   RUN_TEST(test_simp_init);
   RUN_TEST(test_push);
   RUN_TEST(test_insert);
   RUN_TEST(test_pop);
   return UNITY_END();
}
