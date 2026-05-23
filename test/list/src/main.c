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
   RUN_TEST(test_simp_init__d);
   RUN_TEST(test_simp_init__c);
   RUN_TEST(test_simp_init__u);
   RUN_TEST(test_simp_init__l);
   RUN_TEST(test_push__i);
   RUN_TEST(test_push__f);
   RUN_TEST(test_push__d);
   RUN_TEST(test_push__c);
   RUN_TEST(test_push__u);
   RUN_TEST(test_push__l);
   RUN_TEST(test_insert__i);
   RUN_TEST(test_insert__f);
   RUN_TEST(test_insert__d);
   RUN_TEST(test_insert__c);
   RUN_TEST(test_insert__u);
   RUN_TEST(test_insert__l);
   RUN_TEST(test_pop__i);
   RUN_TEST(test_pop__f);
   RUN_TEST(test_pop__d);
   RUN_TEST(test_pop__c);
   RUN_TEST(test_pop__u);
   RUN_TEST(test_pop__l);
   RUN_TEST(test_erase__i);
   RUN_TEST(test_erase__f);
   RUN_TEST(test_erase__d);
   RUN_TEST(test_erase__c);
   RUN_TEST(test_erase__u);
   RUN_TEST(test_erase__l);
   RUN_TEST(test_find__i);
   RUN_TEST(test_find__f);
   RUN_TEST(test_find__d);
   RUN_TEST(test_find__c);
   RUN_TEST(test_find__u);
   RUN_TEST(test_find__l);
   RUN_TEST(test_size__i);
   RUN_TEST(test_size__f);
   RUN_TEST(test_size__d);
   RUN_TEST(test_size__c);
   RUN_TEST(test_size__u);
   RUN_TEST(test_size__l);
   RUN_TEST(test_stress__i);
   RUN_TEST(test_stress__f);
   RUN_TEST(test_stress__d);
   RUN_TEST(test_stress__c);
   RUN_TEST(test_stress__u);
   RUN_TEST(test_stress__l);
   return UNITY_END();
}
