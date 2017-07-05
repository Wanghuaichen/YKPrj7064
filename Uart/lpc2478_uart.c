/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_uart.c
* Programmer(s)  : Prasad.K.R.S.V
* Version        :
*
**************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                           INCLUDE HEADER FILES
**************************************************************************************************************
*/
#include "lpc2478_uart.h"
#include "rtc.h"
#include "disp.h"

#include "global.h"
/*
**************************************************************************************************************
*                                         INITIALIZE UART
*
* Description: This function initializes UART port, setup pin select, clock, parity, stopbits, FIFO etc
*
* Arguments  : baud_rate    UART baud rate (115200)
*
* Returns    : None
*
**************************************************************************************************************
*/
#define   DIVADDVAL   1                                                 
#define   MULVAL      12   

void  UART_Init(INT32U baudrate)
{
    INT32U  Fdiv;
          
   
	PCONP 	|= 1 << 3;						                            /* 使能UART0                    */
	PINSEL0  = (PINSEL0 & 0xFFFFFF0F) | (0x01 << 4) | (0x01 << 6);	    //设置P0.2和P0.3连接到UART0                                    */
   	U0LCR    = 0x83;		                  /* 8 bits, no Parity, 1 Stop bit                              */
    Fdiv     = (((2 * Fpclk) / 16 / baudrate) + 1) / 2;
    
    U0DLM    = (Fdiv>>8)&0xff;      //) / 256;							                                                        
    U0DLL    =      Fdiv&0xff;      // % 256;
	U0LCR    = 0x03;		                  /* DLAB = 0
	                                                   */
    //U0FDR    = (DIVADDVAL << 0)|(MULVAL << 4); 
    
    U0FCR    = 0x07;		                  /* Enable and reset TX and RX FIFO                            */


}

/*
**************************************************************************************************************
*                                         PRINT CHARECTER
*
* Description: This function is used to print a single charecter through UART1.
*
* Arguments  : ch    charecter to be printed
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_PrintChar (INT8U ch)
{

   while (!(U0LSR & 0x20));
   U0THR  = ch;
}

void  UART_CommStr(char *str,uint8 length)
{
 uint8 i;
 for(i=0;i<length;i++)
       UART_PrintChar(*str++);
 
 }
 
 void  UART_PrintStr(char *str)
{
 while(*str)
 {
   UART_PrintChar(*str++);
 }
}

/*
**************************************************************************************************************
*                                    send data to PC
*
**************************************************************************************************************
*/

