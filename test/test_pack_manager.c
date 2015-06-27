#include "pack_manager.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>


PACK_STATE_T pack_state;
CCAN_MSG_OBJ_T msg;

#define SET_STD(obj, mod_id) {obj.mode_id = MBB_STD | mod_id; obj.dlc = MBB_STD_DLC;}
#define SET_MIN(obj, mVolts) {obj.data[3] &= 0x07; obj.data[3] |= (mVolts2Num(mVolts) & 0x1F) << 3; \
							obj.data[2] = mVolts2Num(mVolts) >> 5;}
#define SET_MAX(obj, mVolts) {obj.data[5] &= 0x07; obj.data[5] |= (mVolts2Num(mVolts) & 0x1F) << 3; \
							obj.data[4] = mVolts2Num(mVolts) >> 5;}

TEST_GROUP(PackManager_Test);

TEST_SETUP(PackManager_Test) {

	PackManager_Init();

	pack_state.pack_min_mVolts = 0;
	pack_state.pack_max_mVolts = 0;

	msg.mode_id = 0;
	msg.dlc = 0;
	msg.data[0] = 0;
	msg.data[1] = 0;
	msg.data[2] = 0;
	msg.data[3] = 0;
	msg.data[4] = 0;
	msg.data[5] = 0;
	msg.data[6] = 0;
	msg.data[7] = 0;
}

TEST_TEAR_DOWN(PackManager_Test) {
	
}

TEST(PackManager_Test, test_to_commit) {
	int8_t res;

	SET_STD(msg, 0x01);
	SET_MIN(msg, 3000);
	SET_MAX(msg, 3010);
	res = PackManager_Update(&msg);
	TEST_ASSERT_EQUAL(0, res);

	SET_STD(msg, 0x05);
	msg.dlc = 1;
	SET_MIN(msg, 2000);
	SET_MAX(msg, 3000);
	res = PackManager_Update(&msg);
	TEST_ASSERT_EQUAL(-1, res);

	SET_STD(msg, 0x02);
	SET_MIN(msg, 2999);
	SET_MAX(msg, 3009);
	res = PackManager_Update(&msg);
	TEST_ASSERT_EQUAL(0, res);

	SET_STD(msg, 0x03);
	SET_MIN(msg, 3001);
	SET_MAX(msg, 3020);
	res = PackManager_Update(&msg);
	TEST_ASSERT_EQUAL(-1, res);

	PackManager_Commit(&pack_state);
	TEST_ASSERT_EQUAL(2999, pack_state.pack_min_mVolts);
	TEST_ASSERT_EQUAL(3010, pack_state.pack_max_mVolts);

	SET_STD(msg, 0x02);
	SET_MIN(msg, 3000);
	SET_MAX(msg, 3010);
	res = PackManager_Update(&msg);
	TEST_ASSERT_EQUAL(0, res);

	PackManager_Commit(&pack_state);
	TEST_ASSERT_EQUAL(3000, pack_state.pack_min_mVolts);
	TEST_ASSERT_EQUAL(3010, pack_state.pack_max_mVolts);


}


TEST_GROUP_RUNNER(PackManager_Test) {
	RUN_TEST_CASE(PackManager_Test, test_to_commit);
}







