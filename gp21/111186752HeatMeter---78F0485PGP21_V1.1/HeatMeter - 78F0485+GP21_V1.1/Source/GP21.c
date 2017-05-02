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


//-----------------------------------------------------------------------------
// Function prototyps
//-----------------------------------------------------------------------------
unsigned long GP21_Fire1Measure(unsigned char *pTimeoutFlag, unsigned char *pNoIntFlag);
unsigned long GP21_Fire2Measure(unsigned char *pTimeoutFlag, unsigned char *pNoIntFlag);
unsigned long GP21_CalResonator(unsigned char *pTimeoutFlag, unsigned char *pNoIntFlag);
void GP21_Reset(void);
void SPI_Enable(void);
void SPI_Disable(void);
void SPI_SendZero(void);
void SPI_SendOne(void);
void SPI_WriteCommand(unsigned char wbuf8);
void SPI_WriteRegister(unsigned long wbuf32);
unsigned long SPI_ReadResult(unsigned char addr);
unsigned int SPI_ReadStatus(void);
unsigned char SPI_ReadTest(void);


//-----------------------------------------------------------------------------
// Local Macro definition
//-----------------------------------------------------------------------------
#define THREE_STOP_EN				// Caculate 3 stop hits


#define GP21_SCK			P3.3
#define GP21_SI				P3.2
#define GP21_SO				P3.1
#define GP21_RST			P3.0
#define GP21_INT			P1.7
#define GP21_SSN			P3.4


#define COMMAND_INIT					0x70
#define COMMAND_PowerOnReset			0x50
#define COMMAND_StartTOF				0x01
#define COMMAND_StartTemp				0x02
#define COMMAND_StartCalResonator		0x03
#define COMMAND_StartTOFRestart			0x05
#define COMMAND_StartTempRestart		0x06

#define COMMAND_ReadReg0				0xB0
#define COMMAND_ReadReg1				0xB1
#define COMMAND_ReadReg2				0xB2
#define COMMAND_ReadReg3				0xB3
#define COMMAND_ReadStatus				0xB4
#define COMMAND_ReadTest				0xB5


//REG0: 30 fire pulses; fire frequency divided by 4; 4 periods for calibrating ceramic resonator;
//      CLKHS divided by 1; Oscillator settling time 480uS; 4 temperature ports; 
//		512us cycle time for temperature; 2 fake measurements for temperature;
//		use 128*CLKHS as period for cycle clock for temperature;
//      calibration on; auto-calibration after measurement; measurement range 2;
//      start/stop1/stop2 rising edge;
#define GP21_REG0		0x80E34B68

//REG1: 1.stop ch1 - start; Fast init mode disabled; stop channel 2 disabled;
//      4 hits on stop channel 1; 32K oscillator low current; Use FIRE as Start;
#define GP21_REG1_stop1	0x81214440

//REG1: 2.stop ch1 - start; Fast init mode disabled; stop channel 2 disabled;
//      4 hits on stop channel 1; 32K oscillator low current; Use FIRE as Start;
#define GP21_REG1_stop2	0x81314440

//REG1: 3.stop ch1 - start; Fast init mode disabled; stop channel 2 disabled;
//      4 hits on stop channel 1; 32K oscillator low current; Use FIRE as Start;
#define GP21_REG1_stop3	0x81414440

//REG2: Timeout interrupt enable; End Hits interrupt disable; ALU interrupt enable;
//      delay1=9556(74.65625uS);
#define GP21_REG2		0x82A02554

//REG3: Timeout forces ALU to write 0xFFFFFFFF into the output register;
//      256us timeout in measurement range 2; delay2=0;
#define GP21_REG3		0x83280000

//REG4: delay3=0;
#define GP21_REG4		0x84200000

//REG5: fire1 on; fire2 off;
//      noise unit off; enable phase noise unit; no REPEAT_FIRE; PHASE_FIRE no inversion
#define GP21_REG5_fire1	0x85400000

//REG5: fire1 off; fire2 on;
//      noise unit off; enable phase noise unit; no REPEAT_FIRE; PHASE_FIRE no inversion
#define GP21_REG5_fire2	0x85200000

