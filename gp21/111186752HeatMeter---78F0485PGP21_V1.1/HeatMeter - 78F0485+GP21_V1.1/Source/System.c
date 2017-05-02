//-----------------------------------------------------------------------------
// Copyright SECOM Telecom Co., LTD
//
// Target:         uPD78F0485
// Tool chain:     NEC PM+ v6.30
// Author:		   Gan zhiyong
// Date:		   2011-5-8
// Vervion:	       V1.1
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// #pragma directive for CC78K0
//-----------------------------------------------------------------------------
#pragma sfr
#pragma nop


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "System.h"


//-----------------------------------------------------------------------------
// Function prototyps
//-----------------------------------------------------------------------------
void Clock_Init(void);
void TM00_Init(void);
void PORT_Init(void);
void KEY_Init(void);
void RTC_Init(void);
void LCD_Init(void);
void UART6_Init(void);
void UART6_Start(void);
void UART6_Stop(void);
void UART6_SendByte(unsigned char DataByte);
void RS485_Init(void);


//-----------------------------------------------------------------------------
// Local Macro definition
//-----------------------------------------------------------------------------
#define RS485_DIR			P1.4

#define RS485_DIR_RX		0
#define RS485_DIR_TX		1


//-----------------------------------------------------------------------------
// Module:   System_Init
// Description: System_Init
//-----------------------------------------------------------------------------
void System_Init(void)
{
	// Set memory and ram size
    IMS = 0xCF;
    IXS = 0x0A;
    
    // Initialize all modules
	Clock_Init();
	TM00_Init();
	PORT_Init();
	KEY_Init();
	RTC_Init();
	LCD_Init();
	RS485_Init();
}


//-----------------------------------------------------------------------------
// Module:   RTC_TimeGet
// Description: Reads the results of RTC and store them in the variables.
//-----------------------------------------------------------------------------
void RTC_TimeGet(struct RTCCounterValue* CounterReadVal)
{
	RTCC1 |= 0x01;	// RWAIT=1
	while((RTCC1 & 0x02) == 0);		// wait for RWST=1
	
	CounterReadVal->Sec = SEC;
	CounterReadVal->Min = MIN;
	CounterReadVal->Hour = HOUR;
	CounterReadVal->Week = WEEK;
	CounterReadVal->Day = DAY;
	CounterReadVal->Month = MONTH;
	CounterReadVal->Year = YEAR;
	
	RTCC1 &= ~0x01;	// RWAIT=0
	while((RTCC1 & 0x02) == 0x02);	// wait for RWST=0
}


//-----------------------------------------------------------------------------
// Module:   RTC_TimeSet
// Description: Changes the RTC value.
//-----------------------------------------------------------------------------
void RTC_TimeSet(struct RTCCounterValue* CounterWriteVal)
{
	RTCC1 |= 0x01;	// RWAIT=1
	while((RTCC1 & 0x02) == 0);		// wait for RWST=1
	
	SEC = CounterWriteVal->Sec;
	MIN = CounterWriteVal->Min;
	HOUR = CounterWriteVal->Hour;
	WEEK = CounterWriteVal->Week;
	DAY = CounterWriteVal->Day;
	MONTH = CounterWriteVal->Month;
	YEAR = CounterWriteVal->Year;
	
	RTCC1 &= ~0x01;	// RWAIT=0
	while((RTCC1 & 0x02) == 0x02);	// wait for RWST=0
}


//-----------------------------------------------------------------------------
// Module:   LCD_DisplayOn
// Description: LCD_DisplayOn
//-----------------------------------------------------------------------------
void LCD_DisplayOn(void)
{
	LCDMD |= 0x10;	// Internal resistance division method (no step-down transforming)
	SCOC = 1;		// Output deselect level to segment pin and LCD waveform to common pin
	LCDON = 1;		// Display on
}


//-----------------------------------------------------------------------------
// Module:   LCD_DisplayOff
// Description: LCD_DisplayOff
//-----------------------------------------------------------------------------
void LCD_DisplayOff(void)
{
	LCDON = 0;		// Display off
	LCDM &= ~0x50;	// Set both SCOC and VAON to 0
	LCDMD &= ~0x30;	// Internal resistor disconnected to save power
}


