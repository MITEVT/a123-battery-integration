#ifndef _PACK_MANAGER_H_
#define _PACK_MANAGER_H_

#include "config.h"
#include "state_types.h"
#include "a123mbb.h"

void PackManager_Init(void);
int8_t PackManager_Update(CCAN_MSG_OBJ_T *msg);
void PackManager_Commit(PACK_STATE_T *pack_state);

uint8_t PackManager_GetExtModId(uint8_t index);
uint16_t PackManager_GetExtBal(uint8_t index);

#endif