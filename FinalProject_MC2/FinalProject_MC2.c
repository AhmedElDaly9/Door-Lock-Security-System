/*
 * FinalProject_MC2.c
 *
 * Created on: 16/12þ/2020
 *      Author: Ahmed El Daly
 */

#include "FinalProject_MC2.h"

/*******************************************************************************
 *                              Variables Declaration                          *
 *******************************************************************************/
/*Variable used as a counter*/
volatile uint8 i;

/*Variable to decide wether there is a password recorded or not*/
volatile uint8 PassRecorded;

/*Variable to decide wether to open the door or turn on the buzzer*/
volatile uint8 HashDecide;

/*Variable to carry the user choice received from MC1 by UART*/
volatile uint8 choice;

/*String to store the 5-digits password received from user or eeprom*/
uint8 PassMC2[6];

int main (void)
{
	/*I2C initialization*/
	TWI_ConfigType TWI_Config = {FCPU_CLOCK,0x02,0x01};

	/*UART initialization*/
	UART_ConfigType UART_Config = {DISABLED,BIT1,BIT8,9600};

	EEPROM_init(&TWI_Config);
	UART_init(&UART_Config);

	//EEPROM_readByte(EEPROM_SA,&PassRecorded);

	while (1)
	{
		while(PassRecorded != RecBefore)
		{
			while (UART_receiveByte() != MC1_Ready) {}
			UART_sendByte(0);

			UART_sendByte(MC2_Ready);
			UART_receiveString(PassMC2);


			EEPROM_writeByte(EEPROM_SA,RecBefore);

			for (i=0; i<5 ; i++)
			{
				EEPROM_writeByte(EEPROM_SA+(i+1),PassMC2[i]);
			}
			PassRecorded=RecBefore;
			EEPROM_writeByte(EEPROM_SA,PassRecorded);
		}

		for (i=0; i<5 ;i++)
		{
			EEPROM_readByte(EEPROM_SA+(i+1),&PassMC2[i]);
		}


		while (UART_receiveByte() != MC1_Ready) {}
		UART_sendByte(1);

		UART_sendByte(MC2_Ready);
		choice = UART_receiveByte();

		if (choice == '*')
		{
			PassMC2[5]='#';

			while (UART_receiveByte() != MC1_Ready) {}
			UART_sendString(PassMC2);

			UART_sendByte(MC2_Ready);
			HashDecide = UART_receiveByte();

			if (HashDecide == OK)
			{
				UART_sendByte(MC2_Ready);
				UART_receiveString(PassMC2);

				for (i=0; i<5 ; i++)
				{
					EEPROM_writeByte(EEPROM_SA+1+i,PassMC2[i]);
				}
			}
			else
			{
				BUZZER_ON_MC2(10);
			}
		}
		else if (choice == '#')
		{
			PassMC2[5]='#';

			while (UART_receiveByte() != MC1_Ready) {}
			UART_sendString(PassMC2);

			UART_sendByte(MC2_Ready);
			HashDecide=UART_receiveByte();

			if (HashDecide == OK)
			{
				SET_BIT(SREG,7);
				OpenDoorMC2();
			}
			else
			{
				SET_BIT(SREG,7);
				BUZZER_ON_MC2(60);
			}
		}
	}

}


void BUZZER_ON_MC2(uint8 seconds)
{
	DDRA |= (1<<PA0);
	PORTA |= (1<<PA0);
	Timer1_setCallBack(BUZZER_OFF_MC2);
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCA,0, 7812*seconds};
	timer1_init(&timer1_config);
}

void BUZZER_OFF_MC2(void)
{
	PORTA &= (~(1<<PA0));
	Timer1_setCallBack(Timer1_Stop);
}


void OpenDoorMC2 (void)
{
	/* configure pin PC0 and PC1 as output pins */
	DDRD |= 0xC0;

	// Rotate the motor --> clock wise
	CLEAR_BIT(PORTD,PD6);
	SET_BIT(PORTD,PD7);
	Timer1_setCallBack(StopDoor);
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCA,0, 7812*15};
	timer1_init(&timer1_config);
}

void ClosingDoor (void)
{
	SET_BIT(PORTD,PD6);
	CLEAR_BIT(PORTD,PD7);
	Timer1_setCallBack(Timer1_Stop);
}

void StopDoor (void)
{
	CLEAR_BIT(PORTD,PD7);
	CLEAR_BIT(PORTD,PD7);
	Timer1_setCallBack(ClosingDoor);
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCA,0, 7812*3};
	timer1_init(&timer1_config);
}
