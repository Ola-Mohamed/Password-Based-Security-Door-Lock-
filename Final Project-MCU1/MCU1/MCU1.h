 /******************************************************************************
 *
 * Module: MCU1
 *
 * File Name:MCU1.h
 *
 * Author: Ola Mohamed
 *
 * ******************************************************************************/

#ifndef MCU1_H_
#define MCU1_H_
/************************************************************************************************
 *                                       DRIVERS                                         *
 ************************************************************************************************/
#include"keypad.h"
#include"lcd.h"
#include "timer1.h"
#include"common_macros.h"
#include"std_types.h"
#include"micro_config.h"
#include "uart.h"
/************************************************************************************************
 *                                       COMMANDS
 ***************************************************************************************/
#define MC1_READY     0XFE
#define MC2_READY    0XFF
#define BUZZER       0XFD
#define DOOR         0XFC
#define NEW_PASSWORD 0XFB
#define GLOBALE_ARRAY   4

/************************************************************************************************
 *                                GLOBAL VARIABLES                                              *
 ************************************************************************************************/
uint8 keyPressed ; /* holds the pressing of even '*' or '%'*/
const volatile uint8 g_password[6] ; /* array to save password of 5 characters */
static  uint8 g_delayFlag = 0 ; /* flag for delaying the system if there is a thief */
static  uint8 g_ticks = 0 ; /* to count No. of overflows of TIMER1 then certain time*/
static volatile uint8  g_stateNum;            /*global state number of the main function */
uint8 operation ; /*to store the operation getted from MC1 */



/************************************************************************************************
 *                                 UART & TIMER CONFIGURATIONS                                  *
 ************************************************************************************************/
/*initialization of configure UART driver by :
 * 1. 8 bits data
 * 2. baud rate of 9600
 * 3. disable parity bit
 * 4. one stop bit
 * then send structure of configuration to the driver*/
Uart_ConfigType UART_config = {8 , 9600 ,DISABLED , ONE_STOP_BIT };

/*configuration of TIMER1
	    /* 1. overflow mode
	     * 2. initial value = 0
	     * 3. compare A value = 0 (unused)
	     * 4. compare B value = 0 (unused)
	     * 5. Duty Cycle value = 0 (unused)
	     * 6. Pre-scalar = 64 */
TIMER1_ConfigType TIMER1_config = { OVERFLOW , 0 , 0  , 0 ,0 , F_CPU_64} ;
/************************************************************************************************
 *                                   FUNCTIONS                                                  *
 ************************************************************************************************/
void createpassword(void);

void sendPasswordToMC2(void);

void mainfunction(uint8 key) ;

void Alert(void) ;

void doorOptions(void) ;

void displayOptions(void);

/************************************************************************************************
 *            ARRAY OF POINTER TO FUNCTIONS THAT TAKE VOID AS AN ARGUMENT AND RETURN VOID       *                                       *
 ************************************************************************************************/
void (*ptr_states[GLOBALE_ARRAY])(void)={ createpassword,mainfunction,doorOptions,displayOptions};




#endif /* MCU1_H_ */
