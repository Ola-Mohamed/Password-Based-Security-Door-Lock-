 /******************************************************************************
 *
 * Module: MCU2
 *
 * Description: the application is even getting password and send it to MC1  or recieve a  new password to save in EEPROM
	  or doing operation even with door motor or buzzer
 * File Name: MCU2.c
 *
 * Author: Ola Mohamed
 *
 * ******************************************************************************/

#include "MCU2.h"

int main(){
	/*
	 * 1.make the PC2 , PC3 and PC4 pins output pins for motor and buzzer
	 * 2.initialization of EEPROM
	 * 3.enabling global interrupt bit
	 * */

		DDRC |= (1<<PC2) | (1<<PC3) | (1<<PC4) ;

		UART_init(&UART_config) ;
		TIMER1_init(&TIMER1_config ) ;
		EEPROM_init() ;
		SET_BIT(SREG , 7) ;

		while(UART_recieveByte() != MC1_READY);
			UART_sendByte(MC2_READY) ;
			UART_receiveString(g_password) ;

			/* to save first sended password in the EEPROM*/
			savePassword() ;

	while(1){
		(*ptr_states[g_stateNum])();  /* calling functions from the array of functions */

	}
}
/*-------------------------Function Definitions---------------------------------*/

/*-------------------------------------------------------------------------------
[Function Name]: savePassword
[Description]: save the password to EEPROM
[Return]: Void
 -------------------------------------------------------------------------------*/
void savePassword(void)
{
	uint8 i ;

	/* save every character of password in separate location in EEPROM*/
	for(i = 0 ; i < 5 ; i++)
	{
		EEPROM_writeByte(EEPROM_ADDRESS+i , g_password[i]) ;
	}
}

/*-------------------------------------------------------------------------------
[Function Name]: getPassword
[Description]:  getting the current password from EEPROM
[Return]: void
 -------------------------------------------------------------------------------*/
void getPassword(void)
{
	uint8 i ;

	/* getting every character of password from separate locations from EEPROM*/
	for(i = 0 ; i < 6 ; i ++)
	{
		EEPROM_readByte(EEPROM_ADDRESS+i , &(g_password[i])) ;
	}
	/*  putting '#' for UART_recieveByte technique*/
	g_password[i] = '#' ;
}

/*-------------------------------------------------------------------------------
[Function Name]:  goToOperation
[Description]:  checks what is the operation requested from MC1
				then go to its function
[Args]:    a_operation : getting the operation from MC1 then passing it here

[Return]: void
 -------------------------------------------------------------------------------*/
void goToOperation(uint8 a_operation)
{
	/*
	 * 1.if operation is NEW_PASSWORD then go to savePassword function
	 * 2.two Micro-controllers tells to us that they are ready for synchronization between them
	 * 3.to save the new password
	 *  */
	if(a_operation == NEW_PASSWORD)
	{

		while(UART_recieveByte() != MC1_READY); // wait until MC1 is ready
    	UART_sendByte(MC2_READY) ;
    	UART_receiveString(g_password) ;
    	savePassword() ;
	}

	/*
	 * 1. if operation is BUZZER then buzzer alerting
	 * 2.turn the buzzer on
	 * 3.entering while loop for delaying till 1 minute passes then zero the flag from buzzerAlerting function
	 *4. turn the buzzer off after 1 minute
	 * */
	else if(a_operation == BUZZER)
	{

		SET_BIT(PORTC , PC4) ;
		Timer1_OVF_setCallBack(Alert) ;
	    g_delayFlag = 1 ;
	    while(g_delayFlag == 1) ;
		CLEAR_BIT(PORTC , PC4) ;
	}
	/*
	 * 1.if operation is DOOR then open the door and close it automatically
	 * 2. first turn the motor clockwise to open the door
	 * 3.entering while loop for delaying till door is opening and closing
	     * then zero the flag from buzzerTermination function
	     *  */
	else if(a_operation == DOOR)
	{

		PORTC &= ~(1<<PC2) ;
		PORTC |= (1<<PC3) ;
   		Timer1_OVF_setCallBack(doorOptions) ;
    	g_delayFlag = 1 ;
    	while(g_delayFlag == 1) ;
	}

}

/*-------------------------------------------------------------------------------
[Function Name]:doorActivities
[Description]: open the door in 15 seconds then hold for 5 seconds
				then close it in 15 seconds
[Return]: void
 -------------------------------------------------------------------------------*/
void doorOtions(void)
{
  /*1. No. of overflows of TIMER1
   * 2.AVR maybe clear this bit after interrupt then open it manually again
   * 3.every tick (overflow) takes 0.5 second then at first 15 seconds door is opening
   * 4.the door holds for 3 seconds
   * 5.the last 15 seconds door is closing
   */
	g_ticks++ ;
	SET_BIT(SREG , 7) ;

	if(g_ticks == 30)
	{
		PORTC &= ~(1<<PC2) & ~(1<<PC3) ;
	}

	else if(g_ticks == 36)
	{
		PORTC &= ~(1<<PC3) ;
		PORTC |= (1<<PC2) ;
	}

	else if(g_ticks == 66)
	{
		PORTC &= ~(1<<PC2) & ~(1<<PC3) ;

		g_delayFlag = 0 ;
		g_ticks = 0 ;
		Timer1_DeInit();
	}

}

/*-------------------------------------------------------------------------------
[Function Name]: Alert
[Description]: calculate 1 minute then turn off the buzzer

[Return]: void
 -------------------------------------------------------------------------------*/
void Alert(void)
{

	/*
	 * 1. No. of overflows of TIMER1
	 * 2.AVR maybe clear this bit after interrupt then open it manually again
	 * 3.every tick (overflow) takes 0.5 sec then it is 1 minute
	 * 4.turn off the buzzer
	 * 5.zero the delay to exit from the while loop of delaying
	 */
	g_ticks++ ;
	SET_BIT(SREG , 7) ;

	if(g_ticks >=120)
	{
		CLEAR_BIT(PORTC , PC4) ;
		g_delayFlag = 0 ;

		g_ticks = 0 ;
		Timer1_DeInit();

	}

}
void mainFunction(void){

	getPassword() ;

	    	/*
	    	 * 1.two Micro-controllers tells to us that they are ready
	    	 * for synchronization between them
	    	 * 2.wait until MC1 is ready
	    	 * */
	    	UART_sendByte(MC2_READY) ;
	    	while(UART_recieveByte() != MC1_READY);
	    	UART_sendString(g_password) ;

	    	while(UART_recieveByte() != MC1_READY);
	    	UART_sendByte(MC2_READY) ;
	    	operation = UART_recieveByte() ;

	    	/* send the operation to function then checks
	    	 * what is the operation requested from MC1
	    					then go to its function */
	    	goToOperation(operation) ;

}
