 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_


/*******************************************************************************
 *                                   INCLUDES                                  *
 *******************************************************************************/

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* UART Driver Baud Rate */

/*******************************************************************************
 *                         	    TYPES DECLARATION                              *
 *******************************************************************************/

typedef enum
{
	DISABLED,RESERVED,EVEN,ODD
}UART_ParityMode;

typedef enum
{
	BIT1,BIT2
}UART_StopBit;

typedef enum
{
	BIT5,BIT6,BIT7,BIT8,Reserved1,Reserved2,Reserved3,BIT9
}UART_CharacterSize;

typedef struct
{
	UART_ParityMode UParityMode;
	UART_StopBit UStopBit;
	UART_CharacterSize UCharacterSize;
	uint32 UBaudRate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_ConfigType * UConfigPtr);

void UART_sendByte(const uint16 data);

uint16 UART_receiveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
