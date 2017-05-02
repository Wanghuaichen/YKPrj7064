#include "msp430x14x.h"

#define TDC_NSS_H P5OUT |= BIT0 
#define TDC_NSS_L P5OUT &=~BIT0
#define TDC_CLK_H P5OUT |= BIT3
#define TDC_CLK_L P5OUT &=~BIT3
#define TDC_DIO_H P5OUT |= BIT1
#define TDC_DIO_L P5OUT &=~BIT1
#define TDC_DII_H P5IN  &  BIT2



#define TDC_INT !(P2IN & BIT4)
void TDC_SendData(char Data){
/*
	_NOP();
	TDC_NSS_L;
	
	for(char i = 0 ; i < 8 ; i++){
		if(Data & 0x80){
			TDC_DIO_H;
		}else{
			TDC_DIO_L;
		}
		Data <<= 1 ; 
		TDC_CLK_H;
		_NOP();
		TDC_CLK_L;
		_NOP();_NOP();
	}
	
	TDC_NSS_H;
	_NOP();
*/
	
	P5SEL |= 0x0e;
	P5OUT &= ~BIT0;
	P5DIR |= BIT0;
	
	U1CTL =
}
signed int TDC_Decode(signed long Rece_Data){
	return (signed int)((Rece_Data / 0xffff) * 800);		//200ns * 4;
}
signed long TDC_ReceData(){
	signed long Buf = 0;
	_NOP();	
	TDC_NSS_L;
	char Data = 0xB0;
	for(char i = 0 ; i < 8 ; i++){
		if(Data & 0x80){
			TDC_DIO_H;
		}else{
			TDC_DIO_L;
		}
		Data <<= 1 ; 
		TDC_CLK_H;
		_NOP();
		TDC_CLK_L;
		_NOP();_NOP();
	}	

	for(char i = 0 ; i < 32 ; i++){
		TDC_CLK_H;
		_NOP();_NOP();
		if(TDC_DII_H){
			Buf |= BIT0;
		}
		Buf <<= 1 ;
		TDC_CLK_L;
		_NOP();_NOP();
	}
	
	return Buf;
}

void TCD_Port_Init(){
	P5DIR |= (BIT0 + BIT1 +  BIT3 + BIT4);
	P5DIR &= ~BIT2;
	P2DIR &= ~BIT4;
}

void TDC_Init(){
	TCD_Port_Init();
	TDC_SendData(0x50);		//上电复位
	
	TDC_SendData(0x80);             //写命令操作码，写REG0
	TDC_SendData(0x00);             //0x00表示都是非反向输入，测量范围选择为1
	TDC_SendData(0x25);             //0x15表示除以2    
	TDC_SendData(0x60);

	TDC_SendData(0x81);            	//写命令操作码，写REG1
	TDC_SendData(0x91);            	//采用的是1rd stop2 hits-1rd stop1 hits
	TDC_SendData(0x09);		//通道1和通道2预计脉冲数都为1
	TDC_SendData(0x00);

	TDC_SendData(0x82);            	//写命令操作码，写REG2
	//TDC_SendData(0xE0);           //原来是0x40
	TDC_SendData(0x80);		//ALU中断触发
	TDC_SendData(0x00);
	TDC_SendData(0x00);

	TDC_SendData(0x83);            	//写命令操作码，写REG3
	TDC_SendData(0x20);
	TDC_SendData(0x00);
	TDC_SendData(0x00);

	TDC_SendData(0x84);            	//写命令操作码，写REG4
	TDC_SendData(0x08);
	TDC_SendData(0x00);
	TDC_SendData(0x00);

	TDC_SendData(0x70);            	//初始化TDC

	//TDC_En_Stop2;          		//Enable pin stop input 2   HIGH acT10ve
	//TDC_En_Stop1;         		//Enable pin stop input 1  HIGH acT10ve
	//TDC_En_Start;          		//Enable pin start input   HIGH acT10ve
	
	P2DIR |= (BIT1 + BIT2 + BIT3);
	P2OUT |= (BIT1 + BIT2 + BIT3);
	_NOP();
}

void main()
{
	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW + WDTHOLD;
	TDC_Init();

	while(1){
		while(TDC_INT){
			TDC_Decode(TDC_ReceData());
			TDC_SendData(0x70);
		}
	}
}