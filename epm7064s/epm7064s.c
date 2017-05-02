#include "global.h"
#include "config.h"
#include "epm7064s.h"

static void delay (int length)
	{
	  while (length >0)
    	 length--;
	 }
//**************************************************************************************/
//  ������     void Start_SPI(void)
//  �������   ��
//  �������   ��
/***************************************************************************************/
void  Epm7064sPIN_Init(void)
{
	 //����P0   PIN0.4  PIN0.5  PIN0.6  PIN0.7 
         //���ÿ�����  ��Ϊ�����      
     SCS      = SCS | 0x01 ;    //use FAST IO 
    
     PINSEL0 &= 0xffffc0ff;      //P0.456
     FIO0DIR |= (EPM7_DATA_LOW_EN | EPM7_DATA_HIG_EN);
     
     //����P1   PIN1.10 11 12 13 14 15 16 17 
     //�������� ��Ϊ�����
     PINSEL2 &= 0x000fffff;   //P1.15 14 13 12 11 10
     PINSEL3 &= 0xfffffff0;   //P1.17 16
     FIO1DIR &= ~(EPM7_BUS_DATA0|EPM7_BUS_DATA1|EPM7_BUS_DATA2|EPM7_BUS_DATA3|EPM7_BUS_DATA4);
     FIO1DIR &= ~(EPM7_BUS_DATA5|EPM7_BUS_DATA6|EPM7_BUS_DATA7);  
     
      //7064init
    //FIO0SET = 0x00000080;
    //FIO0CLR = 0x00000100;
}

void  Motor_Pin_Init(void)
{
	 //����P0   PIN0.7  PIN0.17  PIN0.18 
     //���ÿ�����  ��Ϊ�����      
     SCS      = SCS | 0x01 ;    //use FAST IO 
    
     PINSEL0 &= 0x3fffffff;      //P0.15 P0.16
     PINSEL1 &= 0xfffffffc;      //P0.17 p0.18
     
     PINSEL4 &= 0xfffffff3;      //P2.1
         
     FIO0DIR |= (EPM7_MOTOR_P1 | EPM7_MOTOR_P2);
     FIO2DIR &= 0xfffffffd;
     
      //Ĭ�Ͻ�ֹ���
     FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;
    }
    
    
uint32 Read_Epm7064_Low(void) //��ȡEPM7064���ֽ�
{
  uint32 i;
  IO_EPM7_CLR = EPM7_DATA_LOW_EN;
  delay(10);
  i = ((FIO1PIN&0x0003fc00)>>10)&0xff;
  IO_EPM7_SET = EPM7_DATA_LOW_EN;
  return(i);
}	
uint32 Read_Epm7064_High(void) //��ȡEPM7064���ֽ�
{
  uint32 i;
  IO_EPM7_CLR = EPM7_DATA_HIG_EN;
  delay(10);
  i = ((FIO1PIN&0x0003fc00)>>10)&0x1f;  //ȡ5λ
  IO_EPM7_SET = EPM7_DATA_HIG_EN;
  return(i);
}
void Epm7_Data_Pro(uint32 raw_data) //EPM7064ԭʼ���ݵĴ���
{
  static uint32 x1; //55 54 53h
  static uint32 x2; //59 58 57h
  static uint32 x3;
  static uint32 flag_5ah;
  
         uint32 i,j,k;
  i = raw_data<<8;       
  i = i-x1;  //��һ��
  j = x1;
  }

