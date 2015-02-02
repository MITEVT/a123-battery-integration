#include "devices/a123_can_dev.h"
#include "unity.h"
#include "unity_fixture.h"



TEST_GROUP(A123_CAN_decode);

TEST_SETUP(A123_CAN_decode)
{
  
}

TEST_TEAR_DOWN(A123_CAN_decode)
{
}

TEST(A123_CAN_decode, decode_standard)
{
  uint64_t buf = 0xFF8FFFFFFFFFFFFF;
  mbb_std_msg_t msg;
  a123_decode_standard(&msg, buf);
  TEST_ASSERT_EQUAL(msg.cell_undervolt, 1);
}

TEST_GROUP_RUNNER(A123_CAN_decode)
{
  RUN_TEST_CASE(A123_CAN_decode, decode_standard);
}