//**********************************************************************
//�ļ���:ad_acquire.c
//�����б�:
//1.��ʼ����ʱ��1:         Init_Timer1  ������û��ʹ��
//2.���λ��ⲿ�ж�2:       Init_EINT2
//3.CPLD ԭʼ�����ۼӺ���  epm7064s_filter_raw
//4.�����жϴ���:          FIQ_Exception(��ȡԭʼ����)
//5.�����ۼӺ��ԭʼ����:  Get_Raw
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

static volatile uint32 Raw_Data     = 0;                       //ԭʼ�����ۼ�TOTAL_TIMESΪһ��������
static volatile uint32 Raw_Data_Tmp = 0;
static uint8  raw_data_high = 0;
static uint32 raw_data_low  = 0; 

//**********************************************************************
//��������:Init_Timer1
//�������ܣ���ʼ����ʱ��1,���ڶ�������м���
//**********************************************************************
void Init_Timer1(void)
{
    T1TCR = 0x02;                                 // ��λT1TC
    T1CCR = 0;                                    // ��ֹ����
    T1PR  = 0;                                    // ���ö�ʱ��Ԥ��Ƶֵ������ʱ�Ӽ���                         
    T1MCR = 0x00;                                 // ��ƥ�䲻�����ж�
}

//**********************************************************************
//��������:Init_EINT2()
//��������:��ʼ���ⲿ�ж�2,���ڼ�����ַ���
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Init_EINT2(void)
{
    #if(ADDATA_IC == GP21) 
    //EXTMODE = 0x00;                //����EINT2�ж�Ϊ��ƽ����ģʽ
    EXTMODE = 0x04;                  //����EINT2�ж�Ϊ���ش���
    //EXTPOLAR = 0x04;               //�ߵ�ƽ���������ش���
    EXTPOLAR = 0x00;                 //�͵�ƽ���������ش���
    #else
    EXTMODE  = 0x04;                  //����EINT2�ж�Ϊ���ش���
    EXTPOLAR = 0x0;               //�ߵ�ƽ���������ش���
    #endif   
    EXTINT = 0x04;                   //���EINT2�жϱ�־
    VICIntSelect |= 0x00010000;      //EINT2�ж���ΪFIQ
    VICVectAddr16 = (uint32)FIQ_Exception;
    VICVectPri16 = 0x0;
    VICIntEnable |= 0x00010000;     //ʹ��EINT1�ж�*/
    
   //emp_convert_flag = 0;

}

//**********************************************************************
//ԭʼ���ݴ���
//(1)T1����8
//(2)�ۼ�����ȥ�����1/512
//(3)�ۼ������ϱ����µ�����
//(4)�ۼ�512�����һ��������
//�ۼ� �˲�
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
 ua = k;   //ȡ��Ͳ���
 if(k>255) //��λ�������1
 a = a + j + 1;
 else
 a = a + j; //ua over
 //////////////////////////////////
 k = 0;
 k = ub + ua;
 ub = k;  //ȡ��Ͳ���
 if(k>255)
 b = b + a + 1;
 else
 b = b + a;
 ////////////ub over
 raw_data_low = raw_data_low + (uint32)b;
 if(raw_data_low < b)  //������λ
 raw_data_high++;
 Raw_Data = (raw_data_high<<24)|((raw_data_low>>8)&0x00ffffff); 
 }
 
 
//**********************************************************************
//��������:FIQ_Exception()
//��������:�����жϴ������,��ȡAD7190������
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
         //����DS18B20
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
    
    EXTINT = 0x04;                //���EINT0�жϱ�־
}
/////////////////////////////////////////////////////
///////////////////////////////////////////////////// 
 uint32 Get_Raw(void)
 {
  return(Raw_Data_Tmp);
 }