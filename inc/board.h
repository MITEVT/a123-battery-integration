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

#define LED1_GPIO 2
#define LED1_PIN 10

#define SWITCH_GPIO 0
#define SWITCH_PIN  1

#define CONTACTOR_P_GPIO 	2
#define CONTACTOR_P_PIN  	6
#define CONTACTOR_N_GPIO 	2
#define CONTACTOR_N_PIN  	7
#define CONTACTOR_PRE_GPIO 	2
#define CONTACTOR_PRE_PIN 	10

#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
	#define DEBUG_Print(str) Chip_UART_SendBlocking(LPC_USART, str, strlen(str))
	#define DEBUG_Write(str, count) Chip_UART_SendBlocking(LPC_USART, str, count)
#else
	#define DEBUG_Print(str)
	#define DEBUG_Write(str, count) 
#endif


void Board_UART_Init(uint32_t baudRateHz);

void Board_SPI_Init(uint32_t baudRateHz);

void Board_CCAN_Init(uint32_t baudRateHz, void (*CAN_rx)(uint8_t), void (*CAN_tx)(uint8_t), void (*CAN_error)(uint32_t));

void Board_LED_Init(void);
void Board_LED_On(void);
void Board_LED_Off(void);

void Board_Switch_Init(void);
bool Board_Switch_Read(void);

void Board_Contactors_Init(void);
bool Board_Contactors_On(void);
bool Board_Contactors_Off(void);

bool Board_Contactors_Closed(void);

extern void CAN_rx(uint8_t msg_obj_num);
extern void CAN_tx(uint8_t msg_obj_num);
extern void CAN_error(uint32_t error_info);

#endif
