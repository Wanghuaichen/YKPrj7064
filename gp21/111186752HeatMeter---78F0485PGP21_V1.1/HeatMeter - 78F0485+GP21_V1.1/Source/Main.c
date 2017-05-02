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
#pragma ei
#pragma di
#pragma stop

#pragma interrupt INTTM000 MD_INTTM000
#pragma interrupt INTKR ISR_INTKR
#pragma interrupt INTRTC ISR_INTRTC


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "GP21.h"
#include "System.h"


//-----------------------------------------------------------------------------
// Function prototyps
//-----------------------------------------------------------------------------
void RS485_SendDoubleWord(unsigned long SendData);


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
unsigned char	DeltaTValidFlag = 0;
signed long		DeltaT = 0;
signed long		AverageDeltaT = 0;
signed long 	MaxValue = 0;
signed long 	MinValue = 0x7fffffff;
unsigned char	MeasureFlag = 0;
unsigned char	KeyPressedFlag = 0;


//-----------------------------------------------------------------------------
// Local constants
//-----------------------------------------------------------------------------
#define AverageCount		10
#define AVERAGE_ENABLE


//-----------------------------------------------------------------------------
// Module:   main
// Description: main module
//-----------------------------------------------------------------------------
void main(void)
{
	signed long		temp;
	unsigned char	i=0;
	
	DI();               // disable all interrupts
	
	System_Init();
	GP21_Init();
	
	EI();               // enable all interrupts
	
	while(1)
	{
//		STOP();
		
		if(KeyPressedFlag)
		{
			KeyPressedFlag = 0;
			LCD_DisplayOn();
		}
		
		if(MeasureFlag)
		{
			MeasureFlag = 0;
			temp = GP21_GetDeltaT_StartTOFRestart(&DeltaTValidFlag);
			if(DeltaTValidFlag)
			{
				DeltaTValidFlag = 0;
				DeltaT = temp;
				
				#ifdef AVERAGE_ENABLE
				
					if(i<AverageCount)
					{
						AverageDeltaT += DeltaT;
						i++;
						if(MaxValue < DeltaT)
							MaxValue = DeltaT;
						if(MinValue > DeltaT)
							MinValue = DeltaT;
					}
					else
					{
//						AverageDeltaT = (AverageDeltaT - MaxValue - MinValue) / (AverageCount - 2);
						AverageDeltaT = AverageDeltaT / AverageCount;
						RS485_SendDoubleWord((unsigned long)AverageDeltaT);
						AverageDeltaT = DeltaT;
						MaxValue = DeltaT;
						MinValue = DeltaT;
						i = 1;
					}
				#else
				
					RS485_SendDoubleWord((unsigned long)DeltaT);
				
				#endif
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Module:   MD_INTTM000
// Description: Interrupt service routine of TM00
//-----------------------------------------------------------------------------
__interrupt void MD_INTTM000(void)
{
	MeasureFlag = 1;
}


//-----------------------------------------------------------------------------
// Module:   ISR_INTKR
// Description: Interrupt service routine of key interrupt
//-----------------------------------------------------------------------------
__interrupt void ISR_INTKR(void)
{
	KeyPressedFlag = 1;
}


//-----------------------------------------------------------------------------
// Module:   ISR_INTRTC
// Description: Interrupt service routine of INTRTC
//-----------------------------------------------------------------------------
__interrupt void ISR_INTRTC(void)
{
//	MeasureFlag = 1;
}


//-----------------------------------------------------------------------------
// Module:   RS485_SendDoubleWord
// Description: RS485_SendDoubleWord
//-----------------------------------------------------------------------------
void RS485_SendDoubleWord(unsigned long SendData)
{
	RS485_SendByte((unsigned char) (SendData >> 24));
	RS485_SendByte((unsigned char) (SendData >> 16));
	RS485_SendByte((unsigned char) (SendData >> 8));
	RS485_SendByte((unsigned char) (SendData));
}



