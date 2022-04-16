/**********************************************************************************
 * [FILE NAME]: UART.h
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The UART header file for the CONTROL_ECU
 ***********************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
/*******************************************************************************
 *                          Globale Variables                                  *
 *******************************************************************************/
static volatile uint8 g_UART_dataRecieved;
static volatile uint8 g_UART_dataToSend;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* UART Driver Baud Rate */
#define USART_BAUDRATE 9600

#define INTERRUPTS_recieve
#undef INTERRUPTS_recieve
#define INTERRUPTS_send
#undef INTERRUPTS_send

/*******************************************************************************
 *                             Configurations                                  *
 *******************************************************************************/

typedef enum
{
	ASYNCHRONOUS,SYNCHRONOUS
}UART_mode;

typedef enum
{
	DISABLED,ENABLED_EVEN=2,ENABLED_ODD
}UART_parityBit;

typedef enum
{
	ONE,TWO
}UART_stopBit;

typedef enum
{
	FIVE,SIX,SEVEN,EIGHT
}UART_characterSize;


typedef struct{
	UART_mode mode;
	UART_parityBit parity;
	UART_stopBit stop;
	UART_characterSize size;
}UART_config;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_config * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
