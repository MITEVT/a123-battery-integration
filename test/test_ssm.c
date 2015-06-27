#include "ssm.h"
#include "charge.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>


PACK_STATE_T pack_state;	
OUTPUT_STATE_T out_state;


TEST_GROUP(SSM_Test);

TEST_SETUP(SSM_Test) {

	SSM_Init();

	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3450;
	pack_state.pack_cAmps_in = 0;
	pack_state.contactors_closed = false;
	pack_state.msTicks = 0;

	out_state.brusa_mVolts = 0;
	out_state.brusa_cAmps = 0;
	out_state.close_contactors = false;
  
}

TEST_TEAR_DOWN(SSM_Test) {
	// Return to initial state
	SSM_Step(&pack_state, INP_IDLE, &out_state);
}

TEST(SSM_Test, test_to_charge) {
	// SuperState Machine should start out OFF
	TEST_ASSERT_EQUAL(IDLE, SSM_GetMode());
	TEST_ASSERT_EQUAL(ERROR_NONE, SSM_Step(&pack_state, INP_CHRG, &out_state));
	TEST_ASSERT_EQUAL(CHARGING, SSM_GetMode());
	TEST_ASSERT_EQUAL(DRAIN_OFF, Drain_GetMode());
}

TEST(SSM_Test, test_to_drain) {
	TEST_ASSERT_EQUAL(IDLE, SSM_GetMode());
	TEST_ASSERT_EQUAL(ERROR_NONE, SSM_Step(&pack_state, INP_DRAIN, &out_state));
	TEST_ASSERT_EQUAL(DRAINING, SSM_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
}

TEST(SSM_Test, test_to_charge_drain) {
	TEST_ASSERT_EQUAL(IDLE, SSM_GetMode());
	TEST_ASSERT_EQUAL(ERROR_NONE, SSM_Step(&pack_state, INP_CHRG, &out_state));
	TEST_ASSERT_EQUAL(CHARGING, SSM_GetMode());
	TEST_ASSERT_EQUAL(ERROR_NONE, SSM_Step(&pack_state, INP_IDLE, &out_state));
	TEST_ASSERT_EQUAL(ERROR_NONE, SSM_Step(&pack_state, INP_IDLE, &out_state)); 
	TEST_ASSERT_EQUAL(IDLE, SSM_GetMode());
	TEST_ASSERT_EQUAL(ERROR_NONE, SSM_Step(&pack_state, INP_DRAIN, &out_state));
	TEST_ASSERT_EQUAL(DRAINING, SSM_GetMode());

}

TEST_GROUP_RUNNER(SSM_Test) {
	RUN_TEST_CASE(SSM_Test, test_to_charge);
	RUN_TEST_CASE(SSM_Test, test_to_drain);
	RUN_TEST_CASE(SSM_Test, test_to_charge_drain);
}