void  UART_SendData (void)
{
  uint8  check_value;
  uint32 i,j;
  char send_tmp[35];
  //////////////////////////////
  check_value = 0;
  i = 0;
  ///////////////////////////////改为发送一帧数据 包含所有
  send_tmp[i++] = 'S';
  send_tmp[i++] = 'T';
                        //////////////////////////运行标志
                        if(TRUE == mode_run_flag)
                           send_tmp[i++] = '1';
                        else   
                           send_tmp[i++] = '0';             
                        /////////////////////////稳定标志        
                        if(TRUE == stable_flag)
                           send_tmp[i++] = '1';
                        else   
                           send_tmp[i++] = '0';
                        /////////////////////////正负标志                              
                        if(TRUE == positive_flag)
                           send_tmp[i++] = '1';
                        else   
                           send_tmp[i++] = '0';
                       ///////////////////////////////////////////////      
                        switch(current_mode)
                        {
                         case  STAT_WEIGH:  send_tmp[i++] = '0';    //哪种模式
                                            switch(current_unit)    //重量模式下有哪几个单位？
                                             {
                              				 	case  UNIT_G: send_tmp[i++] = '0'; 
                              				 	              if(QIAN_DIVISION == dot_position)
                              				 	               {
                              				 	                sprintf(&send_tmp[i],"%7.3f",net_weight); //后面补一个0
                              				 	               }
                              				 	              else 
                              				                  sprintf(&send_tmp[i],"%8.4f",net_weight);
                              				                   	             
                              				 	              break;
                              				 	case UNIT_MG: send_tmp[i++] = '1'; 
                              				 	              if(QIAN_DIVISION == dot_position)
                              				 	               {
                              				 	                sprintf(&send_tmp[i],"%6d",(uint32)(net_weight * G_TO_MG));//后面补两个0
                              				 	               }
                              				 	              else
                              				 	              sprintf(&send_tmp[i],"%8.1f",net_weight * G_TO_MG);
                              				 	              
                              				 	              break;
                               					case UNIT_LB: send_tmp[i++] = '2';   
                               					              sprintf(&send_tmp[i],"%8.6f",net_weight * G_TO_LB);
                               					              break;
                               					case UNIT_OZ: send_tmp[i++] = '3';   
                               					              sprintf(&send_tmp[i],"%8.6f",net_weight * G_TO_OZ);
                               					              break;
                                                default:      send_tmp[i++] = '0';  
                                                              sprintf(&send_tmp[i],"%8.4f",net_weight);
                                                              break;
                                              }
                                            //sprintf(&send_tmp[i],"%8.4f",net_weight); //发送净重数据
                                            i += 8;
                                            
                                            break;
                         case    STAT_PCS:  send_tmp[i++] = '1';
                                            if(QIAN_DIVISION == dot_position)
                                             {
                                              sprintf(&send_tmp[i],"%7.3f",weight_per_pcs); //发送单件重 
                                             }
                                            else
                                            sprintf(&send_tmp[i],"%8.4f",weight_per_pcs); //发送单件重
                                            i += 8;
                                            sprintf(&send_tmp[i],"%5d",  pcs_number);     //发送个数
                                            i += 5;
                                            
                                            break;
                         case    STAT_100:  send_tmp[i++] = '2';
                                            if(QIAN_DIVISION == dot_position)
                                             {
                                              sprintf(&send_tmp[i],"%7.3f",weight_compare_100); //发送100%基点
                                             }
                                             else
                                             {
                                              sprintf(&send_tmp[i],"%8.4f",weight_compare_100); //发送100%基点
                                             }
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.2f",number_100);         //发送百分比数据
                                            i += 8;
                               
                                            break;
                         case STAT_CHECK:   send_tmp[i++] = '3';
                                            if(QIAN_DIVISION == dot_position)
                                            {
                                            sprintf(&send_tmp[i],"%7.3f",limit1_weight); //发送下限
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.3f",limit2_weight); //发送上限
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.3f",net_weight);    //发送净重
                                            i += 8;
                                            }
                                            else
                                            {
                                            sprintf(&send_tmp[i],"%8.4f",limit1_weight); //发送下限
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f",limit2_weight); //发送上限
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f",net_weight);    //发送净重
                                            i += 8;
                                            
                                            }
                                            break;
                         case STAT_DIFFER:  send_tmp[i++] = '4';
                                            //暂时不发送
                                            break;
                         case STAT_TOTAL:   send_tmp[i++] = '5';
                                            if(QIAN_DIVISION == dot_position)
                                            {
                                            sprintf(&send_tmp[i],"%7.3f",total_weight);
                                            }
                                            else
                                            {
                                            sprintf(&send_tmp[i],"%8.4f",total_weight);
                                            }
                                            i += 8;
                                            sprintf(&send_tmp[i],"%3d",  total_times);
                                            i += 3;
                                            if(QIAN_DIVISION == dot_position)
                                            {
                                             sprintf(&send_tmp[i],"%7.3f",net_weight);
                                            }
                                            else
                                            {  
                                             sprintf(&send_tmp[i],"%8.4f",net_weight);
                                            }
                                            i += 8;
                                            
                                            break;
                         case STAT_ANIMAL:  send_tmp[i++] = '6';
                                            /*
                                            if(TRUE == animal_ok)
                                            sprintf(&send_tmp[i],"%8.4f", animal_weight);
                                            else
                                            sprintf(&send_tmp[i],"%8.4f", 0.0);
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f", net_weight);
                                            i += 8;
                                            */
                                            break;
                         case STAT_DENSITY: send_tmp[i++] = '7';
                                            //暂时不发送
                                            break;
                         case STAT_PEAK:    send_tmp[i++] = '8';
                                            if(QIAN_DIVISION == dot_position)
                                            {
                                            sprintf(&send_tmp[i],"%7.3f", peak_weight);
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.3f",  net_weight);
                                            i += 8;
                                            }
                                            else
                                            {
                                            sprintf(&send_tmp[i],"%8.4f", peak_weight);
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f",  net_weight);
                                            i += 8;
                                            
                                            }
                                            break;
                         case STAT_TARGET:  send_tmp[i++] = '9';
                                            if(QIAN_DIVISION == dot_position)
                                            {
                                            sprintf(&send_tmp[i],"%7.3f", target_weight);
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.3f",    net_weight);
                                            i += 8;
                                            }
                                            else
                                            {
                                            sprintf(&send_tmp[i],"%8.4f", target_weight);
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f",    net_weight);
                                            i += 8;
                                            
                                            }  
                                            break;
                         //case STAT_CALING:  send_tmp[5] = '1';  
                         
                         default:           send_tmp[i++] = 'A';
                         break;
                        }
                      
  ///////////////////////////////////////////////////////////////////////////////////////
  //校验码
                     j = i;
                     for(j=0;j<i;j++)
                     check_value += send_tmp[j];
                     send_tmp[i++] = check_value;                   
 //结束  
     send_tmp[i++] = 0x0d;
     send_tmp[i++] = 0x0a;
     /////////////////////////////////////
     if(i > 10)
     UART_CommStr((char *)send_tmp,i);                                                    
}

