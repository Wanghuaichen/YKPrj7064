/****************************************************************************
* 名称：TDC-GP1.C
* 功能：时间测量。
****************************************************************************/


#include  "config.h"

#define	  ale  0x00000040

uint8 out_c[4];

#define		MUTIFACTOR		1600 //乘法因子//

#define  UART_BPS	115200			/* 定义通讯波特率 */
/****************************************************************************
* 名称：DelayNS
* 功能：时延。
****************************************************************************/
void DelayNS(uint32 dly)
{
   uint32 i;
   
   for(; dly>0;dly--)

       for(i=0;i<5000;i++);

}

/****************************************************************************
* 名称：UART0_Ini()
* 功能：初始化串口0。设置为8位数据位，1位停止位，无奇偶校验，波特率为115200
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  UART0_Ini(void)
{  uint16 Fdiv;

   U0LCR = 0x83;		            // DLAB = 1，可设置波特率
   Fdiv = (Fpclk / 16) / UART_BPS;  // 设置波特率
   U0DLM = Fdiv / 256;							
   U0DLL = Fdiv % 256;						
   U0LCR = 0x03;
}

 /****************************************************************************
* 名称：UART0_SendByte()
* 功能：向串口发送字节数据，并等待发送完毕。
* 入口参数：data		要发送的数据
* 出口参数：无
****************************************************************************/
void  UART0_SendByte(uint8 data)
{  U0THR = data;				    // 发送数据

   while( (U0LSR&0x40)==0 );	    // 等待数据发送完毕
}
/****************************************************************************
* 名称：UART0_SendStr()
* 功能：向串口发送一字符串
* 入口参数：srt		要发送的字符串的指针
* 出口参数：无
****************************************************************************/
void  UART0_SendStr(uint8 *str)
{  while(1)
   {  if( *str == '\0' ) break;
      UART0_SendByte(*str++);	    // 发送数据
   }
}


uint8   SEND_STRING[8];

/****************************************************************************
* 名称：Init_GP1
* 功能：初始化GP1,控制寄存器配置。
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
* 名称：ReadResultReg
* 功能：读取GP1结果寄存器。
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
* 名称：计算结果寄存器数据
* 功能：读取GP1结果寄存器。
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
	
	ReadResultReg(); //调用读结果寄存器函数 
	CalResultReg();
	result1=(double)result;
	str1=float_char(result1);
	UART0_SendStr(str1);//SEND_STRING
	
	//ConReg11 = 0x03;	//重新启动TDC//
 }








/****************************************************************************
* 名称：IRQ_Eint0
* 功能：外部中断函数。
****************************************************************************/
void __irq IRQ_Eint0(void)
 {
   
   Read_GP1();
   
   //CalResultReg();
   
   ConReg11 = 0x03;	//重新启动TDC//
 
   EXTINT=0x01;         //清除中断标志//
   
   VICVectAddr=0;       //向量中断结束//
  
 
 }





/****************************************************************************
* 名称：main()
* 功能：
****************************************************************************/
int  main(void)
{  
   
   PINSEL0 = 0x00000005;			// 设置管脚连接GPIO,激活UART0  
   PINSEL1 = 0x00000001;        //设置引脚连接，P0.16设置为EINT0//
   PINSEL2 = 0x0f814914;
   
   BCFG0 = 0x0000ffef;
   BCFG1 = 0x0000ffef;
   BCFG2 = 0x0000ffef;
   
   IO0DIR |= ale; 				// 设置I/O为输出
   
   IO0SET |= ale;				//ale = 1
   
   UART0_Ini();
   
   Init_GP1();
    
   //打开EINT0中断（设置向量控制器，即使用向量IRQ)//
   
   EXTMODE=0x01;               //设置EINT0为边沿触发模式//
   
   EXTPOLAR=0x01;              //设置EINT0为高电平或边沿触发触发//
   
   VICIntSelect=0x00000000;     //设置所有中断分配为IRQ中断//
   
   VICVectCntl0=0x2E;          //分配EINT0中断到向量中断0//
   
   VICVectAddr0=(int)IRQ_Eint0; //设置中断服务程序地址//
   
   
   EXTINT=0x01;                //清除EINT0中断标志//
   
   VICIntEnable=0x00004000;    //使能EINT0中断//
  /* 
 while(1)
   {
   Init_GP1();
   
   DelayNS(100);
   
   Read_GP1();
   
  
   }   
  
  */ 
  
   
   while(1);                    //等待中断//
   
   
   return(0);
      
   
   
}

