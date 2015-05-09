#include "charge.h"

static CHARGING_MODE_T mode = CHRG_OFF;

static uint64_t lastTimeAbove1A = 0;
static uint32_t max_pack_mVolts;
static uint32_t max_cell_mVolts;
static uint32_t cc_pack_mVolts;
static uint16_t pack_capacity_cAmpHours;
static uint16_t max_charge_cAmps;
static bool balancing;

void Charge_Config(CHARGING_CONFIG_T *config) {
	max_cell_mVolts = config->max_cell_mVolts;
	max_pack_mVolts = max_cell_mVolts * config->pack_s;
	cc_pack_mVolts = config->cc_cell_mVolts * config->pack_s;
	pack_capacity_cAmpHours = config->cell_capacity_cAmpHours * config->pack_p;
	max_charge_cAmps = pack_capacity_cAmpHours * config->cell_c_rating;
	// Store all values into EEPROM
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
				// If max cell voltage is less than allowable max cell voltage
				// 		Charge in CC
				// Else
				// 		Charge in CV
				mode = (pack_state->pack_max_mVolts < max_cell_mVolts) ? CHRG_CC : CHRG_CV;
			}
			out_state->balance_mVolts = BCM_BALANCE_OFF;
			out_state->brusa_mVolts = 0;
			out_state->brusa_cAmps = 0;
			lastTimeAbove1A = pack_state->msTicks;
			break;
		case CHRG_CC:
			if (pack_state->pack_max_mVolts >= max_cell_mVolts) {
				// Need to go to CV Mode
				mode = CHRG_CV;
				out_state->brusa_mVolts = 0;
				out_state->brusa_cAmps = 0;
			} else {
				// Charge in CC Mode
				out_state->brusa_mVolts = cc_pack_mVolts;
				out_state->brusa_cAmps = max_charge_cAmps;
			}

			if (BCM_BAL_ENABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				out_state->balance_mVolts = pack_state->pack_min_mVolts;
			} else if (BCM_BAL_DISABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				out_state->balance_mVolts = BCM_BALANCE_OFF;
			}
			break;
		case CHRG_CV:

			if (pack_state->pack_max_mVolts < max_cell_mVolts) {
				// Need to go to CC Mode
				mode = CHRG_CC;
				out_state->brusa_mVolts = 0;
				out_state->brusa_cAmps = 0;
			} else {
				// Charge in CV Mode
				out_state->brusa_mVolts = max_pack_mVolts;
				out_state->brusa_cAmps = max_charge_cAmps;
				if (pack_state->pack_cAmps_in < 100) {
					if ((pack_state->msTicks - lastTimeAbove1A) > 60000) { 	// About 60 Seconds
						// Done Charging
						out_state->brusa_mVolts = 0;
						out_state->brusa_cAmps = 0;
						out_state->balance_mVolts = BCM_BALANCE_OFF;
						mode = CHRG_DONE;
						if (BCM_BAL_ENABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
							return CHRG_ERROR;
						}
						break;
					} 
				} else {
					lastTimeAbove1A = pack_state->msTicks;
				}
			}

			if (BCM_BAL_ENABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				out_state->balance_mVolts = pack_state->pack_min_mVolts;
			} else if (BCM_BAL_DISABLE(pack_state->pack_max_mVolts, pack_state->pack_min_mVolts)) {
				out_state->balance_mVolts = BCM_BALANCE_OFF;
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