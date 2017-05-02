//-----------------------------------------------------------------------------
// Copyright SECOM Telecom Co., LTD
//
// Target:         uPD78F0485
// Tool chain:     NEC PM+ v6.30
// Author:		   Gan zhiyong
// Date:		   2011-5-8
// Vervion:	       V1.1
//-----------------------------------------------------------------------------

#ifndef	_SYSTEM_H
#define _SYSTEM_H


//-----------------------------------------------------------------------------
// Struct define
//-----------------------------------------------------------------------------
struct RTCCounterValue
{
	unsigned char Sec;
	unsigned char Min;
	unsigned char Hour;
	unsigned char Day;
	unsigned char Week;
	unsigned char Month;
	unsigned char Year;
};


//-----------------------------------------------------------------------------
// Function prototyps
//-----------------------------------------------------------------------------
extern void System_Init(void);
extern void RTC_TimeGet(struct RTCCounterValue* CounterReadVal);
extern void RTC_TimeSet(struct RTCCounterValue* CounterWriteVal);
extern void LCD_DisplayOn(void);
extern void LCD_DisplayOff(void);
extern void RS485_SendByte(unsigned char DataByte);




#endif	//_SYSTEM_H