//REG6: Internal analog section used; Internal schmitt trigger used; 0mV offset;
//      Charge time 90uS; 1 multiple of 50/60Hz to trigger second temperature measurement;
//      1 multiple of 50/60Hz to trigger second TOF measurement; Select 50Hz; 
//      The default level of inactive fire buffer is low; QUAD_RES off; DOUBLE_RES off;
//      Measure order PT1>PT2>PT3>PT4; 30 fire pulses;
#define GP21_REG6		0x86C00041



//-----------------------------------------------------------------------------
// Module:   GP21_Init
// Description: GP21_Init
//-----------------------------------------------------------------------------
void GP21_Init(void)
{
	GP21_Reset();
	SPI_WriteCommand(COMMAND_PowerOnReset);
	SPI_WriteRegister(GP21_REG0);
	SPI_WriteRegister(GP21_REG1_stop1);
	SPI_WriteRegister(GP21_REG2);
	SPI_WriteRegister(GP21_REG3);
	SPI_WriteRegister(GP21_REG4);
	SPI_WriteRegister(GP21_REG5_fire1);
	SPI_WriteRegister(GP21_REG6);
}


//-----------------------------------------------------------------------------
// Module:   GP21_GetDeltaT
// Description: Complete once measurement including Downstream-time, 
//			    clock calibration, and upstream-time.
// return value: signed 32-bit value, high 16-bit are integer, low 16-bit are
//               fractional digits. To get the final value(unit: ns), this
//               return value must be divided by 65536, and multiplied by 250ns.
// return Parameter: *pValidFlag - Indicate whether the returned value is valid.
//-----------------------------------------------------------------------------
signed long GP21_GetDeltaT(unsigned char *pValidFlag)
{
	unsigned char	TimeoutFlag, NoIntFlag;
	signed long		UpStream, DownStream, CalResonator;
	signed long 	DeltaT;
	float			DeltaT_float;
	
	UpStream = (signed long)GP21_Fire1Measure(&TimeoutFlag, &NoIntFlag);	// upstream
	if(NoIntFlag || TimeoutFlag)	// GP21 error processing
	{
		*pValidFlag = 0;
		GP21_Init();
		return(0);
	}
	
/*	CalResonator = (signed long)GP21_CalResonator(&TimeoutFlag, &NoIntFlag);	// calibrate clock
	if(NoIntFlag || TimeoutFlag)	// GP21 error processing
	{
		*pValidFlag = 0;
		GP21_Init();
		return(0);
	}
*/	
	DownStream = (signed long)GP21_Fire2Measure(&TimeoutFlag, &NoIntFlag);	// downstream
	if(NoIntFlag || TimeoutFlag)	// GP21 error processing
	{
		*pValidFlag = 0;
		GP21_Init();
		return(0);
	}
	
	*pValidFlag = 1;
	DeltaT = UpStream - DownStream;	// UpStream is always larger than DownStream, but it's not true
									// when water flows very slowly or water is quiet.
	
//	DeltaT_float = ((float)(DeltaT)) * 0x01E84800 / CalResonator;	// multiply clock calibration factor
	
	return(DeltaT);
}


