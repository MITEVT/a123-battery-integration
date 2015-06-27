#include "pack_manager.h"

static MBB_STD_T mbb_std[NODE_COUNT];
static MBB_EXT_T mbb_ext[NODE_COUNT];

void PackManager_Init() {
	uint8_t i;
	for (i = 0; i < NODE_COUNT; i++) {
		mbb_std[i].id = 0xFF;
		mbb_ext[i].id = 0xFF;

		mbb_std[i].mod_min_mVolts = 0xFFFFFFFF;
		mbb_std[i].mod_max_mVolts = 0;

		mbb_ext[i].bal = 0;
	}
}

int8_t PackManager_Update(CCAN_MSG_OBJ_T *msg_obj) {
	uint8_t mod_id = MBB_GetModID(msg_obj->mode_id);
	if (MBB_IsStandard(msg_obj->mode_id)) {
		uint8_t i;
		for (i = 0; i < NODE_COUNT; i++) {
			if (mbb_std[i].id == 0xFF || mod_id == mbb_std[i].id) {
				return MBB_DecodeStd(&mbb_std[i], msg_obj);
			}
		}
		return -1;
	} else if (MBB_IsExtended(msg_obj->mode_id)) {
		uint8_t i;
		for (i = 0; i < NODE_COUNT; i++) {
			if (mbb_ext[i].id == 0xFF || mod_id == mbb_ext[i].id) {
				return MBB_DecodeExt(&mbb_ext[i], msg_obj);
			}
		}
		return -1;
	}

	return -1;
}

void PackManager_Commit(PACK_STATE_T *pack_state) {
	uint8_t i;
	uint32_t min = mbb_std[0].mod_min_mVolts;
	uint32_t max = mbb_std[0].mod_max_mVolts;

	for (i = 1; i < NODE_COUNT; i++) {
		min = (mbb_std[i].mod_min_mVolts < min) ? mbb_std[i].mod_min_mVolts : min;
		max = (mbb_std[i].mod_max_mVolts > max) ? mbb_std[i].mod_max_mVolts : max;
	}

	pack_state->pack_min_mVolts = min;
	pack_state->pack_max_mVolts = max;
}

uint8_t PackManager_GetExtModId(uint8_t index) {
	if (index >= NODE_COUNT) return 0xFE;
	return mbb_ext[index].id;
}

uint16_t PackManager_GetExtBal(uint8_t index) {
	if (index >= NODE_COUNT) return 0x00;
	return mbb_ext[index].bal;
}

