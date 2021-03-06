 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "uart.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 flag_bit9 =0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void UART_init(const UART_ConfigType * UConfigPtr)
{
	uint32 BaudRate;
	uint8 BaudPreScaler;
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * TXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	if (UConfigPtr->UCharacterSize == 7)
	{
		UCSRB |= (1<<UCSZ2);
		flag_bit9 = 1;
	}

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	UCSRC |= (1<<URSEL);
	UCSRC |= ( UConfigPtr->UParityMode << UPM0 );
	UCSRC |= ( UConfigPtr->UStopBit <<USBS);
	
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	BaudRate = UConfigPtr->UBaudRate;
	BaudPreScaler = ((F_CPU / (BaudRate * 8UL)) - 1);
	UBRRH = BaudPreScaler>>8;
	UBRRL = BaudPreScaler;
}
	
void UART_sendByte(const uint16 data)
{
	uint8 bit9;
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for 
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	if (flag_bit9==1)
	{
		bit9 = data & (1<<8);
		UCSRB = (UCSRB & 0xFE) | bit9;
	}
	/* Put the required data in the UDR register and it also clear the UDRE flag as 
	 * the UDR register is not empty now */	 
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/	
}

uint16 UART_receiveByte(void)
{
	uint8 bit9;
	/* RXC flag is set when the UART receive data so wait until this 
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	bit9 = UCSRB & 0x01;

	/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
    return (UDR | (bit9<<8));
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
	Str[i] = UART_receiveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}
	Str[i] = '\0';
}
