 /******************************************************************************
 *
 * Module: Timer 1
 *
 * File Name:timer1.c
 *
 * Author: Ola Mohamed
 *
 * ******************************************************************************/

#include "timer1.h"

static volatile void (*g_OVF_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_CMPA_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_CMPB_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*-------------------------------------------------------------------------------
[Function Name]:
[Description]:   Function to initialize the TIMER1 driver with configration
  	              1. Set the required clock.
  	              2. Initialize Timer1 Registers.
[Args]:
[in]:     Config_Ptr : pointer to structure of configuration
[out]:    NONE
[Return]: NONE
 --------------------------------------------------*/



 void TIMER1_init(const TIMER1_ConfigType * Config_Ptr)
{
	if(Config_Ptr -> mode == OVERFLOW)
	{
		TCNT1 = Config_Ptr -> initialValue ;
		/* Configure the timer control register
			 * 1. Non PWM mode FOC1A=1 & FOC1A=1
			 * 2. Normal Mode WGM10=0 & WGM11=0 & WGM12=0 & WGM13=0
			 * 3. Normal Mode COM1A/B1=0 & COM1A/B0=0
			 */
		/* Non PWM Mode */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B) ;

		/*clock insertion by the configurable structure */
		TCCR1B = (TCCR1B & 0X00) | ((Config_Ptr -> clock & 0X07)) ;
		TIMSK |= (1<<TOIE1) ;
	}
	else if(Config_Ptr -> mode == COMPARE_A)
	{
		TCNT1 = Config_Ptr->initialValue;
		OCR1A = Config_Ptr->compareValueA;
		/* Configure timer0 control register
			 * 1. Non PWM mode FOC1A=1 & FOC1B=1
			 * 2. CTC Mode WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
			 * 3. No need for OC0 so COM00=0 & COM01=0 , "we'll do so using PWM mode"
		/* Non PWM Mode with*/
		TCCR1A = (1<<FOC1A) | (1<<FOC1B)  ;

		/*clock insertion by the configurable structure */
		TCCR1B = (TCCR1B & 0X00) | ((Config_Ptr -> clock & 0X07)) | (1<<WGM12) ;
		TIMSK |= (1<<OCIE1A) ; // Enable Timer1 Compare Interrupt

	}
	else if(Config_Ptr -> mode == COMPARE_B)
	{
		TCNT1 = Config_Ptr->initialValue;
		OCR1B = Config_Ptr->compareValueB;
		/* Configure timer0 control register
			 * 1. Non PWM mode FOC1A=1 & FOC1B=1
			 * 2. CTC Mode WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
			 * 3. No need for OC0 so COM00=0 & COM01=0 , "we'll do so using PWM mode"
		/* Non PWM Mode with*/
		TCCR1A = (1<<FOC1A) | (1<<FOC1B)  ;

		/*clock insertion by the configurable structure */
		TCCR1B = (TCCR1B & 0X00) | ((Config_Ptr -> clock & 0X07)) | (1<<WGM12) ;
		TIMSK |= (1<<OCIE1B) ; // Enable Timer1 Compare Interrupt
	}
	else if(Config_Ptr -> mode == PWM)
	{
		TCNT1 = Config_Ptr->initialValue;
		OCR1A = Config_Ptr->dutyCycle;
		/* Configure timer control register
		 * 1. Fast PWM mode FOC1A=0 & FOC1B=0
		 * 2. Fast PWM Mode WGM10=1 & WGM11=0 & WGM12=1 & WGM13=1
		 * 3. Clear OC1A when match occurs (non inverted mode) COM1A0=0 & COM1A1=1
		 */
		/* PWM Mode with channelA toggling at comparing*/
		TCCR1A = (1<<WGM10) | (WGM11) | (1<<COM1A0) ;

		/*clock insertion by the configurable structure with Fast PWM Mode */
		TCCR1B = (TCCR1B & 0X00) | ((Config_Ptr -> clock & 0X07)) | (1<<WGM12) | (1<<WGM13) ;

		DDRB  = DDRB | (1<<PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

	}


}


void Timer1_OVF_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_OVF_callBackPtr = a_ptr;
}



void Timer1_CMPA_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_CMPA_callBackPtr = a_ptr;
}

void Timer1_CMPB_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_CMPB_callBackPtr = a_ptr;
}


void Timer1_DeInit(void)
{
	TCNT1=0;
	TCCR1A=0;
	TCCR1B=0;
	CLEAR_BIT(TIMSK,TOIE1);    // Disable Overflow INT
	CLEAR_BIT (TIMSK,OCIE1A);   // Disable Timer1 Compare Interrupt
	CLEAR_BIT (TIMSK,OCIE1B);   // Disable Timer1 Compare Interrupt
}




ISR(TIMER1_OVF_vect)
{
	if(g_OVF_callBackPtr != NULL_PTR)
	{
		(*g_OVF_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr_OVF(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_CMPA_callBackPtr != NULL_PTR)
	{
		(*g_CMPA_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr_CMP(); */
	}
}

ISR(TIMER1_COMPB_vect)
{
	if(g_CMPB_callBackPtr != NULL_PTR)
	{
		(*g_CMPB_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr_CMP(); */
	}
}
