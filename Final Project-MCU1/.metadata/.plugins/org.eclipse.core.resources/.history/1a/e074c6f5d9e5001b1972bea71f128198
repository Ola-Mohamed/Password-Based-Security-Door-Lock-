/**********************************************************************************
 * [FILE NAME]: UART.c
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The UART source file for the CONTROL_ECU
 ***********************************************************************************/
#include "UART.h"

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)

#ifdef INTERRUPTS_send
ISR(USART_UDRE_vect){

	UDR= g_UART_dataToSend;
}
#endif

#ifdef INTERRUPTS_recieve
ISR(USART_RXC_vect){

	g_UART_dataRecieved= UDR;
}
#endif
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void UART_init(const UART_config * Config_Ptr)
{
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB |= (1<<RXEN) | (1<<TXEN);

#ifdef INTERRUPTS_recieve
	UCSRB = ( UCSRB & 0x7F )| ( 1<< RXCIE);
#endif
#ifdef INTERRUPTS_send
	UCSRB = ( UCSRB & 0xDF )| ( 1<< UDRIE);
#endif

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation , 1 Synchronous Operation
	 * UPM1:0  = 00 Disable parity bit , 10 Even Parity , 11 Odd Parity
	 * USBS    = 0 One stop bit , 1 Two stop bit
	 * UCSZ1:0 =  00 For 5-bit data mode ,01 For 6-bit data mode , 10 For 7-bit data mode,
	 *             11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	SET_BIT(UCSRC,URSEL);

	UCSRC = ( UCSRC & 0xBF ) | ( (Config_Ptr->mode) << 6);

	UCSRC = ( UCSRC & 0xCF ) | ( (Config_Ptr->parity) << 4);

	UCSRC = ( UCSRC & 0xF7 ) | ( (Config_Ptr->stop) << 3);

	UCSRC = ( UCSRC & 0xF9 ) | ( (Config_Ptr->size) << 1);

	UCSRC = ( UCSRC & 0xFE ) | ( Config_Ptr->mode );

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;
}

void UART_sendByte(const uint8 data)

{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
#ifndef INTERRUPTS_send
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
		UDR = data;
		while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
		SET_BIT(UCSRA,TXC); // Clear the TXC flag
	 *******************************************************************/
#endif
#ifdef INTERRUPTS_send
 g_UART_dataToSend=data;
#endif
}


uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
#ifndef INTERRUPTS_recieve
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data automatically */
	return UDR;
#else

	return g_UART_dataRecieved;

#endif
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	 *******************************************************************/
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