///////////////////////////////////////////////////////////////////////////////////////////
//add printer function
//20150809
///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static void  Printer_enter(void)
  {
   //UART_PrintChar(0x0d);
   UART_PrintChar(0x0a);
  }
static void  Printer_CH_MODE(void)
  {
   UART_PrintChar(0x1C);
   UART_PrintChar(0x26);  //进入中文打印
  }
static void  Printer_EN_MODE(void)
  { 
   UART_PrintChar(0x1C);
   UART_PrintChar(0x2E);  //退出中文打印
  }

void UART_Print_Char1(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x36);  //选择字符集 1
 }  
void UART_Print_Char2(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x37);  //选择字符集 2
 }    
//////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////  
static void  Printer_TITLE(void)
{
  uint8  i,j;
  
  uint8  send_tmp[20];
  i = 0;
  /////////////////////////
  UART_PrintChar(0x1b);
  UART_PrintChar(0x56);  
  UART_PrintChar(0x02);  //横向纵向都放大一倍
  /////////////////////////////////////////////
  Printer_EN_MODE();
  
  //设置偏移位置
   send_tmp[i++]  = ' ';
   send_tmp[i++]  = ' ';
   send_tmp[i++]  = ' ';
   send_tmp[i++]  = ' ';
  if(CHINESE == Get_Language_Type())
   {
    send_tmp[i++]  = ' ';
    send_tmp[i++]  = ' ';
    send_tmp[i++]  = ' ';
   }
  /////////////////////取得型号数据 
   send_tmp[i++]  = 'S';
   send_tmp[i++]  = 'Y';
   send_tmp[i++]  = (model_id&0xffff)/100      +0x30;
   send_tmp[i++]  = ((model_id&0xffff)%100)/10 +0x30;
   if(1 == dot_position)
   send_tmp[i++]  = '4';
   else
   send_tmp[i++]  = '3';
 //////////////////////////////////
  if(INNER_CAL_VERSION)
    send_tmp[i++]  = 'C';
   
   send_tmp[i++]  = ' ';
  
  send_tmp[i++]  = 0x00;
  UART_PrintStr(send_tmp);
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("电子天平");
   }
  else
   {
    UART_PrintStr("ELECTRONIC BALANCE");
   } 
   
  Printer_enter();
  Printer_enter();
  /////////////////////////////////////////////
  /*
  Printer_EN_MODE();
  UART_PrintChar(0x1b);
  UART_PrintChar(0x55);  
  UART_PrintChar(0x01);  //横向

  UART_PrintChar(0x1b);
  UART_PrintChar(0x56);  
  UART_PrintChar(0x02);  //纵向都放大一倍
  */
  ////////////////////////////////////////////
}

