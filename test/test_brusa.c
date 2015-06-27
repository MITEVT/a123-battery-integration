#include "brusa.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

static NLG5_ERR_T err;
static CCAN_MSG_OBJ_T msg;


TEST_GROUP(Brusa_Test);

TEST_SETUP(Brusa_Test) {
  	err = 0;
}

TEST_TEAR_DOWN(Brusa_Test) {

}

TEST(Brusa_Test, test_decode_error) {
	msg.mode_id = NLG5_ERR;
	msg.dlc = NLG5_ERR_DLC;
	msg.data[0] = 10;
	msg.data[1] = 0;
	msg.data[2] = 0;
	msg.data[3] = 0;
	msg.data[4] = 0;
	msg.data[5] = 0;
	msg.data[6] = 0;
	msg.data[7] = 0;

	Brusa_DecodeErr(&err, &msg);
	TEST_ASSERT_EQUAL(10, err);
	
}

TEST_GROUP_RUNNER(Brusa_Test) {
	RUN_TEST_CASE(Brusa_Test, test_decode_error);
}