//-----------------------------------------------------------------------------
// Module:   GP21_GetDeltaT_StartTOFRestart
// Description: Complete once measurement including Downstream-time and upstream-time.
// return value: signed 32-bit value, high 16-bit are integer, low 16-bit are
//               fractional digits. To get the final value(unit: ns), this
//               return value must be divided by 65536, and multiplied by 250ns.
// return Parameter: *pValidFlag - Indicate whether the returned value is valid.
//-----------------------------------------------------------------------------
signed long GP21_GetDeltaT_StartTOFRestart(unsigned char *pValidFlag)
{
	unsigned int	status;
	unsigned int	cnt;
	unsigned char 	TimeoutFlag;
	unsigned char 	NoIntFlag;
	signed long		UpStream, DownStream;
	signed long 	DeltaT;
	
	SPI_WriteCommand(COMMAND_INIT);
	SPI_WriteCommand(COMMAND_StartTOFRestart);
	
	cnt = 1000;		// if int signal has not occurred until 5ms, break.
	while(GP21_INT)	// wait for low level on INT pin of GP21
	{
		cnt--;
		if(cnt == 0)
		{
			NoIntFlag = 1;
			return(0);
		}
	}
	NoIntFlag = 0;
	
	#ifdef	THREE_STOP_EN
	
		SPI_WriteRegister(GP21_REG1_stop2);
		for(cnt=0; cnt<10; cnt++);		// wait for 4.6us at least
		SPI_WriteRegister(GP21_REG1_stop3);
		for(cnt=0; cnt<10; cnt++);		// wait for 4.6us at least
	
	#endif
	
	status = SPI_ReadStatus();
	if((status & 0x0600) == 0)	// timeout flags are 0
	{
		TimeoutFlag = 0;
		UpStream = (signed long)SPI_ReadResult(COMMAND_ReadReg0);
		
		#ifdef	THREE_STOP_EN
		
		UpStream += (signed long)SPI_ReadResult(COMMAND_ReadReg1);
		UpStream += (signed long)SPI_ReadResult(COMMAND_ReadReg2);
		SPI_WriteRegister(GP21_REG1_stop1);
		
		#endif
	}
	else	// timeout error process
	{
		TimeoutFlag = 1;
	}
	
	SPI_WriteCommand(COMMAND_INIT);
	
	cnt = 10000;		// if int signal has not occurred until 50ms, break.
	while(GP21_INT)	// wait for low level on INT pin of GP21
	{
		cnt--;
		if(cnt == 0)
		{
			NoIntFlag = 1;
			return(0);
		}
	}
	NoIntFlag = 0;
	
	#ifdef	THREE_STOP_EN
	
		SPI_WriteRegister(GP21_REG1_stop2);
		for(cnt=0; cnt<10; cnt++);		// wait for 4.6us at least
		SPI_WriteRegister(GP21_REG1_stop3);
		for(cnt=0; cnt<10; cnt++);		// wait for 4.6us at least
	
	#endif
	
	status = SPI_ReadStatus();
	if((status & 0x0600) == 0)	// timeout flags are 0
	{
		TimeoutFlag = 0;
		DownStream = (signed long)SPI_ReadResult(COMMAND_ReadReg0);
		
		#ifdef	THREE_STOP_EN
		
		DownStream += (signed long)SPI_ReadResult(COMMAND_ReadReg1);
		DownStream += (signed long)SPI_ReadResult(COMMAND_ReadReg2);
		SPI_WriteRegister(GP21_REG1_stop1);
		
		#endif
	}
	else	// timeout error process
	{
		TimeoutFlag = 1;
	}
	
	if(NoIntFlag || TimeoutFlag)	// GP21 error processing
	{
		*pValidFlag = 0;
		GP21_Init();
		return(0);
	}
	
	*pValidFlag = 1;
	DeltaT = UpStream - DownStream;	// UpStream is always larger than DownStream, but it's not true
									// when water flows very slowly or water is quiet.
	
	#ifdef	THREE_STOP_EN
	
		DeltaT = DeltaT / 3;
		
	#endif
	
	return(DeltaT);
}


//-----------------------------------------------------------------------------
// Module:   GP21_Fire1Measure
// Description: Fire1 transmit, fire2 receive
//-----------------------------------------------------------------------------
unsigned long GP21_Fire1Measure(unsigned char *pTimeoutFlag, unsigned char *pNoIntFlag)
{
	unsigned int	status;
	unsigned int	cnt;
	
	SPI_WriteRegister(GP21_REG5_fire1);
	SPI_WriteCommand(COMMAND_INIT);
	SPI_WriteCommand(COMMAND_StartTOF);
	
	cnt = 1000;		// if int signal has not occurred until 5ms, break.
	while(GP21_INT)	// wait for low level on INT pin of GP21
	{
		cnt--;
		if(cnt == 0)
		{
			*pNoIntFlag = 1;
			return(0);
		}
	}
	*pNoIntFlag = 0;
	
	status = SPI_ReadStatus();
	if((status & 0x0600) == 0)	// timeout flags are 0
	{
		*pTimeoutFlag = 0;
		return(SPI_ReadResult(COMMAND_ReadReg0));
	}
	else	// timeout error process
	{
		*pTimeoutFlag = 1;
		return(0);
	}
}


