#include "global.h"
#include "config.h"
#include "gp21.h"

static void delay (int length)
	{
	  while (length >0)
    	 length--;
	 }
//**************************************************************************************/
//  函数名     void Start_SPI(void)
//  输入参数   无
//  输出参数   无
/***************************************************************************************/
void  GP21PIN_Init(void)
	{

	 //配置P1   PIN1.0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
     //         P1.01489 AD7190USE   
     SCS      = SCS | 0x01 ;    //use FAST IO 
     PINSEL2 &= 0xfff0fcf0;      //P1.01489
     PINSEL2 |= 0x00000000;   
	 IO_GP21_DIR  |=   GP21_SSN | GP21_SCLK | GP21_DIN | GP21_RST;
     IO_GP21_DIR  &=  ~GP21_DOUT;
     }
void spi_enable(void) //开启spi 通信,将ssn 置低
{
  IO_GP21_CLR = GP21_SSN;
  delay(2);
  //IO_GP21_CLR = GP21_SCLK;
}	
/////////////////////////////////////////////////
void spi_disable(void) //关闭spi 通信,将ssn 置高
{
 //SSN=1;//ssn 置高
 IO_GP21_SET = GP21_SSN;
 delay(2);
}
///////////////////////////////////////////////
void send_zero(void) //发送0
{
 IO_GP21_SET = GP21_SCLK; //SCK=1;//SCK 高电平
 delay(1);                //_nop_();
 IO_GP21_CLR = GP21_DIN;  //SI=0;//SI-输出一个低平
 delay(1);                //_nop_();
 IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK 低平
 delay(1);                //_nop_();
}
////////////////////////////////////////////////
void send_one(void) //发送1
{
 IO_GP21_SET = GP21_SCLK; //SCK=1;//SCK 高电平
 delay(1);                //_nop_();
 IO_GP21_SET = GP21_DIN;  //SI=1;//SI-输出一个高平
 delay(1);                //_nop_();
 IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK 低平
 delay(1);                //_nop_();
}
////////////////////////////////////////
void spi_writeCMD(uint8 wbuf8)  // spi 写入8位数据
{
uint8 cnt,tmp=0x80;
spi_enable();
for(cnt=8;cnt>0;cnt--)
	{
     if((wbuf8&tmp)!=0)
         send_one(); //发送1
         else
         send_zero();//发送0
     tmp = tmp >> 1; //tmp 右移一位
	} 
 IO_GP21_SET = GP21_SCLK; //SCK=1;//SCK 高电平
 spi_disable();

}
////////////////////////////////////////
void spi_write8(uint8 wbuf8)  // spi 写入8位数据
{
uint8 cnt,tmp=0x80;
spi_enable();
for(cnt=8;cnt>0;cnt--)
	{
     if((wbuf8&tmp)!=0)
         send_one(); //发送1
         else
         send_zero();//发送0
     tmp = tmp >> 1; //tmp 右移一位
	} //没有spi 关闭命令，测试程序中代码关闭！

// IO_GP21_SET = GP21_SCLK; //SCK=1;//SCK 高电平

}
/////////////////////////////////////////////////////////////
void spi_write32(uint32 wbuf32) // spi 写32 位数据
{
 uint8   cnt;
 uint32  tmp=0x80000000;
 //spi_enable();
 for(cnt=32;cnt>0;cnt--)
 {
    if((wbuf32&tmp)!=0)
       send_one();//送1
       else
       send_zero();//发送0
    tmp = tmp >> 1; //tmp 右移一位
 }
 spi_disable();
 }
 //=====================SPI 读数据=====================//
