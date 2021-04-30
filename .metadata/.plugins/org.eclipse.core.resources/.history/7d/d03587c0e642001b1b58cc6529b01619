 /******************************************************************************
 *
 * Module: dc_motor
 *
 * File Name: dc_motor.h
 *
 * Description: dc motor header file
 *
 * Author: Ahmed Mohamed
 *
 *******************************************************************************/
#ifndef _TIMER1_H_
#define _TIMER1_H_
/*********************************************************************************
 * 									INCLUDES									 *
 *********************************************************************************/
#include"../COMPLEX DRIVERS/micro_config.h"
#include"../COMPLEX DRIVERS/common_macros.h"
#include"../COMPLEX DRIVERS/std_types.h"
/*********************************************************************************
* 								Types Declarations							 *
 *********************************************************************************/
extern volatile uint8 g_t1tick;
typedef enum
{
	TIMER1_NORMAL = 0x0000 ,
	TIMER1_CTC = 0x0800 ,
}TIMER1_mode;

typedef enum
{
	TIMER1_NON ,
	TIMER1_F_CPU ,
	TIMER1_F_CPU_8 ,
	TIMER1_F_CPU_64 ,
	TIMER1_F_CPU_256 ,
	TIMER1_F_CPU_1024 ,
	TIMER1_PIN_FALLING_EDGE ,
	TIMER1_PIN_RAISING_EDGE ,
}TIMER1_clock;
/**************************************************
	 * [name] : TIMER1_configType
	 * [Type] : Structure
	 * [Function] : TIMER1 Module Dynamic configuration
	 * [Members] :
	 * 			mode TIMER1_NORMAL or TIMER1_CTC (16bit only so it's not a conig for me)
	 * 			output_mode TIMER1_NORMAL_OUTPUT or TIMER1_TOGGLE_OUTPUT etc..
	 * 			clock TIMER0_NON , F_CPU , 8 , 64 , 256 , 1024
	 * 			compare_interrupt enable or disable
	 * 			overflow_interrupt enable or disable
	 * 			compare_value 0 -> 65535
	 * 			initial_value 0 -> 65535
	 ***************************************************/
typedef struct
{
	TIMER1_mode mode;
	TIMER1_clock clock;
	uint8 compare_interrupt;
	uint8 overflow_interrupt;
	uint16 compare_value;
	uint16 initial_value;
}TIMER1_configType;

/*********************************************************************************
* 								Function Declarations							 *
 *********************************************************************************/
void TIMER1_init(const TIMER1_configType * config_ptr);
/**********************************************************
 * [FUNCTION NAME] : TIMER1_init
 * [FUNCTION TYPE] : void
 * [TASK] :
 * 		1.setting the FOC1A,B bits due to no pwm mode
 * 		2.setting the mode in the TCCR1A,B registers
 * 		3.setting the init values in the TCNT1 and OCR1A compare register
 * 		4.setting both the OVF interrupt and COM interrupt according to the config struct
 * 		5.starting the timer with the predefined prescaler in the config struct
 * 		6.enabling the global interrupt
 * [ARGS] :
 *	pointer to the configuration structure :
 *		indicating a constant configurations from the user in the main application
 ***********************************************************/
void TIMER1_setCompareValue(uint16 value);
/**********************************************************
 * [FUNCTION NAME] : TIMER1_setCompareValue
 * [FUNCTION TYPE] : void
 * [TASK] : setting a new compare value in the OCR1A
 * [ARGS] :
 * 	an unsigned character value :
 * 		indicating the compare value to be set in the OCR1A
 ***********************************************************/

void TIMER1_start(TIMER1_clock clk);
/**********************************************************
 * [FUNCTION NAME] : TIMER1_start
 * [FUNCTION TYPE] : void
 * [TASK] : starting the timer with a certain clock (PRESCALER) and initiating the TCNT1
 * [ARGS] :
 * 	unsigned character value :
 * 		indicating the value to be set in the TCCR1B register to set the selected prescaler
 ***********************************************************/
void TIMER1_stop(void);
/**********************************************************
 * [FUNCTION NAME] : TIMER1_stop
 * [FUNCTION TYPE] : void
 * [TASK] : settingg the prescaler of the timer clock as NON
 * 			thus No clock provided(STOPPING)
 ***********************************************************/
void TIMER1_setCallBackCompareMode(void (*a_ptr)(void));
/**********************************************************
 * [FUNCTION NAME] : TIMER1_setCallBackCompareMode
 * [FUNCTION TYPE] : void
 * [TASK] : setting the code of the compare match interrupt service routine (ISR)
 * [ARGS] :
 * 	pointer to void function which takes void arguments :
 * 		indicating the code that will be executed in the ISR of the compare match
 ***********************************************************/
void TIMER1_setCallBackOverflowMode(void (*a_ptr)(void));
/**********************************************************
 * [FUNCTION NAME] : TIMER1_setCallBackOverflowMode
 * [FUNCTION TYPE] : void
 * [TASK] : setting the code of the overflow interrupt service routine (ISR)
 * [ARGS] :
 * 	pointer to void function which takes void arguments :
 * 		indicating the code that will be executed in the ISR of the overflow
 ***********************************************************/
#endif /* _TIMER1_H_ */