//-----------------------------------------------------------------------------
// Module:   GP21_Fire2Measure
// Description: Fire2 transmit, fire1 receive
//-----------------------------------------------------------------------------
unsigned long GP21_Fire2Measure(unsigned char *pTimeoutFlag, unsigned char *pNoIntFlag)
{
	unsigned int	status;
	unsigned int	cnt;
	
	SPI_WriteRegister(GP21_REG5_fire2);
	SPI_WriteCommand(COMMAND_INIT);
	SPI_WriteCommand(COMMAND_StartTOF);
	
	cnt = 1000;		// if int signal has not occurred until 5ms, break.
	while(GP21_INT)	// wait for low level on INT pin of GP21
	{
		cnt--;
		if(cnt == 0)
		{
			*pNoIntFlag = 1;
			return(0);
		}
	}
	*pNoIntFlag = 0;
	
	status = SPI_ReadStatus();
	if((status & 0x0600) == 0)	// timeout flags are 0
	{
		*pTimeoutFlag = 0;
		return(SPI_ReadResult(COMMAND_ReadReg0));
	}
	else	// timeout error process
	{
		*pTimeoutFlag = 1;
		return(0);
	}
}


//-----------------------------------------------------------------------------
// Module:   GP21_CalResonator
// Description: Calibrate 4MHz clock
//-----------------------------------------------------------------------------
unsigned long GP21_CalResonator(unsigned char *pTimeoutFlag, unsigned char *pNoIntFlag)
{
	unsigned int	status;
	unsigned int	cnt;
	
	SPI_WriteCommand(COMMAND_INIT);
	SPI_WriteCommand(COMMAND_StartCalResonator);
	
	cnt = 1000;		// if int signal has not occurred until 5ms, break.
	while(GP21_INT)	// wait for low level on INT pin of GP21
	{
		cnt--;
		if(cnt == 0)
		{
			*pNoIntFlag = 1;
			return(0);
		}
	}
	*pNoIntFlag = 0;
	
	status = SPI_ReadStatus();
	if((status & 0x0600) == 0)	// timeout flags are 0
	{
		*pTimeoutFlag = 0;
		return(SPI_ReadResult(COMMAND_ReadReg0));
	}
	else	// timeout error process
	{
		*pTimeoutFlag = 1;
		return(0);
	}
}


//-----------------------------------------------------------------------------
// Module:   GP21_Reset
// Description: Reset GP21
//-----------------------------------------------------------------------------
void GP21_Reset(void)
{
	GP21_RST = 0;	// followed by 50ns at least
	NOP();
	NOP();
	GP21_RST = 1;	// followed by 200ns at least
	NOP();
	NOP();
	NOP();
	NOP();
}


//-----------------------------------------------------------------------------
// Module:   SPI_Enable
// Description: SPI_Enable
//-----------------------------------------------------------------------------
void SPI_Enable(void)
{
	GP21_SSN = 0;	// followed by 40ns at least
	NOP();
	NOP();
}


//-----------------------------------------------------------------------------
// Module:   SPI_Disable
// Description: SPI_Disable
//-----------------------------------------------------------------------------
void SPI_Disable(void)
{
	NOP();			// 70ns at least
	GP21_SSN = 1;	// followed by 50ns at least
	NOP();
	NOP();
}


//-----------------------------------------------------------------------------
// Module:   SPI_SendZero
// Description: SPI_SendZero
//-----------------------------------------------------------------------------
void SPI_SendZero(void)
{
	GP21_SCK = 1;	// SCK high level 50ns at least
	NOP();
	GP21_SI = 0;
	NOP();
	GP21_SCK = 0;	// SCK low level 50ns at least
	NOP();
}


//-----------------------------------------------------------------------------
// Module:   SPI_SendOne
// Description: SPI_SendOne
//-----------------------------------------------------------------------------
void SPI_SendOne(void)
{
	GP21_SCK = 1;	// SCK high level 50ns at least
	NOP();
	GP21_SI = 1;
	NOP();
	GP21_SCK = 0;	// SCK low level 50ns at least
	NOP();
}


