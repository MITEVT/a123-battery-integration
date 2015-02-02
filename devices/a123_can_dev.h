#define BCM_CMD_ID 0x050
#define BCM_CMD_DLC 3

#define BCM_REQUEST_TYPE_BYTE 0
#define BCM_REQUEST_TYPE_BIT 0
#define BCM_REQUEST_TYPE_MASK 0x3
#define BCM_REQUEST_TYPE_SHIFT (7 - BCM_REQUEST_TYPE_BYTE)*8 + BCM_REQUEST_TYPE_BIT
#define BCM_REQEUST_TYPE_STANDARD 0
#define BCM_REQUEST_TYPE_DETAILED 1

#define BCM_LEAKAGE_CMD_BYTE 0
#define BCM_LEAKAGE_CMD_BIT 2
#define BCM_LEAKAGE_CMD_MASK 0x3
#define BCM_LEAKAGE_CMD_SHIFT (7 - BCM_LEAKAGE_CMD_BYTE)*8 + BCM_LEAKAGE_CMD_BIT

#define BCM_REQUEST_ID_BYTE 0
#define BCM_REQUEST_ID_BIT 4
#define BCM_REQUEST_ID_MASK 0xF
#define BCM_REQUEST_ID_SHIFT (7 - BCM_REQUEST_ID_BYTE)*8 + BCM_REQUEST_ID_BIT

#define BCM_BALANCE_TARGET_BYTE 2
#define BCM_BALANCE_TARGET_BIT 3
#define BCM_BALANCE_TARGET_MASK 0x1FFF
#define BCM_BALANCE_TARGER_SHIFT (7 - BCM_BALANCE_TARGET_BYTE)*8 + BCM_BALANCE_TARGET_BIT
#define BCM_BALANCE_TARGET_OFF 0x1FFE
#define BCM_BALANCE_TARGET_SCALE 5
#define BCM_BALANCE_TARGET_OFFSET 10000

//----------------------------------------------------

#define MBB_STANDARD_ID 0x200
#define MBB_STANDARD_MASK 0x700
#define MBB_STANDARD_DLC 8

#define MBB_CELL_OVERVOLT_BYTE 0
#define MBB_CELL_OVERVOLT_BIT  2
#define MBB_CELL_OVERVOLT_MASK 0x1
#define MBB_CELL_OVERVOLT_SHIFT (7 - MBB_CELL_OVERVOLT_BYTE)*8 + MBB_CELL_OVERVOLT_BIT

#define MBB_CELL_UNDERVOLT_BYTE 0
#define MBB_CELL_UNDERVOLT_BIT  3
#define MBB_CELL_UNDERVOLT_MASK 0x1
#define MBB_CELL_UNDERVOLT_SHIFT (7 - MBB_CELL_UNDERVOLT_BYTE)*8 + MBB_CELL_UNDERVOLT_BIT

#define MBB_RESPONSE_ID_BYTE 0
#define MBB_RESPONSE_ID_BIT 4
#define MBB_RESPONSE_ID_MASK 0xF
#define MBB_RESPONSE_ID_SHIFT (7 - MBB_RESPONSE_ID_BYTE)*8 + MBB_RESPONSE_ID_BIT

#define MBB_THERM_X_BYTE 1
#define MBB_THERM_X_BIT  0
#define MBB_THERM_X_MASK 0x1F
#define MBB_THERM_X_SHIFT (7 - MBB_THERM_X_BYTE)*8 + MBB_THERM_X_BIT
#define MBB_THERM_X_SCALE 5
#define MBB_THERM_X_OFFSET -400

#define MBB_TEMP_CHN_BYTE 3
#define MBB_TEMP_CHN_BIT 0
#define MBB_TEMP_CHN_MASK 0x7
#define MBB_TEMP_CHN_SHIFT (7 - MBB_TEMP_CHN_BYTE)*8 + MBB_TEMP_CHN_BIT

#define MBB_V_MIN_BYTE 3
#define MBB_V_MIN_BIT 3
#define MBB_V_MIN_MASK 0x1FFF
#define MBB_V_MIN_SHIFT (7 - MBB_V_MIN_BYTE)*8 + MBB_V_MIN_BIT
#define MBB_V_MIN_SCALE 5
#define MBB_V_MIN_OFFSET 10000

#define MBB_V_MAX_BYTE 5
#define MBB_V_MAX_BIT 3
#define MBB_V_MAX_MASK 0x1FFF
#define MBB_V_MAX_SHIFT (7 - MBB_V_MAX_BYTE)*8 + MBB_V_MAX_BIT
#define MBB_V_MAX_SCALE 5
#define MBB_V_MAX_OFFSET 10000

#define MBB_BAL_COUNT_BYTE 5
#define MBB_BAL_COUNT_BIT 0
#define MBB_BAL_COUNT_MASK 0x7
#define MBB_BAL_COUNT_SHIFT (7 - MBB_BAL_COUNT_BYTE)*8 + MBB_BAL_COUNT_BIT

#define MBB_V_AVE_BYTE 7
#define MBB_V_AVE_BIT  3
#define MBB_V_AVE_MASK 0x1FFF
#define MBB_V_AVE_SHIFT (7 - MBB_V_AVE_BYTE)*8 + MBB_V_AVE_BIT
#define MBB_V_AVE_SCALE 5
#define MBB_V_AVE_OFFSET 10000

#define MBB_VOLTAGE_COMPARE_OOR_BYTE 7
#define MBB_VOLTAGE_COMPARE_OOR_BIT 2
#define MBB_VOLTAGE_COMPARE_OOR_MASK 0x1
#define MBB_VOLTAGE_COMPARE_OOR_SHIFT (7 - MBB_VOLTAGE_COMPARE_OOR_BYTE)*8 + MBB_VOLTAGE_COMPARE_OOR_BIT

//----------------------------------------------------

typedef enum {BCM_CMD, STANDARD, EXTRA} a123_msg_type;

typedef struct _a123_msg_t_ {
	a123_msg_type type;
	uint32_t msg;
	uint32_t pad1;
	uint32_t pad2;
} a123_msg_t;

typedef struct _bcm_cmd_msg_t_ {
	uint8_t bcm_request_type;
	uint8_t bcm_leakage_cmd;
	uint8_t bcm_request_id;
	uint16_t bcm_v_balance_target;
} bcm_cmd_msg_t;

typedef _mbb_std_msg_t_ {
	uint8_t cell_overvolt;
	uint8_t cell_undervolt;
	uint8_t response_id;
	int8_t therm_x;
	uint16_t v_min;
	uint8_t temp_chn
	uint16_t v_max;
	uint8_t bal_cnt;
	uint16_t v_ave;
	uint8_t voltage_compare_oor;
} mbb_std_msg_t;

int a123_decode(a123_msg_t *msg, uint16_t id, uint8_t dlc, uint8_t *buffer);




