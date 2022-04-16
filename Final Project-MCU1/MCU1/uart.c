 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "uart.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void UART_init(const Uart_ConfigType * Config_Ptr)
{
	uint16 BAUD_PRESCALE = (((F_CPU / (Config_Ptr->BaudRate * 8UL))) - 1);

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * TXEN  = 1 Transmitter Enable
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	if (Config_Ptr->NumOfDataBits == 6)
	{
		SET_BIT(UCSRC,UCSZ0);
	}
	else if (Config_Ptr->NumOfDataBits == 7)
	{
		SET_BIT(UCSRC,UCSZ1);
	}
	else if (Config_Ptr->NumOfDataBits == 8)
	{
		SET_BIT(UCSRC,UCSZ0);
		SET_BIT(UCSRC,UCSZ1);
	}
	else if (Config_Ptr->NumOfDataBits == 9)
	{
		SET_BIT(UCSRB,UCSZ2);
		SET_BIT(UCSRC,UCSZ1);
		SET_BIT(UCSRC,UCSZ0);
	}

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	SET_BIT(UCSRC,URSEL);

	if (Config_Ptr->ParityType == EVEN)
	{
		SET_BIT(UCSRC,UPM1);
	}
	else if (Config_Ptr->ParityType == ODD)
	{
		SET_BIT(UCSRC,UPM1);
		SET_BIT(UCSRC,UPM0);
	}
	else if (Config_Ptr->ParityType == DISABLED)
	{
		CLEAR_BIT(UCSRC,UPM1);
		CLEAR_BIT(UCSRC,UPM0);
	}


	if (Config_Ptr->StopBitsNum == TWO_STOP_BIT)
	{
		SET_BIT(UCSRC,USBS);
	}
	else if (Config_Ptr->StopBitsNum == ONE_STOP_BIT)
	{
		CLEAR_BIT(UCSRC,USBS);
	}


	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;
}

void UART_sendByte(const uint8 data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
    return UDR;
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}

}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
