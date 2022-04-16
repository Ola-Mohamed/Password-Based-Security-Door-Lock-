 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/


typedef enum
{
	EVEN,ODD,DISABLED
}Uart_ParityType;

typedef enum
{
	ONE_STOP_BIT,TWO_STOP_BIT
}Uart_StopBitsNum;


typedef struct
{
	 uint8 NumOfDataBits;
	 uint16 BaudRate;
	 Uart_ParityType ParityType;
	 Uart_StopBitsNum StopBitsNum;

}Uart_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const Uart_ConfigType * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
