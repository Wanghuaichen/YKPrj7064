/****************************************************************************
* ���ƣ�TDC-GP1.C
* ���ܣ�ʱ�������
****************************************************************************/


#include  "config.h"

#define	  ale  0x00000040

uint8 out_c[4];

#define		MUTIFACTOR		1600 //�˷�����//

#define  UART_BPS	115200			/* ����ͨѶ������ */
/****************************************************************************
* ���ƣ�DelayNS
* ���ܣ�ʱ�ӡ�
****************************************************************************/
void DelayNS(uint32 dly)
{
   uint32 i;
   
   for(; dly>0;dly--)

       for(i=0;i<5000;i++);

}

/****************************************************************************
* ���ƣ�UART0_Ini()
* ���ܣ���ʼ������0������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ115200
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  UART0_Ini(void)
{  uint16 Fdiv;

   U0LCR = 0x83;		            // DLAB = 1�������ò�����
   Fdiv = (Fpclk / 16) / UART_BPS;  // ���ò�����
   U0DLM = Fdiv / 256;							
   U0DLL = Fdiv % 256;						
   U0LCR = 0x03;
}

 /****************************************************************************
* ���ƣ�UART0_SendByte()
* ���ܣ��򴮿ڷ����ֽ����ݣ����ȴ�������ϡ�
* ��ڲ�����data		Ҫ���͵�����
* ���ڲ�������
****************************************************************************/
void  UART0_SendByte(uint8 data)
{  U0THR = data;				    // ��������

   while( (U0LSR&0x40)==0 );	    // �ȴ����ݷ������
}
/****************************************************************************
* ���ƣ�UART0_SendStr()
* ���ܣ��򴮿ڷ���һ�ַ���
* ��ڲ�����srt		Ҫ���͵��ַ�����ָ��
* ���ڲ�������
****************************************************************************/
void  UART0_SendStr(uint8 *str)
{  while(1)
   {  if( *str == '\0' ) break;
      UART0_SendByte(*str++);	    // ��������
   }
}


uint8   SEND_STRING[8];

/****************************************************************************
* ���ƣ�Init_GP1
* ���ܣ���ʼ��GP1,���ƼĴ������á�
****************************************************************************/
void Init_GP1(void)
{
    ConReg7  = 0x00;
    //DelayNS(2);
    ConReg11 = 0xA3;
    //DelayNS(2);
	ConReg0  = 0x48;	//init reg0//
	//DelayNS(2);
	ConReg1  = 0x40;	//init reg1//
	//DelayNS(2);
	ConReg2  = 0x01;	//init reg2//
	//DelayNS(2);
	ConReg4  = 0x80;	//init reg4//
	//DelayNS(2);
	ConReg6  = 0x22;	//init reg6// 
	//DelayNS(2);
	ConReg7  = 0x41;	//init reg7// 
	//DelayNS(2);
	 
}


/****************************************************************************
* ���ƣ�ReadResultReg
* ���ܣ���ȡGP1����Ĵ�����
****************************************************************************/
void ReadResultReg()
{
	out_c[0] = ResultReg0;
	
	//DelayNS(1);
	
	out_c[1] = ResultReg0;
	
	//DelayNS(1);
	
	out_c[2] = ResultReg0;
	
	//DelayNS(1);
	
	out_c[3] = ResultReg0;
	
	
}

float result;
double result1;

int16 vi;
  
uint16 vf;


/****************************************************************************
* ���ƣ��������Ĵ�������
* ���ܣ���ȡGP1����Ĵ�����
****************************************************************************/

float CalResultReg()
{
  
  
  
  
  vf=out_c[0]|(out_c[1]*256);
  
  vi=out_c[2]|(out_c[3]*256);
  
  result=(float)vi+(float)vf/65536.0;
  
  result=result*MUTIFACTOR;
  
  return  result; 
  
  
}

uint8* float_char(double data)//double
{
	uint8 str[11];
	int temp,temp1;
	int a0,a1,a2,a3,a4,a5,a6,a7,a8;

	temp=(int)data;
	temp1=temp;
	a0=temp/1000;
	str[0]=a0+0x30;


	temp=temp-a0*1000;
	a1=temp/100;
	str[1]=a1+0x30;


	temp=temp-a1*100;
	a2=temp/10;
	str[2]=a2+0x30;


	temp=temp-a2*10;
	a3=temp/1;
	str[3]=a3+0x30;

	str[4]='.';
     temp=(data-temp1)*10000;

	a5=temp/1000;
	str[5]=a5+0x30;


	temp=temp-a5*1000;
	a6=temp/100;
	str[6]=a6+0x30;


	temp=temp-a6*100;
	a7=temp/10;
	str[7]=a7+0x30;


	temp=temp-a7*10;
	a8=temp;
	str[8]=a8+0x30;
	str[9]='\0';
	str[10]='\n';
	return str;

}




void Read_GP1()
{
 

	//double result1;	
	uint8 *str1;
	
	ReadResultReg(); //���ö�����Ĵ������� 
	CalResultReg();
	result1=(double)result;
	str1=float_char(result1);
	UART0_SendStr(str1);//SEND_STRING
	
	//ConReg11 = 0x03;	//��������TDC//
 }








/****************************************************************************
* ���ƣ�IRQ_Eint0
* ���ܣ��ⲿ�жϺ�����
****************************************************************************/
void __irq IRQ_Eint0(void)
 {
   
   Read_GP1();
   
   //CalResultReg();
   
   ConReg11 = 0x03;	//��������TDC//
 
   EXTINT=0x01;         //����жϱ�־//
   
   VICVectAddr=0;       //�����жϽ���//
  
 
 }





/****************************************************************************
* ���ƣ�main()
* ���ܣ�
****************************************************************************/
int  main(void)
{  
   
   PINSEL0 = 0x00000005;			// ���ùܽ�����GPIO,����UART0  
   PINSEL1 = 0x00000001;        //�����������ӣ�P0.16����ΪEINT0//
   PINSEL2 = 0x0f814914;
   
   BCFG0 = 0x0000ffef;
   BCFG1 = 0x0000ffef;
   BCFG2 = 0x0000ffef;
   
   IO0DIR |= ale; 				// ����I/OΪ���
   
   IO0SET |= ale;				//ale = 1
   
   UART0_Ini();
   
   Init_GP1();
    
   //��EINT0�жϣ�������������������ʹ������IRQ)//
   
   EXTMODE=0x01;               //����EINT0Ϊ���ش���ģʽ//
   
   EXTPOLAR=0x01;              //����EINT0Ϊ�ߵ�ƽ����ش�������//
   
   VICIntSelect=0x00000000;     //���������жϷ���ΪIRQ�ж�//
   
   VICVectCntl0=0x2E;          //����EINT0�жϵ������ж�0//
   
   VICVectAddr0=(int)IRQ_Eint0; //�����жϷ�������ַ//
   
   
   EXTINT=0x01;                //���EINT0�жϱ�־//
   
   VICIntEnable=0x00004000;    //ʹ��EINT0�ж�//
  /* 
 while(1)
   {
   Init_GP1();
   
   DelayNS(100);
   
   Read_GP1();
   
  
   }   
  
  */ 
  
   
   while(1);                    //�ȴ��ж�//
   
   
   return(0);
      
   
   
}