static void  Printer_TABLE(void)
{
  uint32 temp_tmp;
  uint8  send_tmp[20];
  RTCTime time_tmp = RTCGetTime();
  
  temp_tmp = (uint32)(temp_float*10);
  ////////////////////////////////////////用户编号 
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("用户编号");
   }
  else
   {
    Printer_EN_MODE();
    UART_PrintStr("USER ID");
   } 
     
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x30+user_id/100;
  send_tmp[3]  = 0x30+(user_id%100)/10;
  send_tmp[4]  = 0x30+user_id%10;
  send_tmp[5]  = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
  ///////////////////////////////////////打印日期
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("打印时间");
   }
  else
   {
    UART_PrintStr("PRINT TIME");
   } 
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x30+time_tmp.RTC_Mon/10;
  send_tmp[3]  = 0x30+time_tmp.RTC_Mon%10;
  send_tmp[4]  = '-';
  send_tmp[5]  = 0x30+time_tmp.RTC_Day/10;
  send_tmp[6]  = 0x30+time_tmp.RTC_Day%10;
  send_tmp[7]  = ' ';
  send_tmp[8]  = 0x30+time_tmp.RTC_Hour/10;
  send_tmp[9]  = 0x30+time_tmp.RTC_Hour%10;
  send_tmp[10] = ':';
  send_tmp[11] = 0x30+time_tmp.RTC_Min/10;
  send_tmp[12] = 0x30+time_tmp.RTC_Min%10;
  send_tmp[13] = ':';
  send_tmp[14] = 0x30+time_tmp.RTC_Sec/10;
  send_tmp[15] = 0x30+time_tmp.RTC_Sec%10;
  send_tmp[16] = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
  ///////////////////////////////////////环境温度
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("当前温度");
   }
  else
   {
    UART_PrintStr("CUR_TEMP");
   } 
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x30+temp_tmp/100;
  send_tmp[3]  = 0x30+(temp_tmp%100)/10;
  send_tmp[4]  = '.';
  send_tmp[5]  = 0x30+temp_tmp%10; 
  send_tmp[6]  = ' '; 
  send_tmp[7]  = 0x00;
  UART_PrintStr(send_tmp);
  
  //print unit
    ///////////////////////////////////////环境温度
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("摄氏度");
   }
  else
   {
    UART_PrintStr("degree Celsius");
   } 
  
  Printer_EN_MODE();
  
  Printer_enter();
  ///////////////////////////////////////当前模式
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("当前模式");
   }
  else
   {
    UART_PrintStr("CUR_MODE");
   } 
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x00;
  UART_PrintStr(send_tmp);
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    switch(current_mode)
    {
     case STAT_WEIGH:    UART_PrintStr("基本称重");   break;
     case STAT_PCS:      UART_PrintStr("计数");       break;
     case STAT_100:      UART_PrintStr("百分比");     break;
     case STAT_CHECK:    UART_PrintStr("检重模式");   break;
     case STAT_DIFFER:   UART_PrintStr("差异化");     break;
     case STAT_TOTAL:    UART_PrintStr("累加模式");   break;
     case STAT_ANIMAL:   UART_PrintStr("动物称重");   break;
     case STAT_DENSITY:  UART_PrintStr("密度测试");   break;
     case STAT_PEAK:     UART_PrintStr("峰值模式");   break;
     case STAT_TARGET:   UART_PrintStr("目标称重");   break;
     default: break;
    }
   }
  else
   {
    switch(current_mode)
    {
     case STAT_WEIGH:    UART_PrintStr("BASIC WEIGHT");   break;
     case STAT_PCS:      UART_PrintStr("COUNT");          break;
     case STAT_100:      UART_PrintStr("PERCENTAGE");     break;
     case STAT_CHECK:    UART_PrintStr("CHECK WEIGHT");   break;
     case STAT_DIFFER:   UART_PrintStr("DIFFER MODE");    break;
     case STAT_TOTAL:    UART_PrintStr("TOTAL MODE");     break;
     case STAT_ANIMAL:   UART_PrintStr("ANIMAL MODE");    break;
     case STAT_DENSITY:  UART_PrintStr("DENSITY MODE");   break;
     case STAT_PEAK:     UART_PrintStr("PEAK MODE");      break;
     case STAT_TARGET:   UART_PrintStr("TARGET MODE");    break;
     default: break;
    }
   }   
 
  Printer_enter();
}

static void  Printer_SIGN(void)
{
  uint8  send_tmp[20];
  uint8  i;
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("签名");
   }
  else
   {
    UART_PrintStr("Signature");
   } 
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  for(i=1;i<20;i++)
  send_tmp[i]  = '_';
  send_tmp[15] = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter(); 
  Printer_enter();
  Printer_enter();
}

