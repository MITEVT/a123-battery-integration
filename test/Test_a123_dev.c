#include "devices/a123_can_dev.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>



TEST_GROUP(A123_CAN_decode);

TEST_SETUP(A123_CAN_decode)
{
  
}

TEST_TEAR_DOWN(A123_CAN_decode)
{
}

TEST(A123_CAN_decode, decode_standard) {
	uint8_t buffer[8] = {0x8C, 0x00, 0x00, 0x07, 0x00, 0x07, 0xFF, 0xFF};
	uint64_t buf = 0x8C0000070007FFFF;
	// mbb_std_msg_t msg;
	// a123_decode_standard(&msg, buf);
	a123_msg_t msgwrap;
	a123_decode(&msgwrap, MBB_STANDARD_ID, 8, buffer);
	mbb_std_msg_t * msg = (mbb_std_msg_t*)(&msgwrap.msg);
	printf("PRINTINGL %d %d %d", msg->cell_overvolt, msg->cell_undervolt, msg->response_id);
	TEST_ASSERT_EQUAL_UINT8(msg->cell_overvolt, 1);
	TEST_ASSERT_EQUAL_UINT8(msg->cell_undervolt, 1);
	TEST_ASSERT_EQUAL_UINT8(msg->response_id, 8);
	TEST_ASSERT_EQUAL_INT8(msg->therm_x, -400);
	TEST_ASSERT_EQUAL_UINT16(msg->v_min, 10000);
	TEST_ASSERT_EQUAL_UINT8(msg->temp_chn, 7);
	TEST_ASSERT_EQUAL_UINT16(msg->v_max, 10000);
	TEST_ASSERT_EQUAL_UINT8(msg->bal_cnt, 7);
	TEST_ASSERT_EQUAL_UINT16(msg->v_ave, (0xFFFF >> 3) * 5 + 10000);
	TEST_ASSERT_EQUAL_UINT8(msg->voltage_compare_oor, 1);
}

TEST_GROUP_RUNNER(A123_CAN_decode)
{
  RUN_TEST_CASE(A123_CAN_decode, decode_standard);
}