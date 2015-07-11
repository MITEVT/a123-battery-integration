/**
 * @file pack_manager.h
 * @author Eric Ponce
 * @date 11 July 2015
 * @brief Pack State Staging Mananger
 */

#ifndef _PACK_MANAGER_H_
#define _PACK_MANAGER_H_

#include "config.h"
#include "state_types.h"
#include "a123mbb.h"

/**
 * @brief Initialize the Pack State Manager
 */
void PackManager_Init(void);
/**
 * @brief Update the Pack State Manager with the latest a123 message
 * @details This function stages the incoming data so that when PackManager_Commit is
 * called the latest data is stuffed into pack state
 * 
 * @param msg Latest A123 message
 * @return 0 for OK, -1 for error
 */
int8_t PackManager_Update(CCAN_MSG_OBJ_T *msg);
/**
 * @brief Commit the staged data to a PACK_STATE_T struct. Currently only updates the min and max voltages
 * 
 * @param pack_state struct for storing staged data
 */
void PackManager_Commit(PACK_STATE_T *pack_state);
/**
 * @brief Get the ID of the module at the specified extended messsage list index
 * 
 * @param index Index in extended message list
 * @return module ID, unless not found (0xFE)
 */
uint8_t PackManager_GetExtModId(uint8_t index);
/**
 * @brief Get the balance information at specified extended message list index
 * 
 * @param index Index in extended message list
 * @return balance information, unless not found (0x00)
 */
uint16_t PackManager_GetExtBal(uint8_t index);

#endif