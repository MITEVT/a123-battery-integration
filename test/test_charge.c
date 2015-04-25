#include "charge.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>



TEST_GROUP(Charge_Test);

TEST_SETUP(Charge_Test)
{
  
}

TEST_TEAR_DOWN(Charge_Test)
{
}

TEST(Charge_Test, test_balance) {
	PACK_STATE pack_state;
	pack_state.pack_v_min = mVolts2Num(3400);
	pack_state.pack_node_min = 0;
	pack_state.pack_v_max = mVolts2Num(3450);
	pack_state.pack_node_max = 1;;
	pack_state.pack_v_avg = mVolts2Num(3425);;
	pack_state.messagesReceived = 1;
	pack_state.contactors_closed = false;

	//OUTPUT_STATE out_state;

	TEST_ASSERT_EQUAL(CHRG_INIT, Charge_GetMode());
}

TEST_GROUP_RUNNER(Charge_Test)
{
  RUN_TEST_CASE(Charge_Test, test_balance);
}