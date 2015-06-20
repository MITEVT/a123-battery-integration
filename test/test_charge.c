#include "charge.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

#define PACK_S 22
#define PACK_P 3
#define MAX_CELL_MVOLTS 3600
#define CC_CELL_MVOLTS 3700
#define CELL_CAMP_HOURS 2000
#define CELL_mC_RATING 1000
#define CC_PACK_MVOLTS CC_CELL_MVOLTS * PACK_S
#define CV_PACK_MVOLTS MAX_CELL_MVOLTS * PACK_S
#define CURRENT_LIMIT CELL_CAMP_HOURS * (CELL_mC_RATING/1000) * PACK_P


PACK_STATE_T pack_state;	
OUTPUT_STATE_T out_state;


TEST_GROUP(Charge_Test);

TEST_SETUP(Charge_Test) {

	CHARGING_CONFIG_T config;
	config.pack_s = PACK_S;
	config.pack_p = PACK_P;
	config.max_cell_mVolts = MAX_CELL_MVOLTS;
	config.cc_cell_mVolts = CC_CELL_MVOLTS;
	config.cell_capacity_cAmpHours = CELL_CAMP_HOURS;
	config.cell_mC_rating = CELL_mC_RATING;
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
	pack_state.brusa_error_message = 0;

	out_state.brusa_mVolts = 0;
	out_state.brusa_cAmps = 0;
	out_state.close_contactors = false;
	out_state.brusa_output = false;
  
}

TEST_TEAR_DOWN(Charge_Test) {
	// Return to initial state
	Charge_Step(&pack_state, REQ_IDLE, &out_state);
}

TEST(Charge_Test, test_contactors_open) {
	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode, should stay in init until contactors turn on.
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	int i;
	for (i = 0; i < 100; i++) {
		TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
		TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
		TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
		TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
		TEST_ASSERT_EQUAL(true, out_state.brusa_output);
		TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	}

	pack_state.contactors_closed = true;

	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cc_w_bal) {
	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3450;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should be balancing to min val
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cc_w_out_bal) {
	pack_state.pack_min_mVolts = 34005/10;
	pack_state.pack_max_mVolts = 3401;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should not be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cv_w_bal) {
	pack_state.pack_min_mVolts = 3596;
	pack_state.pack_max_mVolts = 3600;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CV Range
	// Mode should change to CV, Output current should be max current 
	// and output voltage should be CV voltage
	// should be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cv_w_out_bal) {
	pack_state.pack_min_mVolts = 3599;
	pack_state.pack_max_mVolts = 3600;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CV Range
	// Mode should change to CV, Output current should be max current 
	// and output voltage should be CV voltage
	// should not be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cc_to_cv_w_bal) {
	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3405;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);


	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Change pack state to be in cv range
	pack_state.pack_min_mVolts = 3595;
	pack_state.pack_max_mVolts = 3600;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cc_to_cv_w_out_bal) {
	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3400;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);


	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Change pack state to be in cv range
	pack_state.pack_min_mVolts = 3598;
	pack_state.pack_max_mVolts = 3600;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_to_cv_to_done) {
	pack_state.pack_min_mVolts = 3600;
	pack_state.pack_max_mVolts = 3600;
	pack_state.pack_cAmps_in = 50;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CV Range
	// Mode should change to CV, Output current should be max current 
	// and output voltage should be CV voltage
	// should not be balancing
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CV, Charge_GetMode());
	TEST_ASSERT_EQUAL(CV_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Pack should stop charging
	pack_state.msTicks = 60000;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_DONE, Charge_GetMode());

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST(Charge_Test, test_fully_balance) {
	pack_state.pack_min_mVolts = 3400;
	pack_state.pack_max_mVolts = 3405;

	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Request Charging Mode
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	// Turn contactors on, set pack_v_max to within CC Range
	// Mode should change to CC, Output current should be max current 
	// and output voltage should be CC voltage
	// should be balancing to min val
	pack_state.contactors_closed = true;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Should keep balancing
	int i;
	for (i = 1; i < 4; i++) {
		pack_state.pack_min_mVolts = 3400 + i;
		TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
		TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
		TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
		TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
		TEST_ASSERT_EQUAL(true, out_state.brusa_output);
		TEST_ASSERT_EQUAL(true, out_state.close_contactors);
		TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
		TEST_ASSERT_EQUAL(true, out_state.balance);
	}

	// Should stop balancing
	pack_state.pack_min_mVolts = 3405;
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(BCM_BALANCE_OFF, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(false, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);	
}

TEST(Charge_Test, test_brusa_error) {
	// Charge State Machine should start out OFF
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());

	// Set a brusa error
	pack_state.brusa_error_message = 0x0F;

	// Request Charging Mode, should stay in init until contactors turn on and no brusa errors
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	pack_state.contactors_closed = true;

	// Should stay in init bc of brusa error
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_CHARGING, &out_state));
	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);

	pack_state.brusa_error_message = 0;

	// Now shoudl go ahead to next state
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_NONE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_CC, Charge_GetMode());
	TEST_ASSERT_EQUAL(CC_PACK_MVOLTS, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(CURRENT_LIMIT, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(true, out_state.brusa_output);
	TEST_ASSERT_EQUAL(true, out_state.close_contactors);
	TEST_ASSERT_EQUAL(pack_state.pack_min_mVolts, out_state.balance_mVolts);
	TEST_ASSERT_EQUAL(true, out_state.balance);

	// Turn off Charging
	TEST_ASSERT_EQUAL(ERROR_NONE, Charge_Step(&pack_state, REQ_IDLE, &out_state));
	TEST_ASSERT_EQUAL(CHRG_OFF, Charge_GetMode());
	TEST_ASSERT_EQUAL(0, out_state.brusa_mVolts);
	TEST_ASSERT_EQUAL(0, out_state.brusa_cAmps);
	TEST_ASSERT_EQUAL(false, out_state.brusa_output);
	TEST_ASSERT_EQUAL(false, out_state.close_contactors);
}

TEST_GROUP_RUNNER(Charge_Test) {
	RUN_TEST_CASE(Charge_Test, test_contactors_open);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_w_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_w_out_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cv_w_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cv_w_out_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_to_cv_w_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cc_to_cv_w_out_bal);
  	RUN_TEST_CASE(Charge_Test, test_to_cv_to_done);
  	RUN_TEST_CASE(Charge_Test, test_fully_balance);
  	RUN_TEST_CASE(Charge_Test, test_brusa_error);
}







