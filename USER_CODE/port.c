//**********************************************************************
//�ļ���:port.c
//�����б�:
//1.��ʼ��GPIO��:   Init_Port
//2 CPU���е�ȡ��   LED_CPU_RUN_Cpl
//
//**********************************************************************
#include "port.h"
#include "global.h"
#include "type_def.h"
#include "lpc2400.h"

//**********************************************************************
//����  ��:Init_Port
//��������:��ʼ��GPIO�ں���չ�ڵ�״̬
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Init_Port(void)
{
    SCS |= 0x01;                //io01 is configed as fast mode
    //����P0   PIN0.01 2  3   456789 10 11 12 13 14 15 
    //                 RX TX                           
    PINSEL0 &= 0xf0f03f0f;      //P0.0-P0.15
    PINSEL0 |= 0x00000050;  
    //����P0   PIN0.161718 19   202122  2324  25 26  2728  293031 
    //                nc        x9313        ntc     I2C             
    PINSEL1 &= 0xfc03c0cf;      //P0.0-P0.15
    PINSEL1 |= 0x01540000;
       
    FIO0DIR &= 0xf9Dbccff;
    FIO0DIR |= 0x00043c80;
    //����P1   PIN1.0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    //         P1.01489 connect GP21    
    //         P1.3     PWM0[2] OUTPUT 1.35KHZ DRIVE SIGNAL                  
    //         P1.7     PWM0[5] TFT BACKLIGHT CONTROL SIGNAL here always 1
    PINSEL2 &= 0xffff330f;      //P1.0-P1.15
    PINSEL2 |= 0x0000CCC0;  
    //SPEAK_PWM_DRI
    //����P2   PIN2.0      1     23456789 10 11  12   13 14 15 
    //             run                          eint2             
    //             out                           in
    PINSEL4 &= 0xfcfffffc;      //p2.0 p2.1 p2.12
    PINSEL4 |= 0x01000000;      //
    FIO2DIR |= EO2_LED_CPU_S;
    //����P3   PIN3.0123456789101112131415
    //PINSEL6 = 0x00000000;      //P3.0-P3.15
    
    //����P3   PIN3.16         17     18       1920212223    24     25262728293031 
    //                       P3_WP  drive_sig             speaker
    //                        OUT     OUT                  pwm1[1]
    PINSEL7 &= 0xfffcffc0;      //P3.18
    PINSEL7 |= 0x00030001;   
    
    FIO3DIR |= (L_DRV_OUT|P3_WP);
    //����P4   
    //P4 NOT CONFIGED
    FIO0SET = WIRELESS_LED1|WIRELESS_LED2;
 }
 
//**********************************************************************
//����  ��:LED_CPU_RUN_Cpl
//��������:cpu ����ָʾ��ȡ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************  
void  LED_CPU_RUN_Cpl(void)
 {
   if(FIO2PIN & EO2_LED_CPU_S)
     FIO2CLR = EO2_LED_CPU_S;
     else
     FIO2SET = EO2_LED_CPU_S;

 }
 //**********************************************************************
//����  ��:
//��������:Wireless1����ָʾ��ȡ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************  
void  Wireless_Run_Led1_Cpl(void)
 {
   if(FIO0PIN & WIRELESS_LED1)
     FIO0CLR = WIRELESS_LED1;
     else
     FIO0SET = WIRELESS_LED1;

 }
 //**********************************************************************
//����  ��:
//��������:Wireless2 ����ָʾ��ȡ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************  
void  Wireless_Run_Led2_Cpl(void)
 {
   if(FIO0PIN & WIRELESS_LED2)
     FIO0CLR = WIRELESS_LED2;
     else
     FIO0SET = WIRELESS_LED2;

 }