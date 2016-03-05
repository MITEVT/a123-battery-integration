#include "balance.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

PACK_STATE_T pack_state;	
OUTPUT_STATE_T out_state;

TEST_GROUP(Balance_Test);

TEST_SETUP(Balance_Test) {

	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3450;
	pack_state.pack_cAmps_in = 0;
	pack_state.contactors_closed = false;
	pack_state.msTicks = 0;
	pack_state.brusa_error = 0;

	out_state.brusa_mVolts = 0;
	out_state.brusa_cAmps = 0;
	out_state.close_contactors = false;
	out_state.brusa_output = false;
  
}

TEST_TEAR_DOWN(Balance_Test) {
	// Return to initial state
	Balance_Step(&pack_state, REQ_IDLE, &out_state);
}

TEST(Balance_Test, test_balance_init) {
	// Balance State Machine should start out OFF
	TEST_ASSERT_EQUAL(BAL_OFF, Balance_GetMode());

	TEST_ASSERT_EQUAL(ERROR_NONE, Balance_Step(&pack_state, REQ_BALANCING, &out_state));
	TEST_ASSERT_EQUAL(BAL_ON, Balance_GetMode());
	TEST_ASSERT_EQUAL(true, out_state.balance);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);

	// Turn off Balancing
	TEST_ASSERT_EQUAL(ERROR_NONE, Balance_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(BAL_OFF, Balance_GetMode());
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
	TEST_ASSERT_EQUAL(false, out_state.balance);
}

TEST(Balance_Test, test_balance_finish) {
	// Balance State Machine should start out OFF
	TEST_ASSERT_EQUAL(BAL_OFF, Balance_GetMode());

	TEST_ASSERT_EQUAL(ERROR_NONE, Balance_Step(&pack_state, REQ_BALANCING, &out_state));
	TEST_ASSERT_EQUAL(BAL_ON, Balance_GetMode());
	TEST_ASSERT_EQUAL(true, out_state.balance);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);

	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3401;

	TEST_ASSERT_EQUAL(ERROR_NONE, Balance_Step(&pack_state, REQ_BALANCING, &out_state));
	TEST_ASSERT_EQUAL(BAL_ON, Balance_GetMode());
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Turn off Balancing
	TEST_ASSERT_EQUAL(ERROR_NONE, Balance_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(BAL_OFF, Balance_GetMode());
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
	TEST_ASSERT_EQUAL(false, out_state.balance);
}

TEST_GROUP_RUNNER(Balance_Test) {
	RUN_TEST_CASE(Balance_Test, test_balance_init);
	RUN_TEST_CASE(Balance_Test, test_balance_finish);
}







