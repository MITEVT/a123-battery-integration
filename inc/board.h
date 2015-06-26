/**
 * @file board.h
 * @author Eric Ponce
 * @date 25 June 2015
 * @brief Board peripheral driver
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "chip.h"
#include "state_types.h"
#include "mcp2515.h"
#include "brusa.h"
#include "a123mbb.h"
#include "util.h"
#include "charge.h"
#include "drain.h"
#include "ssm.h"

#define LED1_GPIO 2
#define LED1_PIN 10

#define LED2_GPIO 3
#define LED2_PIN 0

#define LED3_GPIO 3
#define LED3_PIN 1

#define LED4_GPIO 3
#define LED4_PIN 2

#define LED5_GPIO 3
#define LED5_PIN 3

#define SWITCH_GPIO 0
#define SWITCH_PIN  1

#define CONTACTOR_P_GPIO 	2
#define CONTACTOR_P_PIN  	8
#define CONTACTOR_N_GPIO 	2
#define CONTACTOR_N_PIN  	7
#define CONTACTOR_PRE_GPIO 	2
#define CONTACTOR_PRE_PIN 	1

#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
	#define DEBUG_Print(str) Chip_UART_SendBlocking(LPC_USART, str, strlen(str))
	#define DEBUG_Println(str) {DEBUG_Print(str); DEBUG_Print("\r\n");}
	#define DEBUG_Write(str, count) Chip_UART_SendBlocking(LPC_USART, str, count)
#else
	#define DEBUG_Print(str)
	#define DEBUG_Println(str)
	#define DEBUG_Write(str, count) 
#endif


/**
 * @details Initialize the UART used for debugging
 * 
 * @param baudRateHz the desired baud rate
 */
void Board_UART_Init(uint32_t baudRateHz);

/**
 * @details Initialize the SPI0 peripheral used in off-chip CAN
 * 
 * @param baudRateHz the desired baud rate
 */
void Board_SPI_Init(uint32_t baudRateHz);

/**
 * @details Initialize the on-chip CAN peripheral
 * 
 * @param baudRateHz the desired baud rate
 * @param CAN_rx CAN RX callback function
 * @param CAN_tx CAN TX callback function
 * @param CAN_error CAN error callback function
 */
void Board_CCAN_Init(uint32_t baudRateHz, void (*CAN_rx)(uint8_t), void (*CAN_tx)(uint8_t), void (*CAN_error)(uint32_t));

/**
 * @details Initialize Board Status LED
 */
void Board_LED_Init(void);

/**
 * @details Turn status LED on
 */
void Board_LED_On(void);

/**
 * @details Turn status LED off
 */
void Board_LED_Off(void);

/**
 * @details Initialize board input switch
 */
void Board_Switch_Init(void);

/**
 * @details Get board input switch state
 * @return state of switch
 */
bool Board_Switch_Read(void);

/**
 * @details Initialize contactors
 */
void Board_Contactors_Init(void);

/**
 * @details Close or open the contactors
 * @param close Whether or no to close the contactors
 */
void Board_Close_Contactors(bool close);

/**
 * @details Get state of contactors
 * @return state of contactors
 */
bool Board_Contactors_Closed(void);

extern void CAN_rx(uint8_t msg_obj_num);
extern void CAN_tx(uint8_t msg_obj_num);
extern void CAN_error(uint32_t error_info);

#endif
