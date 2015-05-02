#include "charge.h"

static CHARGING_MODE_T mode = CHRG_OFF;
static uint16_t max_current = 400; 				// Deci Amps
static uint64_t lastTimeAbove1A = 0;
void Charge_Config(CHARGING_CONFIG_T *config) {
	max_current = config->max_deci_amps_per_p * config->mod_p;
}

CHARGING_STATUS_T Charge_Step(PACK_STATE_T *pack_state, MODE_REQUEST_T req_mode, OUTPUT_STATE_T *out_state) {
	switch (req_mode) {
		case REQ_CHARGING:
			// Do nothing
			mode = CHRG_INIT;
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
		case REQ_NONE:
			// Chill
			break;
	}

	switch(mode) {
		case CHRG_OFF:
			// Wut, this shouldn't happen, software bug though
			return CHRG_ERROR;
			break;
		case CHRG_INIT:
			// Start charging
			if (!pack_state->contactors_closed) {
				out_state->close_contactors = true;
			} else {
				// Go into either CC or CV mode depending on max cell voltage
				mode = BCM_CHRG_STATE(pack_state->pack_v_max);
			}
			out_state->balance_voltage = BCM_BALANCE_OFF;
			out_state->brusa_deci_volts = 0;
			out_state->brusa_deci_amps = 0;
			lastTimeAbove1A = pack_state->msTicks;
			break;
		case CHRG_CC:
			if (pack_state->pack_v_max >= BCM_CHRG_CC_CELL_V_MAX) {
				// Need to go to CV Mode
				mode = CHRG_CV;
				out_state->brusa_deci_volts = 0;
				out_state->brusa_deci_amps = 0;
			} else {
				// Charge in CC Mode
				out_state->brusa_deci_volts = BCM_CHRG_CC_BRUSA_DECI_V;
				out_state->brusa_deci_amps = max_current;
			}

			if (BCM_BAL_ENABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
				out_state->balance_voltage = pack_state->pack_v_min;
			} else if (BCM_BAL_DISABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
				out_state->balance_voltage = BCM_BALANCE_OFF;
			}
			break;
		case CHRG_CV:

			if (pack_state->pack_v_max < BCM_CHRG_CC_CELL_V_MAX) {
				// Need to go to CC Mode
				mode = CHRG_CC;
				out_state->brusa_deci_volts = 0;
				out_state->brusa_deci_amps = 0;
			} else {
				// Charge in CV Mode
				out_state->brusa_deci_volts = BCM_CHRG_CV_BRUSA_DECI_V;
				out_state->brusa_deci_amps = max_current;
				if (pack_state->centi_amps_in < 100) {
					if ((pack_state->msTicks - lastTimeAbove1A) > 60000) { 	// About 60 Seconds
						// Done Charging
						out_state->brusa_deci_volts = 0;
						out_state->brusa_deci_amps = 0;
						out_state->balance_voltage = BCM_BALANCE_OFF;
						mode = CHRG_DONE;
						if (BCM_BAL_ENABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
							return CHRG_ERROR;
						}
						break;
					} 
				} else {
					lastTimeAbove1A = pack_state->msTicks;
				}
			}

			if (BCM_BAL_ENABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
				out_state->balance_voltage = pack_state->pack_v_min;
			} else if (BCM_BAL_DISABLE(pack_state->pack_v_max, pack_state->pack_v_min)) {
				out_state->balance_voltage = BCM_BALANCE_OFF;
			}
			break;
		case CHRG_DONE:
			out_state->close_contactors = false;
			break;
	}

	return CHRG_OK;
}

CHARGING_MODE_T Charge_GetMode() {
	return mode;
}