//-----------------------------------------------------------------------------
// Module:   SPI_WriteCommand
// Description: SPI_WriteCommand
//-----------------------------------------------------------------------------
void SPI_WriteCommand(unsigned char wbuf8)
{
	unsigned char cnt, tmp;
	
	tmp = 0x80;
 	SPI_Enable();
 	for(cnt=8; cnt>0; cnt--)
    {
    	if((wbuf8&tmp) == 0)
       		SPI_SendZero();
     	else
       		SPI_SendOne();
     	
     	tmp = tmp >> 1;
    }
    SPI_Disable();
}


//-----------------------------------------------------------------------------
// Module:   SPI_WriteRegister
// Description: SPI_WriteRegister
//-----------------------------------------------------------------------------
void SPI_WriteRegister(unsigned long wbuf32)
{
	unsigned char cnt;
 	unsigned long tmp;
 	
 	tmp = 0x80000000;
 	SPI_Enable();
 	for(cnt=32; cnt>0; cnt--)
    {
     	if((wbuf32&tmp) == 0)
       		SPI_SendZero();
     	else
       		SPI_SendOne();
     
     	tmp = tmp >> 1;
    }
    SPI_Disable();
}


//-----------------------------------------------------------------------------
// Module:   SPI_ReadResult
// Description: SPI_ReadResult
//-----------------------------------------------------------------------------
unsigned long SPI_ReadResult(unsigned char addr)
{
 	unsigned char cnt, tmp8;
 	unsigned long tmp32;
 	unsigned long rbuf32;
 	
 	tmp8 = 0x80;
 	SPI_Enable();
 	for(cnt=8; cnt>0; cnt--)
    {
    	if((addr&tmp8) == 0)
       		SPI_SendZero();
     	else
       		SPI_SendOne();
     	
     	tmp8 = tmp8 >> 1;
    }
    
    tmp32 = 0x80000000;
    rbuf32 = 0x00000000;
 	for(cnt=32; cnt>0; cnt--)
    {
    	GP21_SCK = 1;
     	NOP();
    	if(GP21_SO == 1)
        	rbuf32 |= tmp32;
     	tmp32 = tmp32 >> 1;
     	GP21_SCK = 0;
     	NOP();
    }
    SPI_Disable();
    
 	return(rbuf32);
}


//-----------------------------------------------------------------------------
// Module:   SPI_ReadStatus
// Description: SPI_ReadStatus
//-----------------------------------------------------------------------------
unsigned int SPI_ReadStatus(void)
{
 	unsigned char cnt, tmp8;
 	unsigned int  tmp16;
 	unsigned int  rbuf16;
 	
 	tmp8 = 0x80;
 	SPI_Enable();
 	for(cnt=8; cnt>0; cnt--)
    {
    	if((COMMAND_ReadStatus&tmp8) == 0)
       		SPI_SendZero();
     	else
       		SPI_SendOne();
     	
     	tmp8 = tmp8 >> 1;
    }
    
    tmp16 = 0x8000;
    rbuf16 = 0x0000;
 	for(cnt=16; cnt>0; cnt--)
    {
    	GP21_SCK = 1;
     	NOP();
    	if(GP21_SO == 1)
        	rbuf16 |= tmp16;
     	tmp16 = tmp16 >> 1;
     	GP21_SCK = 0;
     	NOP();
    }
    SPI_Disable();
    
 	return(rbuf16);
}


//-----------------------------------------------------------------------------
// Module:   SPI_ReadTest
// Description: This function is only for testing communication between MCU and GP21.
//-----------------------------------------------------------------------------
unsigned char SPI_ReadTest(void)
{
 	unsigned char cnt, tmp8;
 	unsigned char rbuf8;
 	
 	tmp8 = 0x80;
 	SPI_Enable();
 	for(cnt=8; cnt>0; cnt--)
    {
    	if((COMMAND_ReadTest&tmp8) == 0)
       		SPI_SendZero();
     	else
       		SPI_SendOne();
     	
     	tmp8 = tmp8 >> 1;
    }
    
    tmp8 = 0x80;
    rbuf8 = 0x00;
 	for(cnt=8; cnt>0; cnt--)
    {
    	GP21_SCK = 1;
     	NOP();
    	if(GP21_SO == 1)
        	rbuf8 |= tmp8;
     	tmp8 = tmp8 >> 1;
     	GP21_SCK = 0;
     	NOP();
    }
    SPI_Disable();
    
 	return(rbuf8);
}

