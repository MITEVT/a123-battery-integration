//Progresses state of DRAIN state machine
//	If function is called, current mode must be DRAINING
//	next_mode is the next requested mode

//INIT

#include "drain.h"

DRAIN_RET_STATUS_T (PACK_STATE pack_state, MODE_T next_mode){
	switch(next_mode){
		case IDLE:
			break;

		case CHARGING:
			//Check if can continue drain
			//Return output state request
			break;

		case DRAINING:
			//Check if can continue drain
			//Return output state request, contactor = close
			break;
	}
}
