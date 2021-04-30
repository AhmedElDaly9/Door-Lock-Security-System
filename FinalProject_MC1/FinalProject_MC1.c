/*
 * FinalProject.c
 *
 *  Created on: 16/12þ/2020
 *      Author: Ahmed El Daly
 */


#include "FinalProject_MC1.h"


/*******************************************************************************
 *                              Variables Declaration                          *
 *******************************************************************************/

static volatile void (*g_Timer1_Call_Back_Ptr)(void);

/*Variable to receive key pressed on keypad*/
volatile uint8 keyPressed;

/*Variable to decide wether there is a password recorded or not*/
volatile uint8 PassRec;

/*Variable to record the user choice wether to change pass or open door*/
volatile uint8 keyOption=0;

/*String to store the 5-digits password received from user or eeprom*/
uint8 PassMC1[6];

/*flag to ensure that matching occurs between the two passwords*/
volatile uint8 flag=1;

/*two counters for loops*/
uint8 i;
uint8 j;


int main (void)
{
	/*LCD initialization*/
	LCD_init();

	/*UART initialization*/
	UART_ConfigType UART_Config = {DISABLED,BIT1,BIT8,9600};
	UART_init(&UART_Config);

	while(1)
	{
		UART_sendByte(MC1_Ready);
		PassRec=UART_receiveByte();

		if (PassRec==0)
		{
			EnterNewPassword();
		}
		else if (PassRec==1)
		{
			display_options();
		}
	}
}
void display_options (void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"*: Change pass");
	LCD_goToRowColumn(1,0);
	LCD_displayStringRowColumn(1,0,"#: Open door");

	/*keyOption received the button pressed on the keypad*/
	keyOption = KeyPad_getPressedKey();

	/*while loop for ignoring any inputs other than the * and # */
	while (keyOption != '*' && keyOption != '#')
	{
		keyOption = KeyPad_getPressedKey();
	}

	if (keyOption=='*')
	{
		_delay_ms(500);

		while (UART_receiveByte() != MC2_Ready) {}
		UART_sendByte(keyOption);

		ChangePassword();
	}

	else if (keyOption=='#')
	{
		_delay_ms(500);

		while (UART_receiveByte() != MC2_Ready) {}
		UART_sendByte(keyOption);

		CheckPassword();
	}
}

void ChangePassword(void)
{
	flag=1;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Current password:");
	LCD_goToRowColumn(1,0);

	UART_sendByte(MC1_Ready);
	UART_receiveString(PassMC1);
	PassMC1[5]='#';

	i=0;
	while (i<5)
	{
		 /* get the pressed key number */
		keyPressed = KeyPad_getPressedKey();
		if (PassMC1[i] != keyPressed + '0')
		{
			flag=0;
		}
		LCD_displayCharacter('*');
		_delay_ms(500); /* Press time */
		i++;
	}

	if (flag==1)
	{
		while (UART_receiveByte() != MC2_Ready) {}
		UART_sendByte(OK);

		EnterNewPassword();
	}

	else if (flag==0)
	{
		j=0;
		do {
			flag=1;
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(500);
			LCD_clearScreen();
			LCD_displayString("Current Password:");
			LCD_goToRowColumn(1,0);

			i=0;
			while (i<5)
			{
				keyPressed = KeyPad_getPressedKey(); /* get the pressed key number */
				if (PassMC1[i] != keyPressed+'0')
				{
					flag=0;
				}
				LCD_displayCharacter('*');
				_delay_ms(500); /* Press time */
				i++;
			}
			j++;
		}while (j<2 && flag==0);

		/*If the user enters 3 consecutive wrong passwords the buzzer rings*/
		if (flag==0)
		{
			BuzzerMC1();
		}
		else
		{
			while (UART_receiveByte() != MC2_Ready) {}
			UART_sendByte(OK);

			EnterNewPassword();
		}
	}
}

