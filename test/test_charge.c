#include "charge.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

PACK_STATE_T pack_state;	
OUTPUT_STATE_T out_state;


TEST_GROUP(Charge_Test);

TEST_SETUP(Charge_Test)
{
	pack_state.pack_v_min = mVolts2Num(3400);
	pack_state.pack_node_min = 0;
	pack_state.pack_v_max = mVolts2Num(3450);
	pack_state.pack_node_max = 1;;
	pack_state.pack_v_avg = mVolts2Num(3425);
	pack_state.messagesReceived = 1;
	pack_state.centi_amps_in = 0;
	pack_state.contactors_closed = false;
	pack_state.msTicks = 0;

	out_state.brusa_deci_volts = 0;
	out_state.brusa_deci_amps = 0;
	out_state.close_contactors = false;
  
}

TEST_TEAR_DOWN(Charge_Test)
{
	// Return to initial state
	Charge_Step(&pack_state, REQ_IDLE, &out_state);
}

TEST(Charge_Test, test_contactors_open) {
	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode, should stay in init until contactors turn on.
	int i;
	for (i = 0; i < 1000; i++) {
		TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
		TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
		TEST_ASSERT_EQUAL(0, out_state.brusa_deci_amps);
		TEST_ASSERT_EQUAL(0, out_state.brusa_deci_volts);
		TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	}
}

TEST(Charge_Test, test_to_cc_w_bal) {
	pack_state.pack_v_min = mVolts2Num(3401);
	pack_state.pack_v_max = mVolts2Num(3450);
	pack_state.centi_amps_in = 0;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_deci_amps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_deci_volts);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should be balancing to min val
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(BCM_CHRG_CC_BRUSA_DECI_V, out_state.brusa_deci_volts);
	TEST_ASSERT_EQUAL(400, out_state.brusa_deci_amps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(true, BCM_BAL_ENABLE(pack_state.pack_v_max, pack_state.pack_v_min));
	TEST_ASSERT_EQUAL(pack_state.pack_v_min, out_state.balance_voltage);
}

TEST_GROUP_RUNNER(Charge_Test)
{
	RUN_TEST_CASE(Charge_Test, test_contactors_open);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_w_bal);
}