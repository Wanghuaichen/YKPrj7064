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
          
   
	PCONP 	|= 1 << 3;						                            /* ʹ��UART0                    */
	PINSEL0  = (PINSEL0 & 0xFFFFFF0F) | (0x01 << 4) | (0x01 << 6);	    //����P0.2��P0.3���ӵ�UART0                                    */
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
  ///////////////////////////////��Ϊ����һ֡���� ��������
  send_tmp[i++] = 'S';
  send_tmp[i++] = 'T';
                        //////////////////////////���б�־
                        if(TRUE == mode_run_flag)
                           send_tmp[i++] = '1';
                        else   
                           send_tmp[i++] = '0';             
                        /////////////////////////�ȶ���־        
                        if(TRUE == stable_flag)
                           send_tmp[i++] = '1';
                        else   
                           send_tmp[i++] = '0';
                        /////////////////////////������־                              
                        if(TRUE == positive_flag)
                           send_tmp[i++] = '1';
                        else   
                           send_tmp[i++] = '0';
                       ///////////////////////////////////////////////      
                        switch(current_mode)
                        {
                         case  STAT_WEIGH:  send_tmp[i++] = '0';    //����ģʽ
                                            switch(current_unit)    //����ģʽ�����ļ�����λ��
                                             {
                              				 	case  UNIT_G: send_tmp[i++] = '0'; 
                              				 	              if(QIAN_DIVISION == dot_position)
                              				 	               {
                              				 	                sprintf(&send_tmp[i],"%7.3f",net_weight); //���油һ��0
                              				 	               }
                              				 	              else 
                              				                  sprintf(&send_tmp[i],"%8.4f",net_weight);
                              				                   	             
                              				 	              break;
                              				 	case UNIT_MG: send_tmp[i++] = '1'; 
                              				 	              if(QIAN_DIVISION == dot_position)
                              				 	               {
                              				 	                sprintf(&send_tmp[i],"%6d",(uint32)(net_weight * G_TO_MG));//���油����0
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
                                            //sprintf(&send_tmp[i],"%8.4f",net_weight); //���;�������
                                            i += 8;
                                            
                                            break;
                         case    STAT_PCS:  send_tmp[i++] = '1';
                                            if(QIAN_DIVISION == dot_position)
                                             {
                                              sprintf(&send_tmp[i],"%7.3f",weight_per_pcs); //���͵����� 
                                             }
                                            else
                                            sprintf(&send_tmp[i],"%8.4f",weight_per_pcs); //���͵�����
                                            i += 8;
                                            sprintf(&send_tmp[i],"%5d",  pcs_number);     //���͸���
                                            i += 5;
                                            
                                            break;
                         case    STAT_100:  send_tmp[i++] = '2';
                                            if(QIAN_DIVISION == dot_position)
                                             {
                                              sprintf(&send_tmp[i],"%7.3f",weight_compare_100); //����100%����
                                             }
                                             else
                                             {
                                              sprintf(&send_tmp[i],"%8.4f",weight_compare_100); //����100%����
                                             }
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.2f",number_100);         //���Ͱٷֱ�����
                                            i += 8;
                               
                                            break;
                         case STAT_CHECK:   send_tmp[i++] = '3';
                                            if(QIAN_DIVISION == dot_position)
                                            {
                                            sprintf(&send_tmp[i],"%7.3f",limit1_weight); //��������
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.3f",limit2_weight); //��������
                                            i += 8;
                                            sprintf(&send_tmp[i],"%7.3f",net_weight);    //���;���
                                            i += 8;
                                            }
                                            else
                                            {
                                            sprintf(&send_tmp[i],"%8.4f",limit1_weight); //��������
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f",limit2_weight); //��������
                                            i += 8;
                                            sprintf(&send_tmp[i],"%8.4f",net_weight);    //���;���
                                            i += 8;
                                            
                                            }
                                            break;
                         case STAT_DIFFER:  send_tmp[i++] = '4';
                                            //��ʱ������
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
                                            //��ʱ������
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
  //У����
                     j = i;
                     for(j=0;j<i;j++)
                     check_value += send_tmp[j];
                     send_tmp[i++] = check_value;                   
 //����  
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
   UART_PrintChar(0x26);  //�������Ĵ�ӡ
  }
static void  Printer_EN_MODE(void)
  { 
   UART_PrintChar(0x1C);
   UART_PrintChar(0x2E);  //�˳����Ĵ�ӡ
  }

void UART_Print_Char1(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x36);  //ѡ���ַ��� 1
 }  
void UART_Print_Char2(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x37);  //ѡ���ַ��� 2
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
  UART_PrintChar(0x02);  //�������򶼷Ŵ�һ��
  /////////////////////////////////////////////
  Printer_EN_MODE();
  
  //����ƫ��λ��
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
  /////////////////////ȡ���ͺ����� 
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
    UART_PrintStr("������ƽ");
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
  UART_PrintChar(0x01);  //����

  UART_PrintChar(0x1b);
  UART_PrintChar(0x56);  
  UART_PrintChar(0x02);  //���򶼷Ŵ�һ��
  */
  ////////////////////////////////////////////
}

