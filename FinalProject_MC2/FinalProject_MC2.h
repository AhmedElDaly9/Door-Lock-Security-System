/*
 * FinalProject_MC2.h
 *
 * Created on: 16/12?/2020
 *      Author: Ahmed El Daly
 */

#ifndef FINALPROJECT_MC2_H_
#define FINALPROJECT_MC2_H_

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define MC1_Ready 0x10
#define MC2_Ready 0x01
#define BUZZER 0x55
#define OK 0xAA
#define EEPROM_SA 0x0A
#define RecBefore 0x11

/*******************************************************************************
 *                                   INCLUDES                                  *
 *******************************************************************************/

#include "timer1.h"
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "external_eeprom.h"
#include "i2c.h"
#include "uart.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void OpenDoorMC2 (void);
void ClosingDoor (void);
void StopDoor (void);
void BUZZER_ON_MC2(uint8 seconds);
void BUZZER_OFF_MC2(void);
void StopDoor (void);

#endif /* FINALPROJECT_MC2_H_ */
