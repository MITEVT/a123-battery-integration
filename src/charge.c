#include "charge.h"

static CHARGING_MODE_T mode = CHRG_OFF;

static uint64_t lastTimeAbove1A;
static uint32_t max_pack_mVolts;
static uint32_t max_cell_mVolts;
static uint32_t cc_pack_mVolts;
static uint32_t pack_capacity_cAmpHours;
static uint32_t max_charge_cAmps;
static bool balancing;

void Charge_Config(CHARGING_CONFIG_T *config) {
	mode = CHRG_OFF;
	max_cell_mVolts = config->max_cell_mVolts;
	max_pack_mVolts = max_cell_mVolts * config->pack_s;
	cc_pack_mVolts = config->cc_cell_mVolts * config->pack_s;
	pack_capacity_cAmpHours = config->cell_capacity_cAmpHours * config->pack_p;
	max_charge_cAmps = pack_capacity_cAmpHours * (uint32_t)(config->cell_mC_rating) / 1000;
	lastTimeAbove1A = 0;
	balancing = false;
	// Store all values into EEPROM
}

ERROR_T Charge_Step(PACK_STATE_T *pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T *out_state) {
	switch (req_mode) {
		case REQ_CHARGING:
			// If starting from not charging, go to init, else stay where you are
			mode = (mode == CHRG_OFF) ? CHRG_INIT : mode;
			break;
		case REQ_DRAINING:
			// Move to exit state
			mode = CHRG_OFF;
			out_state->close_contactors = false;
			break;
		case REQ_IDLE:
			// Move to exit state
			mode = CHRG_OFF;
			out_state->close_contactors = false;
			break;
		case REQ_BALANCING:
			// Move to exit state
			mode = CHRG_OFF;
			out_state->close_contactors = false;
		case REQ_NONE:
			// Chill
			break;
	}
handler:
	out_state->brusa_clear_latch = false;
	switch(mode) {
		case CHRG_OFF:
			out_state->balance = false;
			out_state->brusa_output = false;
			out_state->close_contactors = false;
			break;
		case CHRG_INIT:
			// Start charging
			out_state->balance = false;
			out_state->brusa_mVolts = 0;
			out_state->brusa_cAmps = 0;
			out_state->brusa_output = true;

			lastTimeAbove1A = pack_state->msTicks;
			if (!pack_state->contactors_closed) {
				out_state->close_contactors = true;
			} else if (pack_state->brusa_error != 0) {
				out_state->brusa_clear_latch = true;
				break;
			} else {
				// If max cell voltage is less than allowable max cell voltage
				// 		Charge in CC
				// Else
				// 		Charge in CV
				mode = (pack_state->pack_max_mVolts < max_cell_mVolts) ? CHRG_CC : CHRG_CV;
				goto handler;
			}
			
			break;
		case CHRG_CC:

			if (pack_state->brusa_error != 0) {
				out_state->brusa_output = false;
				out_state->balance = false;
				out_state->close_contactors = false;
				mode = CHRG_OFF;
				return ERROR_BRUSA;
			}

			if (pack_state->pack_max_mVolts >= max_cell_mVolts) {
				// Need to go to CV Mode
				mode = CHRG_CV;
				goto handler;
			} else {
				// Charge in CC Mode
				out_state->brusa_mVolts = cc_pack_mVolts;
				out_state->brusa_cAmps = max_charge_cAmps;
				out_state->brusa_output = true;
			}

			if (!balancing && BCM_BAL_ENABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				balancing = true;
			} else if (balancing && 
						BCM_BAL_DISABLE(pack_state->pack_max_mVolts, 
										pack_state->pack_min_mVolts)) {
				balancing = false;
			}

			if (balancing) {
				out_state->balance = true;
				out_state->balance_mVolts = pack_state->pack_min_mVolts;
			} else {
				out_state->balance = false;
				out_state->balance_mVolts = BCM_BALANCE_OFF;
			}
			break;
		case CHRG_CV:

			if (pack_state->brusa_error != 0) {
				out_state->brusa_output = false;
				out_state->balance = false;
				out_state->close_contactors = false;
				mode = CHRG_OFF;
				return ERROR_BRUSA;
			}

			if (pack_state->pack_max_mVolts < max_cell_mVolts) {
				// Need to go to CC Mode
				mode = CHRG_CC;
				goto handler;
			} else {
				// Charge in CV Mode
				out_state->brusa_mVolts = max_pack_mVolts;
				out_state->brusa_cAmps = max_charge_cAmps;
				out_state->brusa_output = true;
				if (pack_state->pack_cAmps_in < 100) {
					if ((pack_state->msTicks - lastTimeAbove1A) >= 60000) { 	// About 60 Seconds
						// Done Charging
						out_state->brusa_output = false;
						out_state->balance = false;
						mode = CHRG_DONE;
						balancing = false;
						break;
					} 
				} else {
					lastTimeAbove1A = pack_state->msTicks;
				}
			}

			if (!balancing && BCM_BAL_ENABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				balancing = true;
			} else if (balancing && 
							BCM_BAL_DISABLE(pack_state->pack_max_mVolts, 
											pack_state->pack_min_mVolts)) {
				balancing = false;
			}

			if (balancing) {
				out_state->balance = true;
				out_state->balance_mVolts = pack_state->pack_min_mVolts;
			} else {
				out_state->balance = false;
				out_state->balance_mVolts = BCM_BALANCE_OFF;
			}
			break;
		case CHRG_DONE:
			out_state->close_contactors = false;
			break;
	}

	return ERROR_NONE;
}

CHARGING_MODE_T Charge_GetMode() {
	return mode;
}