static void  Printer_data(void)
{
  uint8  i;
  uint8  send_tmp[20];
 
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("当前净重");
   } 
  else
   {
    UART_PrintStr("Net_Weight");
   }
   
  Printer_EN_MODE();
  i = 0;
  send_tmp[i++]  = ':';
  if(FALSE == positive_flag)
  send_tmp[i++]  = '-';
  else
  send_tmp[i++]  = ' ';
  
  if(QIAN_DIVISION == dot_position) {
    sprintf(&send_tmp[i],"%7.3f",net_weight);
    i+=7;
    send_tmp[i++]  = ' ';      
  }
  else {
    sprintf(&send_tmp[i],"%8.4f",net_weight);    //发送净重
    i+=8;
  } 
  send_tmp[i++]  = ' ';
  switch(current_unit)
  {
   case UNIT_G:    send_tmp[i++]  = ' ';send_tmp[i++]  = 'g'; break;
   case UNIT_MG:   send_tmp[i++]  = 'm';send_tmp[i++]  = 'g'; break;
   case UNIT_LB:   send_tmp[i++]  = 'L';send_tmp[i++]  = 'b'; break;
   case UNIT_OZ:   send_tmp[i++]  = 'O';send_tmp[i++]  = 'z'; break;
   default:break;
  }
  
  send_tmp[i++]  = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
  
  //////////////////////////打印第二个参数
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    switch(current_mode)
    {
     //case STAT_WEIGH:    UART_PrintStr("COUNT");          break;
     case STAT_PCS:      UART_PrintStr("个数");             break;
     case STAT_100:      UART_PrintStr("百分比");           break;
     //case STAT_CHECK:    UART_PrintStr("CHECK WEIGHT");   break;
     //case STAT_DIFFER:   UART_PrintStr("DIFFER MODE");    break;
     case STAT_TOTAL:    UART_PrintStr("累计重量");         break;
     //case STAT_ANIMAL:   UART_PrintStr("ANIMAL MODE");    break;
     //case STAT_DENSITY:  UART_PrintStr("DENSITY MODE");   break;
     case STAT_PEAK:     UART_PrintStr("最大值");           break;
     //case STAT_TARGET:   UART_PrintStr("TARGET MODE");    break;
     default: break;
    }
   } 
  else
   {
    switch(current_mode)
    {
     //case STAT_WEIGH:    UART_PrintStr("COUNT");          break;
     case STAT_PCS:      UART_PrintStr("COUNT");            break;
     case STAT_100:      UART_PrintStr("PERCENTAGE");       break;
     //case STAT_CHECK:    UART_PrintStr("CHECK WEIGHT");   break;
     //case STAT_DIFFER:   UART_PrintStr("DIFFER MODE");    break;
     case STAT_TOTAL:    UART_PrintStr("TOT.weight");       break;
     //case STAT_ANIMAL:   UART_PrintStr("ANIMAL MODE");    break;
     //case STAT_DENSITY:  UART_PrintStr("DENSITY MODE");   break;
     case STAT_PEAK:     UART_PrintStr("MAX.weight");       break;
     //case STAT_TARGET:   UART_PrintStr("TARGET MODE");    break;
     default: break;
    }
   }
   
  Printer_EN_MODE();
  i = 0;
  switch(current_mode)
    {
     //case STAT_WEIGH:    UART_PrintStr("COUNT");          break;
     case STAT_PCS:      
                           send_tmp[i++]  = ':';
                           send_tmp[i++]  = ' ';
                           send_tmp[i++]  = 0x30+  pcs_number/1000;
                           send_tmp[i++]  = 0x30+ (pcs_number%1000)/100;
                           send_tmp[i++]  = 0x30+ (pcs_number%100)/10;
                           send_tmp[i++]  = 0x30+ (pcs_number%10);   
                           break;
     case STAT_100:        
                           send_tmp[i++]  = ':';
                           send_tmp[i++]  = ' ';
                           sprintf(&send_tmp[i],"%7.2f",number_100);         //发送百分比数据
                           i += 8;
                           break;
     //case STAT_CHECK:    UART_PrintStr("CHECK WEIGHT");   break;
     //case STAT_DIFFER:   UART_PrintStr("DIFFER MODE");    break;
     case STAT_TOTAL:      send_tmp[i++]  = ':';
                           send_tmp[i++]  = ' ';
                           if(QIAN_DIVISION == dot_position)
                           sprintf(&send_tmp[i],"%7.3f",total_weight);
                           else
                           sprintf(&send_tmp[i],"%8.4f",total_weight);       //发送累加和数据
                           i += 8;
                           break;
     case STAT_PEAK:       send_tmp[i++]  = ':';
                           send_tmp[i++]  = ' ';
                           if(QIAN_DIVISION == dot_position)
                           sprintf(&send_tmp[i],"%7.3f",peak_weight);
                           else
                           sprintf(&send_tmp[i],"%8.4f",peak_weight);        //发送峰值数据
                           i += 8;
                           break; 
     default: break;
    }
   
  send_tmp[i++]  = 0x00;
  switch(current_mode)
    {
     case STAT_PCS:      
     case STAT_100:     
     case STAT_TOTAL:     
     case STAT_PEAK:      
                     UART_PrintStr(send_tmp);
     default: break;
    }
 
  Printer_enter();
  Printer_enter();
}  
void  UART_PrintData(void)
{
  Printer_TITLE();
  Printer_TABLE();
  Printer_data(); 
  Printer_SIGN();
  print_busy = 0;
  
 }
 
void  Print_Init(void)
  {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x40);  //初始化打印机
    
    UART_PrintChar(0x1b);
    UART_PrintChar(0x63);
    UART_PrintChar(0x00);  //从左到右打印
    
    UART_Print_Char1();
  }   
    