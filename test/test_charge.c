#include "charge.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

#define PACK_S 66
#define PACK_P 9
#define MAX_CELL_MVOLTS 3600
#define CC_CELL_MVOLTS 3700
#define CELL_CAMP_HOURS 2000
#define CELL_C_RATING 1
#define CC_PACK_MVOLTS CC_CELL_MVOLTS * PACK_S
#define CV_PACK_MVOLTS MAX_CELL_MVOLTS * PACK_S
#define CURRENT_LIMIT CELL_CAMP_HOURS * CELL_C_RATING * PACK_P


PACK_STATE_T pack_state;	
OUTPUT_STATE_T out_state;


TEST_GROUP(Charge_Test);

TEST_SETUP(Charge_Test)
{

	CHARGING_CONFIG_T config;
	config.pack_s = PACK_S;
	config.pack_p = PACK_P;
	config.max_cell_mVolts = MAX_CELL_MVOLTS;
	config.cc_cell_mVolts = CC_CELL_MVOLTS;
	config.cell_capacity_cAmpHours = CELL_CAMP_HOURS;
	config.cell_c_rating = CELL_C_RATING;
	Charge_Config(&config);

	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_node_min = 0;
	pack_state.pack_max_mVolts = 3450;
	pack_state.pack_node_max = 1;;
	pack_state.pack_avg_mVolts = 3425;
	pack_state.messagesReceived = 1;
	pack_state.pack_cAmps_in = 0;
	pack_state.contactors_closed = false;
	pack_state.msTicks = 0;

	out_state.brusa_mVolts = 0;
	out_state.brusa_cAmps = 0;
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
		TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
		TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
		TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	}
}

TEST(Charge_Test, test_to_cc_w_bal) {
	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3450;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
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
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
}

TEST(Charge_Test, test_to_cc_w_out_bal) {
	pack_state.pack_min_mVolts = 34005/10;
	pack_state.pack_max_mVolts = 3401;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should not be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
}

TEST(Charge_Test, test_to_cv_w_bal) {
	pack_state.pack_min_mVolts = 3596;
	pack_state.pack_max_mVolts = 3600;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CV Range
	// Mode should change to CV, Output current should be max current 
	// and output voltage should be CV voltage
	// should be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
}

TEST(Charge_Test, test_to_cv_w_out_bal) {
	pack_state.pack_min_mVolts = 3599;
	pack_state.pack_max_mVolts = 3600;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CV Range
	// Mode should change to CV, Output current should be max current 
	// and output voltage should be CV voltage
	// should not be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
}

TEST(Charge_Test, test_to_cc_to_cv_w_bal) {
	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3405;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);


	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);

	// Change pack state to be in cv range
	pack_state.pack_min_mVolts = 3595;
	pack_state.pack_max_mVolts = 3600;
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CHRG_OK, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);

}

TEST_GROUP_RUNNER(Charge_Test)
{
	RUN_TEST_CASE(Charge_Test, test_contactors_open);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_w_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_w_out_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cv_w_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cv_w_out_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_to_cv_w_bal);
}