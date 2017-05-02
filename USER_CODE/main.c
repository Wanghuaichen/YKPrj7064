//**********************************************************************
//�ļ�   ��:
//˵       ��:CPU�ͺ�--LPC2478
//�汾   ��:
//�޸���ʷ:
//**********************************************************************
#include "port.h"
#include "Font_Lib.h"
#include "disp.h"
#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "timer.h"

#include "service.h"
#include "normal_operation.h"
#include "ad_acquire.h"
#include "i2c_com.h"
#include "filter.h"
#include "epm7064s.h"

#include "config.h"
#include "stdio.h"
///////////////////////////////////////USB HEADFILE
#include  "usbcfg.h"

///////////////////////////////////////
uint8 Start_Menu_Pro(void);

#define SDRAM_ADDR	  0xA0000000 
#define SDRAM_SIZE     0x2000000	    //32M bytes=256Mbits

//////////////////////////////////////��ʱ������������ʱ
static void delay15us(void)
	{
	  uint32 i;
	  i = 210;
      while(--i);	  
	 }
	 
//
void  Init_Lpc2478ADC(void)
  {
   PCONP  |= 1 << 12;
   // ����ADCģ������
   AD0CR = (0x08 << 0)					 |	                            /* SEL=0x01,ѡ��ͨ��0            */
			(3   << 8) |	                                                /* 18M/4 = 4.5M       */
			(1   << 16)					 |	                            /* BURST=1,ʹ��Burstģʽ��       */
			                                                            /* ��Ӳ�����ϵؽ���ת��          */
			(0   << 17)					 |	                            /* CLKS=0, ʹ��11clockת��       */
			(1   << 21)					 |                              /* PDN=1,��������ģʽ            */
			(0   << 24);				                                    /* START=0,��Burstģʽ�£�       */
	 //delay(2000);
	 
   //AD0CR |= (1<<24);	                                                      
  
  }	 
////////////////////////////////////////////////////////
sint32 main(void)
{
   
    uint32 i,err;
    //volatile uint16 *piAddr1;
   
    for(i=0;i<1000000;i++){;}
      
    if (WDMOD&0x04) WDMOD &= ~0x04;//���Ź���λ
    //////////////////////////////
    /*   
    SCS = 	SCS | 0x01 ;         //use FAST IO  
    PINSEL4 &= 0xfffffffc;       //p2.0 p2.1 p2.12
    PINSEL4 |= 0x00000000;       //
    FIO2DIR |= EO2_LED_CPU_S; 
	/////////////
    USB_Init();
    USB_Connect(1);         // USB Connect
    while(1){
     //	 if(0 == (FIO3PIN&KEY0))
         {
     	  IO1CLR = LED0;
     	  IO1SET = LED1|LED2|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY1))
         {
     	  IO1CLR = LED1;
     	  IO1SET = LED0|LED2|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY2))
         {
     	  IO1CLR = LED2;
     	  IO1SET = LED0|LED1|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY3))
         {
     	  IO1CLR = LED3;
     	  IO1SET = LED1|LED2|LED0;
     	  }
     	  
     	sysDelayNS(500);
     	LED_CPU_RUN_Cpl();
       }
   //////////////////////////////////test end  */
   // Init_PWM0_DRIVER();    //GP21 use
   /////////////////////////
    Init_Cilib();
   	Init_Graph();
    Init_Port();          //��ʼ��IO��
    
    #if USB_ENABLE
    USB_Init();
    USB_Connect(1);         // USB Connect
    #endif
   
    //////////////////////////���������ڲ˵��н���
    Init_Brightness_PWM(); //use pwm[0]
    Init_Sound_PWM();
    ////////////////////////////////////// 
    /////////////////////////ds18b20 ����
    /*
    if(0 == DS18B20_Init()) 
     FIO2CLR = EO2_LED_CPU_S;
     else
     FIO2SET = EO2_LED_CPU_S;
   
    Clr_Bar(0,0,800,480,COL_BLACK);
	Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
     
    Put_Str(100,100,Language_Str("DS18B20 TEST DATA","DS18B20 TEST DATA"));
    while(1)
    {
     tmp_change();
     for(i=0;i<60;i++)
     delay15us();
     get_temp();
     if(0.0 != temp_float)
     //Put_Data(120,150,"%4d",temp_int);
     Put_Data_Float(120,180,"%4.1f",temp_float);
     for(i=0;i<60;i++)
     delay15us();
     
    } 
    */
    #if(1 == USE_DS18B20)
    DS18B20_Init();
    #endif
   	///////////////////////
    ADS7843PIN_Init();
    ///////////////////////
    //Set_Language_Type(ENGLISH);
    //Set_Language_Type(CHINESE);
    
    ///////////////////////
    UART_Init(9600);
    //���Ӵ�ӡ����
    Print_Init();
    
    Init_Timer0(200);     //����200us��ʱ
    RTCInit();
    Init_I2C(250000);     //��ʼ��I2C����
    //Init_Timer1();        //��ʼ����ʱ��1�����ж����ŵ�����������
    Init_Lpc2478ADC();
    filter_init_lpc2478ad();
    //gp21Configuration();  //
    //GP21PIN_Init();
	//GP21_init();
    //Init_EINT2();
    //epm7064s Configuration();  //
    
    ///////////////////////
	InitModelIDVarible();       //�Ȱѻ����ͺŵĲ���������
	get_cal_para();
	
    Init_MachineVarible();
   
    
    #if (AD_IC == EPM7064S)
    Epm7064sPIN_Init();
    Motor_Pin_Init();
    filter_init();
	Init_EINT2();
	//adc_times = 448;   //Ĭ��
	#else
	cs5530_pin_init();
	if(1==cs5530_System_Reset())
	{
	     Set_ASC_Lib(Arial16B_Font);
         Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
	  Put_Str(30,200,Language_Str("CS5530 ERROR","CS5530 ERROR"));
	 while(1){;}
	 }
	#endif
	
	
  ////////////////////
	if(INNER_CAL_VERSION)
	 { 
	  //����λ
	  motor_err_flag = motor_run_proc(LOAD_POSITION);
	  for(i=0;i<100000;i++)
	  {;}
	  motor_err_flag = motor_run_proc(UNLOAD_POSITION);

	 }
	
	/////////////////////////////////����������ʾ
    Sleep_process();  
    //Key_Status = KEY_RELEASE ;
    ////////////////////////////////////////////////
    machine_mode =  SystemInit();    //�������洦��
  	 
    if(MACHINE_SERVICE_MODE == machine_mode)
    Start_Service_process();  
    else
    Normal_Operation();
   
    while (1)
    {;}
}