//-----------------------------------------------------------------------------
// Module:   RS485_SendByte
// Description: RS485_SendByte
//-----------------------------------------------------------------------------
void RS485_SendByte(unsigned char DataByte)
{
	RS485_DIR = RS485_DIR_TX;
	UART6_SendByte(DataByte);
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	RS485_DIR = RS485_DIR_RX;
}


//-----------------------------------------------------------------------------
// Module:   Clock_Init
// Description: Clock_Init
//-----------------------------------------------------------------------------
void Clock_Init(void)
{
	RSTOP = 0;		// Internal high-speed oscillator(fRH=8MHz) oscillating
	LSRSTOP = 1;	// Internal low-speed oscillator(fRL=240KHz) stopped
	OSCCTL = 0x10;	// X1, X2 as I/O port. XT1, XT2 oscillation mode
	MSTOP = 1;		// High-speed system clock(fXH) stopped
	MCM = 0;		// Main system clock(fXP) and Peripheral hardware clock(fPRS) all 
					// operate with Internal high-speed oscillation clock(fRH)
	PCC = 0x00;		// CPU clock(fCPU) = fXP (8MHz)
}


//-----------------------------------------------------------------------------
// Module:   TM00_Init
// Description: TM00_Init  interval timer
//-----------------------------------------------------------------------------
void TM00_Init(void)
{
	TMC00 = 0;		// operation stop
	TMMK000 = 1;	/* disable INTTM000 */
	TMIF000 = 0;	/* clear INTTM000 flag */
	TMMK010 = 1;	/* disable INTTM010 */
	TMIF010 = 0;	/* clear INTTM010 flag */
	TMPR000 = 1;	/* set INTTM000 low priority */
	/* TM00 interval timer */
	PRM00 = 0x03;	// TM00 internal clock source: fPRS/2^4
	CRC00 = 0x00;	// TM0_CR00_COMPARE operates as compare register
	CR000 = 0xC34F;	// 2^4/8MHz*0xC34F = 100ms
	
	TMMK000 = 0;	/* enable INTTM000 */
	TMC00 &= ~0x0C;	// stop TM00
	TMC00 |= 0x0C;	// Clear & start mode entered upon a match between TM00 and CR000
}


//-----------------------------------------------------------------------------
// Module:   PORT_Init
// Description: PORT_Init
//-----------------------------------------------------------------------------
void PORT_Init(void)
{
	P1 = 0x00;
	P2 = 0x00;
	P3 = 0x11;
	P4 = 0x00;
	
	PU1 = 0x80;		// GP2-INT set internal pull-up
	PU4 = 0x08;		// KEY set internal pull-up
	
	PM1 = 0xA0;
	PM2 = 0x00;
	PM3 = 0x02;
	PM4 = 0x08;
	
	PF1 = 0x40;		// P16 used as TxD6
	PFALL = 0x0F;	// P13,P14,P15 used as port. P08,P09,P10,P11 used as segment output
}


//-----------------------------------------------------------------------------
// Module:   KEY_Init
// Description: KEY_Init
//-----------------------------------------------------------------------------
void KEY_Init(void)
{
	KRMK = 1;		// Disable INTKR
	KRIF = 0;		// Clear INTKR flag
	KRPR = 1;		// Set INTKR low priority
	KRM = 0x08;		// Only KR3 used
	KRMK = 0;		// Enable INTKR
}


//-----------------------------------------------------------------------------
// Module:   RTC_Init
// Description: RTC_Init
//-----------------------------------------------------------------------------
void RTC_Init(void)
{
	RTCE = 0;		// Disable real-time counter
	RTCMK = 1;		// Disable INTRTC
	RTCIF = 0;		// Clear INTRTC flag
	RTCIMK = 1;		// Disable INTRTCI
	RTCIIF = 0;		// Clear INTRTCI flag
	RTCPR = 1;		// Set INTRTC low priority
	RTCCL = 0;		// RTC input clock (fRTC)=fSUB
	RTCC0 = 0x0A;	// 24-hour system, INTRTC Once per 1s
	
	// Set RTC initial value
	SEC = 0x00;
	MIN = 0x00;
	HOUR = 0x00;
	WEEK = 0x02;
	DAY = 0x15;
	MONTH = 0x03;
	YEAR = 0x11;
	
	RTCC2 = 0;		// INTRTCI not used, RTCDIV output disabled
	
	RTCMK = 0;		// Enable INTRTC
	RTCE = 1;		// Enable RTC counter
	
	// The following processing is required when shifting to STOP mode immediately after setting RTCE to 1
	RTCC1 |= 0x01;	// RWAIT=1
	while((RTCC1 & 0x02) == 0);		// wait for RWST=1
	RTCC1 &= ~0x01;	// RWAIT=0
	while((RTCC1 & 0x02) == 0x02);	// wait for RWST=0
}