uint16 spi_read16()
{
 uint8   cnt;
 uint16  tmp    = 0x8000;
 uint16  rbuf16 = 0x0000;
 //spi_enable();
 for(cnt=16;cnt>0;cnt--)
 {
  //IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK
  IO_GP21_SET = GP21_SCLK; //SCK=0;//SCK
 // delay(2);
  if(IO_GP21_PIN & GP21_DOUT)  //if(SO==1)//P6.2 SO
     rbuf16 |=tmp;
  //IO_GP21_SET = GP21_SCLK;  //SCK=1;// SCK
  IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK
  //delay(2);
  tmp = tmp >> 1;
  }
 spi_disable();
 return(rbuf16);
}
//=====================SPI 读数据=====================//
uint32 spi_read24()
{
 uint8   cnt;
 uint32  tmp    = 0x800000;
 uint32  rbuf32 = 0x000000;
 //spi_enable();
 for(cnt=24;cnt>0;cnt--)
 {
  //IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK
  IO_GP21_SET = GP21_SCLK; //SCK=0;//SCK
  //delay(2);
  if(IO_GP21_PIN & GP21_DOUT)  //if(SO==1)//P6.2 SO
     rbuf32 |=tmp;
  //IO_GP21_SET = GP21_SCLK;  //SCK=1;// SCK
  IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK
 // delay(2);
  tmp = tmp >> 1;
  }
// spi_disable();
 return(rbuf32);
}
//=====================SPI 读数据=====================//
uint32 spi_read32()
{
 uint8   cnt;
 uint32  tmp    = 0x80000000;
 uint32  rbuf32 = 0x00000000;
 //spi_enable();
 for(cnt=32;cnt>0;cnt--)
 {
  //IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK
  IO_GP21_SET = GP21_SCLK; //SCK=0;//SCK
  delay(2
  );
  if(IO_GP21_PIN & GP21_DOUT)  //if(SO==1)//P6.2 SO
     rbuf32 |=tmp;
  //IO_GP21_SET = GP21_SCLK;  //SCK=1;// SCK
  IO_GP21_CLR = GP21_SCLK; //SCK=0;//SCK
  delay(2);
  tmp = tmp >> 1;
  }
 spi_disable();
 return(rbuf32);
}
///////////////////////////////////////////////////
//===========GP2 上电复位程序==================//
void GP21_RESET(void)
{
IO_GP21_SET = GP21_RST; // RST=1; //输出高平
delay(2);
IO_GP21_CLR = GP21_RST; //RST=0; //输出低平
delay(5000);
IO_GP21_SET = GP21_RST; //RST=1; //输出高平
delay(2);
}              //给gp2RSTN 管脚一个Reset 的方波
//===========GP2 寄存器配置程序====================//
void GP21_init(void)
{
uint32  REG0,REG1,REG2,REG3,REG4,REG5,REG6;
uint8   PORST = 0x50;
uint8   init  = 0x70;
/*REG0=0x800c3000;//校准陶瓷晶振时间为8 个32k 周期,244.14us.		   0x80008420
//设置高速晶振上电后一直起振.设置测量范围1,自动校准,上升沿敏感
REG1=0x81014100;//预期stop1 脉冲数1 个.计算第一个stop1-start
REG2=0x82E00000;//开启所有中断源
REG3=0x83080000;//									     
REG4=0x84200000;//
REG5=0x85080000;//*/
REG0 = 0x00056911;
REG1 = 0x21420022; 
REG2 = 0x20000033;
REG3 = 0x38000044;
REG4 = 0x20000055;
REG5 = 0xE8000066;
REG6 = 0x00002077;

GP21_RESET();
delay(5);
spi_writeCMD(PORST);//上电复位
delay(5000);
//////////////////////////
spi_write8(0x80);
spi_write32(REG0);
delay(5);
spi_write8(0x80);
spi_write32(REG0);
delay(5);
spi_write8(0x81);
spi_write32(REG1);
delay(5);
spi_write8(0x82);
spi_write32(REG2);
delay(5);
spi_write8(0x83);
spi_write32(REG3);
delay(5);
spi_write8(0x84);
spi_write32(REG4);
delay(5);
spi_write8(0x85);
spi_write32(REG5);
delay(5);
spi_write8(0x86);
spi_write32(REG6);
delay(5);

////////////////////////////////////////
spi_writeCMD(init);//初始化
delay(5);
//spi_writeCMD(0x01);//start measure loop

}
///////////////////////////////////////////////
//===========GP2 时钟校准程序===================//
void GP21_cal(void)
{
uint8  cal_start = 0x03;
uint8  read_reg0 = 0xb0;
uint32 CAL;
//float CAL_f;
//EN_START=1;//EN_START        
spi_write8(cal_start);//启动校准
delay(5);
spi_disable();
while(FIO2PIN & GP21_DOUT) //判断中断置位否 
delay(5);
spi_write8(read_reg0); //读校准的时间数据
delay(5);
CAL=spi_read32();//通过计算校准系数为244.146/(float(CAL)/65536*0.250))
delay(5);
}
