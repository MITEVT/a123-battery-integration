#include "a123mbb.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>


TEST_GROUP(A123_Test);

TEST_SETUP(A123_Test)
{
  
}

TEST_TEAR_DOWN(A123_Test)
{
}

TEST(A123_Test, test_conversion_macros) {
	TEST_ASSERT_EQUAL(3300, num2mVolts((uint32_t)0x11f9));
	TEST_ASSERT_EQUAL(3295, num2mVolts((uint32_t)0x11ee));

	TEST_ASSERT_EQUAL(4000, mVolts2Num(3000));
	
}

TEST_GROUP_RUNNER(A123_Test)
{
	RUN_TEST_CASE(A123_Test, test_conversion_macros);
}