//-----------------------------------------------------------------------------
// Module:   LCD_Init
// Description: LCD_Init
//-----------------------------------------------------------------------------
void LCD_Init(void)
{
	LCDMD = 0x00;	// External resistance division method, internal resistor disconnection
	LCDM = 0x00;	// Display off, Output ground level to segment/common pin, No gate voltage boosting, 1/4 duty, 1/3 bias
	LCDC0 = 0x00;	// LCD source clock (fLCD)=fXT (32.768 kHz), LCD clock (LCDCL)=fLCD/16
}


//-----------------------------------------------------------------------------
// Module:   UART6_Init
// Description: UART6_Init
//-----------------------------------------------------------------------------
void UART6_Init(void)
{
	TXE6 = 0;		//disable uart6 transmit
	RXE6 = 0;		// disable uart6 receive
	POWER6 = 0;		// disable uart6
	STMK6 = 1;		// disable INTST6
	STIF6 = 0;		// clear INTST6 flag
	SRMK6 = 1;		// disable INTSR6
	SRIF6 = 0;		// clear INTSR6 flag
	SREMK6 = 1;		// disable INTSRE6
	SREIF6 = 0;		// clear INTSRE6 flag
	STPR6 = 1;		// set INTST6 low priority
	SRPR6 = 1;		// set INTSR6 low priority
	ASIM6 = 0x05;	// No parity bit; 8 Data bit; 1 stop bit; 
					// INTSR6 occurs in case of error (at this time, INTSRE6 does not occur)
	ASICL6 = 0x16;	// LSB first; Normal output of TXD6
	CKSR6 = 0x05;	// fxclk6 = fPRS/2^5
	BRGC6 = 0x0D;	// Baud rate = fxclk6/2/BRGC6
	
	// Set TXD6, RXD6 pin
	PF1 |= 0x40;	// P16 used as TXD6
	PM1 &= 0xBF;	// P16 output
	PM1 |= 0x20;	// P15 input
	ISC &= 0xCF;
	ISC |= 0x10;	// TxD6:P16, RxD6: P15
}


//-----------------------------------------------------------------------------
// Module:   UART6_Start
// Description: UART6_Start
//-----------------------------------------------------------------------------
void UART6_Start(void)
{
	STIF6 = 0;	/* clear INTST6 flag */
	STMK6 = 1;	/* INTST6 disable */
	SRIF6 = 0;	/* clear INTSR6 flag */
	SRMK6 = 1;	/* disable INTSR6 */	
	POWER6 = 1;	/* enable uart6 */
	TXE6 = 1;	/* enable uart6 transmit */
	RXE6 = 1;	/* enable uart6 receive */
}


//-----------------------------------------------------------------------------
// Module:   UART6_Stop
// Description: UART6_Stop
//-----------------------------------------------------------------------------
void UART6_Stop(void)
{
	TXE6 = 0;	/* disable uart6 transmit */
	RXE6 = 0;	/* disable uart6 receive */
	POWER6 = 0;	/* disable uart6 */
	STMK6 = 1;	/* disable INTST6 */
	STIF6 = 0;	/* clear INTST6 flag */
	SRMK6 = 1;	/* disable INTSR6 */
	SRIF6 = 0;	/* clear INTSR6 flag */
}


//-----------------------------------------------------------------------------
// Module:   UART6_SendByte
// Description: UART6_SendByte
//-----------------------------------------------------------------------------
void UART6_SendByte(unsigned char DataByte)
{
	while(ASIF6 & 0x02);	// wait for transmit buffer empty
	TXB6 = DataByte;
	while(STIF6 == 0);		// wait for transmit completed (interrupt flag)
	STIF6 = 0;
}


//-----------------------------------------------------------------------------
// Module:   RS485_Init
// Description: RS485_Init
//-----------------------------------------------------------------------------
void RS485_Init(void)
{
	RS485_DIR = RS485_DIR_RX;
	UART6_Init();
	UART6_Start();
}






