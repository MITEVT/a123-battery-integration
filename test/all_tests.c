#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(Charge_Test);
}

int main(int argc, char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}