#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(A123_CAN_decode);
}

int main(int argc, char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}