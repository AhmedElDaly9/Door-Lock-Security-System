/******************************************************************************
 *
 * Module: Timer 1
 *
 * File Name: timer1.c
 *
 * Description: Source file for timer1.
 *
 * Author: Ahmed El Daly
 *
 *******************************************************************************/

#include "timer1.h"


static volatile void (*g_Timer1_Call_Back_Ptr)(void);

ISR(TIMER1_COMPA_vect)
{
	(*g_Timer1_Call_Back_Ptr)();
}

/******************************************************************************
 *
 * Function Name: timer1_init
 *
 * Function Prototype: void timer1_init(const Timer1_ConfigType *timer1_config_ptr)
 *
 * Description: Initializes timer1, sets the pre-scaler required for the clock,
 * sets the starting value and the compare value, and the timer mode
 *
 * Arguments:
 *
 * const Timer1_ConfigType *timer1_config_ptr:  Pointer of type Timer1_ConfigType
 * which contains the configuration of timer1.
 *
 *******************************************************************************/

void timer1_init(const Timer1_ConfigType *timer1_config_ptr)
{
	/* Select the Clock*/
	TCCR1B = (TCCR1B & 0xF8) | ((timer1_config_ptr -> T1_clk) & 0x07);


	TCNT1= timer1_config_ptr-> T1_startValue;

	if(timer1_config_ptr->T1_mode == TIMER1_OCB)
	{
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TIMSK, OCIE1B);
		TCCR1A = (TCCR1A & 0x0F) | (((timer1_config_ptr -> T1_cmp_mode) & 0xF0) << 4);
		OCR1B= timer1_config_ptr-> T1_compareValue;
		SET_BIT(TCCR1A, FOC1B);
	}
	else if ((timer1_config_ptr->T1_mode) == TIMER1_OCA)
	{
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TIMSK, OCIE1A);
		TCCR1A = (TCCR1A & 0x0F) | ((timer1_config_ptr -> T1_cmp_mode) << 6);
		OCR1A= timer1_config_ptr-> T1_compareValue;
		SET_BIT(TCCR1A,FOC1A);
	}
	else if ((timer1_config_ptr->T1_mode) == TIMER1_OVF)
	{
		SET_BIT(TCCR1A,FOC1A);
		SET_BIT(TCCR1A,FOC1B);
		SET_BIT(TIMSK, TOIE1);
	}

}

/******************************************************************************
 *
 * Function Name: timerONSeconds
 *
 * Function Prototype: void timerONSeconds(uint8 seconds)
 *
 * Description: Configures that timer in Normal mode in channel A
 * and sets the clock to F_CPU/1024 and takes the number of seconds the timer will
 * tick before triggering an interrupt.
 *
 * Arguments:
 *
 * seconds: the number of seconds the timer will tick before triggering an interrupt
 *
 *******************************************************************************/
void timerONSeconds(uint8 seconds)
{
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCA,0, 7812*seconds};
	timer1_init(&timer1_config);
}


/******************************************************************************
 *
 * Function Name: Timer1_setCallBack
 *
 * Function Prototype: void Timer1_setCallBack(void(*Ptr2Func)(void))
 *
 * Description: Sets call back to the function that the timer will return to after
 * interrupt.
 *
 * Arguments:
 *
 * void(*Ptr2Func)(void): a pointer to the function to be called after interrupt.
 *
 *******************************************************************************/
void Timer1_setCallBack(void(*Ptr2Func)(void))
{
	g_Timer1_Call_Back_Ptr = Ptr2Func;
}


/******************************************************************************
 *
 * Function Name: Timer1_Stop
 *
 * Function Prototype: void Timer1_Stop(void);
 *
 * Description: Sets the clock of the timer to NO Clock which stops the timer
 * from counting
 *
 *
 *******************************************************************************/
void Timer1_Stop(void)
{
	Timer1_ConfigType timer1_config = {NO_CLOCK, COM1_NORMAL_MODE, TIMER1_OCA,0, 0};
	timer1_init(&timer1_config);
}
