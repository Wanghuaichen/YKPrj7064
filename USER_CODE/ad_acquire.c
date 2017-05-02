//**********************************************************************
//文件名:ad_acquire.c
//函数列表:
//1.初始化定时器1:         Init_Timer1  本程序没有使用
//2.初次化外部中断2:       Init_EINT2
//3.CPLD 原始数据累加函数  epm7064s_filter_raw
//4.快速中断处理:          FIQ_Exception(读取原始数据)
//5.返回累加后的原始数据:  Get_Raw
//**********************************************************************
#include "ad_acquire.h"
#include "config.h"
#include "port.h"
#include "global.h"
#include "type_def.h"
#include "stdlib.h"
#include "filter.h"
#include "Font_Lib.h"

#include "ds18b20.h"
//#define   TOTAL_TIMES       (112*4)

static volatile uint32 Raw_Data     = 0;                       //原始数据累加TOTAL_TIMES为一次新数据
static volatile uint32 Raw_Data_Tmp = 0;
static uint8  raw_data_high = 0;
static uint32 raw_data_low  = 0; 

//**********************************************************************
//函数名称:Init_Timer1
//函数功能：初始化定时器1,用于对脉冲进行计数
//**********************************************************************
void Init_Timer1(void)
{
    T1TCR = 0x02;                                 // 复位T1TC
    T1CCR = 0;                                    // 禁止捕获
    T1PR  = 0;                                    // 设置定时器预分频值对外设时钟计数                         
    T1MCR = 0x00;                                 // 不匹配不产生中断
}

//**********************************************************************
//函数名称:Init_EINT2()
//函数功能:初始化外部中断2,用于检测涡轮风扇
//入口参数:无
//出口参数:无
//**********************************************************************
void Init_EINT2(void)
{
    #if(ADDATA_IC == GP21) 
    //EXTMODE = 0x00;                //设置EINT2中断为电平触发模式
    EXTMODE = 0x04;                  //设置EINT2中断为边沿触发
    //EXTPOLAR = 0x04;               //高电平或者上升沿触发
    EXTPOLAR = 0x00;                 //低电平或者下升沿触发
    #else
    EXTMODE  = 0x04;                  //设置EINT2中断为边沿触发
    EXTPOLAR = 0x0;               //高电平或者上升沿触发
    #endif   
    EXTINT = 0x04;                   //清除EINT2中断标志
    VICIntSelect |= 0x00010000;      //EINT2中断设为FIQ
    VICVectAddr16 = (uint32)FIQ_Exception;
    VICVectPri16 = 0x0;
    VICIntEnable |= 0x00010000;     //使能EINT1中断*/
    
   //emp_convert_flag = 0;

}

//**********************************************************************
//原始数据处理：
//(1)T1除以8
//(2)累加器减去自身的1/512
//(3)累加器加上本次新的数据
//(4)累加512次输出一次新数据
//累加 滤波
/////////////////////////////////////////       
static void  epm7064s_filter_raw(uint32 x)
       {
 sint32 i,j;
 uint32 k;
 static uint8  u8,ua,ub;
 static sint32     a, b;
 i = x << 8;         //extend 256
 i = i - (b >> 8);  //sub a/65536
 j = a >> 1;         //b/512
 j = i - j;
 //////////////////////ext 4.5bytes 
 u8 = ((u8>>4)&0x0f)|((j&0x0f)<<4); 
 j = j >> 4;         //j/16
 //////////////////////////////////
 k = 0;
 k = ua + u8;
 ua = k;   //取其低部分
 if(k>255) //低位溢出，加1
 a = a + j + 1;
 else
 a = a + j; //ua over
 //////////////////////////////////
 k = 0;
 k = ub + ua;
 ub = k;  //取其低部分
 if(k>255)
 b = b + a + 1;
 else
 b = b + a;
 ////////////ub over
 raw_data_low = raw_data_low + (uint32)b;
 if(raw_data_low < b)  //产生进位
 raw_data_high++;
 Raw_Data = (raw_data_high<<24)|((raw_data_low>>8)&0x00ffffff); 
 }
 
 
//**********************************************************************
//函数名称:FIQ_Exception()
//函数功能:快速中断处理程序,读取AD7190的数据
//**********************************************************************
////////////////////////////////       
void FIQ_Exception(void)
{ 
    static uint32 Cycle_times=0;
    static uint32 Cycle_cnt  =0;
    static uint32 temp_convert_flag=0;
    
    uint32 x,y;
    ////////////////////////////////////////use 7064s
    x = Read_Epm7064_Low();
    y = (Read_Epm7064_High()<<8) | x;
    //AAAAA = y;
    epm7064s_filter_raw(y);
    Cycle_times++;
    if(adc_times == Cycle_times)
        {Cycle_times = 0;
         flag_new_data = TRUE;
         raw_data_high = 0;
         raw_data_low  = 0;
         Raw_Data_Tmp = Raw_Data;
         /////////////////////////send temp convert command 
         //屏蔽DS18B20
         #if USE_DS18B20
         if(1==temp_convert_flag)
          tmp_change();
         else if(5==temp_convert_flag)
          get_temp();
        
         temp_convert_flag++;
         if(temp_convert_flag>10)
         temp_convert_flag = 1;  
        
         #endif
         
         }
    Cycle_cnt++;
    //if(Cycle_cnt%1000 == 0)     
    //UART_SendData();
    
    EXTINT = 0x04;                //清除EINT0中断标志
}
/////////////////////////////////////////////////////
///////////////////////////////////////////////////// 
 uint32 Get_Raw(void)
 {
  return(Raw_Data_Tmp);
 }