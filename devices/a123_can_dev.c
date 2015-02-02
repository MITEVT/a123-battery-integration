#include "a123_can_dev.h"

int a123_decode(a123_msg_t *msg, uint16_t id, uint8_t dlc, uint8_t *buffer) {
	uint64_t buf = 0;
	int i;

	/* Convert buffer to 64 bit value */
	for (i = 0; i < 8; i++) {
		buf += buffer[i] << (7 - i)*8;
	}

	switch (id) {
		case MBB_STANDARD_ID:
			mbb_std_msg_t *msgptr = (mbb_std_msg_t*)(msg->msg);
			a123_decode_standard(msgptr, buf);
			break;
		default:
			return 0;
	}

	return 1;
}

void a123_decode_standard(mbb_std_msg_t *msgptr, uint64_t buf) {
	mbb_std_msg_t *msgptr = (mbb_std_msg_t*)(msg->msg);
	msgptr->cell_overvolt = (buf >> MBB_CELL_OVERVOLT_SHIFT) & MBB_CELL_OVERVOLT_MASK;
	msgptr->cell_undervolt = (buf >> MBB_CELL_UNDERVOLT_SHIFT) & MBB_CELL_UNDERVOLT_MASK;
	msgptr->response_id = (buf >> MBB_RESPONSE_ID_SHIFT) & MBB_RESPONSE_ID_MASK;
	msgptr->therm_x = ((buf >> MBB_THERM_X_SHIFT) & MBB_THERM_X_MASK) * MBB_THERM_X_SCALE + MBB_THERM_X_OFFSET;
	msgptr->v_min = ((buf >> MBB_V_MIN_SHIFT) & MBB_V_MIN_MASK) * MBB_V_MIN_SCALE + MBB_V_MIN_OFFSET;
	msgptr->temp_chn = (buf >> MBB_TEMP_CHN_SHIFT) & MBB_TEMP_CHN_MASK;
	msgptr->v_max = ((buf >> MBB_V_MAX_SHIFT) & MBB_V_MAX_MASK) * MBB_V_MAX_SCALE + MBB_V_MAX_OFFSET;
	msgptr->bal_cnt = (buf >> MBB_BAL_COUNT_SHIFT) & MBB_BAL_COUNT_MASK;
	msgptr->v_ave = ((buf >> MBB_V_AVE_SHIFT) & MBB_V_AVE_MASK) * MBB_V_AVE_SCALE + MBB_V_AVE_OFFSET;
	msgptr->voltage_compare_oor = (buf >> MBB_VOLTAGE_COMPARE_OOR_SHIFT) * MBB_VOLTAGE_COMPARE_OOR_MASK;
}

void a123_encode_bcm_cmd(bcm_cmd_msg_t *msg, uint8_t *buffer) {
	uint64_t buf = 0;
	int i;

	buf += (msgptr->leakage_cmd & BCM_LEAKAGE_CMD_MASK) << BCM_LEAKAGE_CMD_SHIFT;
	buf += (msgptr->request_type & BCM_REQUEST_TYPE_MASK) << BCM_REQUEST_TYPE_SHIFT;
	buf += (msgptr->request_id & BCM_REQUEST_ID_MASK) << BCM_REQUEST_ID_SHIFT;
	buf += (msgptr->balance_target & BCM_BALANCE_TARGET_MASK) << BCM_BALANCE_TARGER_SHIFT;

	for (i = 0; i < 8; i++) {
		buffer[i] = (buf >> (7 - i)*8) & 0xFF;
	}
}