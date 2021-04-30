 /******************************************************************************
 *
 * Module: timer1
 *
 * File Name: timer1.c
 *
 * Description: timer1 src file
 *
 * Author: Ahmed Mohamed
 *
 *******************************************************************************/
/*********************************************************************************
 * 									INCLUDES									 *
 *********************************************************************************/
#include"timer1.h"
/*********************************************************************************
 * 								Global variables								 *
 *********************************************************************************/

static void (*volatile g_T1CompareInterruptFunc_ptr)(void) = NULL;

static void (*volatile g_T1OverflowInterruptFunc_ptr)(void) = NULL;

volatile uint8 g_t1tick = 0;

/*********************************************************************************
 * 								Function Definitions							 *
 *********************************************************************************/

void TIMER1_setCallBackCompareMode(void (*a_ptr)(void))
{
	g_T1CompareInterruptFunc_ptr = a_ptr;
}

void TIMER1_setCallBackOverflowMode(void (*a_ptr)(void))
{
	g_T1OverflowInterruptFunc_ptr = a_ptr;
}

void TIMER1_init(const TIMER1_configType * config_ptr)
{
	/* setting the mode -> ctc , normal and setting FOC1A,B by zero as no pwm option here */
	TCCR1A = (1 << FOC1A) | (1 << FOC1B) | (config_ptr->mode & 0xFF);

	/* setting the mode -> ctc , normal */
	TCCR1B = (config_ptr->mode >> 8);

	/* setting the initial values selected in the compare reg and counting reg (first of all)*/
	TCNT1 = config_ptr->initial_value;

	/*here we use OCR1A only for comparison , we don't need the second OCR1B */
	OCR1A = config_ptr->compare_value;

	/* setting the interrupt modes */
	TIMSK |= (config_ptr->overflow_interrupt << TOIE1) | ((config_ptr ->compare_interrupt)<<OCIE1A);

	/* starting the timer NOW after all settings is done*/
	TCCR1B |=config_ptr ->clock;

	/* enabling the global interrupt */
	GLOBAL_INTERRUPT_ENABLE();
}

void TIMER1_start(TIMER1_clock clk)
{
	/* setting the prescaler thus enabling the clock then initializing the counter */
	TCCR1B |= clk;
	TCNT1 = 0;
}


void TIMER1_setCompareValue(uint16 value)
{
	OCR1A = value;
}

void TIMER1_stop(void)
{
	CLEAR_BIT(TCCR1B , 0);
	CLEAR_BIT(TCCR1B , 1);
	CLEAR_BIT(TCCR1B , 2);
}


/*********************************************************************************
 * 								interrupt service routines						 *
 *********************************************************************************/

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	/* the corresponding interrupt service routine function (IF EXIST) will be executed*/
	if(g_T1CompareInterruptFunc_ptr != NULL)
	{
		g_T1CompareInterruptFunc_ptr();
	}
}


ISR(TIMER1_OVF_vect)
{
	/* the corresponding interrupt service routine function (IF EXIST) will be executed*/
	if(g_T1OverflowInterruptFunc_ptr != NULL)
	{
		g_T1OverflowInterruptFunc_ptr();
	}
}
