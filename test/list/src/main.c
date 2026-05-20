#include "unity.h"
#include "unity_internals.h"
#include "test_units.h"


void setUp(void) { }
void tearDown(void) { }


int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_simp_init__i);
   RUN_TEST(test_simp_init__f);
   RUN_TEST(test_push__i);
   RUN_TEST(test_push__f);
   RUN_TEST(test_insert__i);
   RUN_TEST(test_insert__f);
   RUN_TEST(test_pop__i);
   RUN_TEST(test_pop__f);
   RUN_TEST(test_erase__i);
   RUN_TEST(test_erase__f);
   RUN_TEST(test_find__i);
   RUN_TEST(test_find__f);
   RUN_TEST(test_size__i);
   RUN_TEST(test_size__f);
   RUN_TEST(test_stress__i);
   RUN_TEST(test_stress__f);
   return UNITY_END();
}
