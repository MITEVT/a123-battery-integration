#include "temptest.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(ProductionCode);

TEST_SETUP(ProductionCode)
{
  //do nothing
}

TEST_TEAR_DOWN(ProductionCode)
{
}

TEST(ProductionCode, addOnetoZero)
{
  //All of these should pass
  TEST_ASSERT_EQUAL(1, addOne(0));
}