static void  Printer_TABLE(void)
{
  uint32 temp_tmp;
  uint8  send_tmp[20];
  RTCTime time_tmp = RTCGetTime();
  
  temp_tmp = (uint32)(temp_float*10);
  ////////////////////////////////////////�û���� 
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("�û����");
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
  ///////////////////////////////////////��ӡ����
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("��ӡʱ��");
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
  ///////////////////////////////////////�����¶�
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("��ǰ�¶�");
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
    ///////////////////////////////////////�����¶�
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("���϶�");
   }
  else
   {
    UART_PrintStr("degree Celsius");
   } 
  
  Printer_EN_MODE();
  
  Printer_enter();
  ///////////////////////////////////////��ǰģʽ
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("��ǰģʽ");
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
     case STAT_WEIGH:    UART_PrintStr("��������");   break;
     case STAT_PCS:      UART_PrintStr("����");       break;
     case STAT_100:      UART_PrintStr("�ٷֱ�");     break;
     case STAT_CHECK:    UART_PrintStr("����ģʽ");   break;
     case STAT_DIFFER:   UART_PrintStr("���컯");     break;
     case STAT_TOTAL:    UART_PrintStr("�ۼ�ģʽ");   break;
     case STAT_ANIMAL:   UART_PrintStr("�������");   break;
     case STAT_DENSITY:  UART_PrintStr("�ܶȲ���");   break;
     case STAT_PEAK:     UART_PrintStr("��ֵģʽ");   break;
     case STAT_TARGET:   UART_PrintStr("Ŀ�����");   break;
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
    UART_PrintStr("ǩ��");
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
    UART_PrintStr("��ǰ����");
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
    sprintf(&send_tmp[i],"%8.4f",net_weight);    //���;���
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
  
  //////////////////////////��ӡ�ڶ�������
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    switch(current_mode)
    {
     //case STAT_WEIGH:    UART_PrintStr("COUNT");          break;
     case STAT_PCS:      UART_PrintStr("����");             break;
     case STAT_100:      UART_PrintStr("�ٷֱ�");           break;
     //case STAT_CHECK:    UART_PrintStr("CHECK WEIGHT");   break;
     //case STAT_DIFFER:   UART_PrintStr("DIFFER MODE");    break;
     case STAT_TOTAL:    UART_PrintStr("�ۼ�����");         break;
     //case STAT_ANIMAL:   UART_PrintStr("ANIMAL MODE");    break;
     //case STAT_DENSITY:  UART_PrintStr("DENSITY MODE");   break;
     case STAT_PEAK:     UART_PrintStr("���ֵ");           break;
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
                           sprintf(&send_tmp[i],"%7.2f",number_100);         //���Ͱٷֱ�����
                           i += 8;
                           break;
     //case STAT_CHECK:    UART_PrintStr("CHECK WEIGHT");   break;
     //case STAT_DIFFER:   UART_PrintStr("DIFFER MODE");    break;
     case STAT_TOTAL:      send_tmp[i++]  = ':';
                           send_tmp[i++]  = ' ';
                           if(QIAN_DIVISION == dot_position)
                           sprintf(&send_tmp[i],"%7.3f",total_weight);
                           else
                           sprintf(&send_tmp[i],"%8.4f",total_weight);       //�����ۼӺ�����
                           i += 8;
                           break;
     case STAT_PEAK:       send_tmp[i++]  = ':';
                           send_tmp[i++]  = ' ';
                           if(QIAN_DIVISION == dot_position)
                           sprintf(&send_tmp[i],"%7.3f",peak_weight);
                           else
                           sprintf(&send_tmp[i],"%8.4f",peak_weight);        //���ͷ�ֵ����
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
    UART_PrintChar(0x40);  //��ʼ����ӡ��
    
    UART_PrintChar(0x1b);
    UART_PrintChar(0x63);
    UART_PrintChar(0x00);  //�����Ҵ�ӡ
    
    UART_Print_Char1();
  }   
    