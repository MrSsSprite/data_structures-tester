#include "unity.h"
#include "unity_internals.h"
#include "test_units.h"


void setUp(void) { }
void tearDown(void) { }


int main(void)
{
   UNITY_BEGIN();

   RUN_TEST(test_simp_init);
   RUN_TEST(test_insert_head);
   RUN_TEST(test_insert_mid);
   return UNITY_END();
}