void EnterNewPassword(void)
{
	while (1)
	{
		flag=1;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Enter new pass:");
		LCD_goToRowColumn(1,0);

		i=0;
		while (i<5)
		{
			 /* get the pressed key number */
			keyPressed = KeyPad_getPressedKey();
			if (keyPressed >= 0 && keyPressed <=9)
			{
				PassMC1[i]= keyPressed + '0';
				LCD_displayCharacter('*');
				_delay_ms(500); /* Press time */
				i++;
			}
		}

		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Re-enter pass:");
		LCD_goToRowColumn(1,0);

		i=0;
		while (i<5)
		{
			keyPressed = KeyPad_getPressedKey(); /* get the pressed key number */

			if (keyPressed >= 0 && keyPressed <=9)
			{
				if (PassMC1[i] != (keyPressed+'0'))
				{
					flag=0;
				}
				LCD_displayCharacter('*');
				_delay_ms(500); /* Press time */
				i++;
			}
		}


		if (flag == 1)
		{
			/*Adding the last string character # for UART receiving and sending*/
			PassMC1[5]= '#';

			while(UART_receiveByte() != MC2_Ready){}
			UART_sendString(PassMC1);

			PassRec=1;

			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Pass Recorded");
			_delay_ms(1000);
			break;
		}
		else
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Not matched");
			_delay_ms(1000);
		}
	}
}

void CheckPassword (void)
{
	flag=1;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Current password:");
	LCD_goToRowColumn(1,0);

	UART_sendByte(MC1_Ready);
	UART_receiveString(PassMC1);
	PassMC1[5]='#';

	i=0;
	while (i<5)
	{
		keyPressed = KeyPad_getPressedKey(); /* get the pressed key number */
		if (PassMC1[i] != keyPressed + '0')
		{
			flag=0;
		}
		LCD_displayCharacter('*');
		_delay_ms(500); /* Press time */
		i++;
	}

	if (flag==1)
	{
		OpenDoorMC1();
	}
	else if (flag==0)
	{
		j=0;
		do {
			flag=1;
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(1000);
			LCD_clearScreen();
			LCD_displayString("Current Password:");
			LCD_goToRowColumn(1,0);

			i=0;
			while (i<5)
			{
				keyPressed = KeyPad_getPressedKey(); /* get the pressed key number */
				if (PassMC1[i] != keyPressed+'0')
				{
					flag=0;
				}
				LCD_displayCharacter('*');
				_delay_ms(500); /* Press time */
				i++;
			}
			j++;
		}while (j<2 && flag==0);

		if (flag==0)
		{
			BuzzerMC1();
		}
		else
		{
			OpenDoorMC1();
		}
	}
}

void OpenDoorMC1(void)
{
	while (UART_receiveByte() != MC2_Ready) {}
	UART_sendByte (OK);

	Timer1_setCallBack(StopDoorMC1);
	LCD_clearScreen();
	LCD_displayString ("Opening Door");
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCB,0, 7812*15};
	timer1_init(&timer1_config);
	while (g_Timer1_Call_Back_Ptr != ClosingDoorMC1){}
}

void StopDoorMC1(void)
{
	Timer1_setCallBack(ClosingDoorMC1);
	LCD_clearScreen();
	LCD_displayString ("Welcome");
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCB,0, 7812*3};
	timer1_init(&timer1_config);
	while (g_Timer1_Call_Back_Ptr != Timer1_Stop){}

}

void ClosingDoorMC1(void)
{
	Timer1_setCallBack(Timer1_Stop);
	LCD_clearScreen();
	LCD_displayString ("Closing Door");
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCB,0, 7812*15};
	timer1_init(&timer1_config);
	while ((TCCR1B)&0x03){}
}

void BuzzerMC1 (void)
{
	while (UART_receiveByte() != MC2_Ready) {}
	UART_sendByte (BUZZER);

	Timer1_setCallBack(Timer1_Stop);
	LCD_clearScreen();
	LCD_displayString ("BUZZER!!!");
	Timer1_ConfigType timer1_config = {F_CPU1024, COM1_NORMAL_MODE, TIMER1_OCB,0, 7812*60};
	timer1_init(&timer1_config);
	while ((TCCR1B)&0x03){}

}
