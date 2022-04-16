 /******************************************************************************
 *
 * Module: Timer1
 *
 * File Name:Timer1.h
 *
 * Author: Ola Mohamed
 *
 * ******************************************************************************/

#ifndef TIMER1_CONFIG_H_
#define TIMER1_CONFIG_H_
/*********************************************************************************
 *                              INCLUDES
 *******************************************************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*********************************************************************************
 *                              ENUMS
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}TIMER1_Clock;

typedef enum
{
	OVERFLOW , COMPARE_A , COMPARE_B , PWM
}TIMER1_Mode;

/*********************************************************************************
 *                              STRUCTURES
 *******************************************************************************/
typedef struct
{
	TIMER1_Mode mode ;
	uint8 initialValue ;
	uint8 compareValueA;
	uint8 compareValueB;
	uint8 dutyCycle ;
	TIMER1_Clock clock ;
}TIMER1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3.  Initialize Timer1 Registers.
 */
void TIMER1_init(const TIMER1_ConfigType * Config_Ptr) ;

void TIMER1_OVF_setCallBack(void(*a_ptr)(void));

void TIMER1_CMPA_setCallBack(void(*a_ptr)(void));

void TIMER1_CMPB_setCallBack(void(*a_ptr)(void));

void TIMER1_DeInit(void);

#endif /* TIMER1_CONFIG_H_ */
