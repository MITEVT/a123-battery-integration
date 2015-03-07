#include "chip.h"
#include <string.h>

#define LED1_GPIO 2
#define LED1_PIN 10

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
