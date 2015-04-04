//Progresses state of DRAIN state machine
//	If function is called, current mode is DRAINING
//	next_mode is the next requested mode

typedef enum {OPEN, CLOSED} CONTACTOR_STATUS;

void open_contactor() {}
void close_contactor() {}

DRAIN_RET_STATUS_T stepDrain(PACK_STATE pack_state, MODE_T next_mode){
	switch(next_mode){
		case IDLE:
			open_contactor();
			break;

		case CHARGING:
			open_contactor();
			break;

		case DRAINING:
			if(BCM_DRAIN_ALLOWED(pack_state.pack_v_min)) {

			}
			close_contactor();
			break;
	}
}
