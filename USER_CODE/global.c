//**********************************************************************
//�ļ���:global.c
//�����б�
//1  ��ʼ��ȫ�ֱ���  		    InitGlobalVarible 
//2  ��ʼ�����Ա���   		    InitLanguageVarible  
//3  ��ʼ�����ȱ���  		    InitBrightnessVarible
//4  ��ʼ��У׼��Ļ����   		InitScreenVarible
//5  ��ʼ�������̱���   		Init_Cal_Varible
//6  ��ʼ�� 0�����			    Init_Zer0_Weigh_Varible
//7  ����У׼���ݼ���     		Get_Linecal_Var
//8   ��ʼ������У׼����		Init_LineCal_Varible
//8a  ��ȡ��ǰ��¼�Ĵ��λ��    Search_Rec_Pos
//9   ��ʾ������ʱ��   		    Display_elapsed_time
//10  ����������ʾ  		    Display_Sleep_Area
//11  ����ʱ����ʾ   		    Display_Sleep_Time
//12  ����״̬     			    Sleep_process
//13  ����ϵͳ��ʼ��  			SystemInit

//14  ����XY,�����������ݽṹ   Create_ButtonList

//15 ���������־    		    Set_Zer0_Flag
//16 ����ȥƤ��־   		    Set_Tare_Flag
//17 ���òɼ��ٷֱȻ�׼��־		Set_Sam100_Flag
//18 �����ۼӱ�־        		Set_Totaladd_Flag
//19 ���òɼ�Ŀ��ֵ���ݱ�־   	Set_SamTarget_Flag

//**********************************************************************
#include "global.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "graph.h"
#include "ad7843.h"
#include "mainmenu.h"
#include "disp.h"
#include "Font_Lib.h"
#include "Font_Lib2.h"
#include "type_def.h"
#include "alarm_pro.h"

#include "epm7064s.h"

#include "rtc.h"
#include "port.h"


un_rec_data  inner_cal_real;
uint32      lcd_brightness;        //��ʾ���Աȶȵ�ֵ

uint32      Key_Status,key_value;  //���������Լ�״̬
struct      KeyStruct  Key[20];

uint32      sleepmode;
uint32      machine_mode;    //����������������״̬
uint32      current_mode,mode_run_flag;    //����������״̬
uint32      current_unit;    //������λ
uint32      stable_flag;     //�ȶ���־
uint32      power_on_flag,power_on_clr_cnt; //������־�뿪��ȥƤʱ��
uint32      elapsed_time;                   //������ʱ��¼
uint32      do_zer0_flag,do_tare_flag;
uint32      do_sam100_flag;
uint32      clr_data,zer0_data,full_code,new_data,new_data_big,tare_data; //У׼��㣬�������
//uint32      Error_Info_Flag,Warning_Info_Flag;

uint32      positive_flag;
uint32      asd_var,lnt_var,prt_mod;  //�����������ڲ�����
uint32      flag_new_data;            //������Ч��־
uint16      line_cal_para, user_cal_para;  //��?��???2?��y ?��?Y?����?3������???��

uint32      point10_cal_start,Point10_Cal_Buf[CAL_POINT_NUM+1],point10_cal_ok; //��ʵ��

uint32      offset_polar;
uint32      offset_50g,offset_100g,offset_150g;
uint32      expected_100g,expected_50g,expected_25g;
  
float       Point10_k[CAL_POINT_NUM-1];
float       percent_10_data;  //1/10��������

uint32      point2_cal_start,point2_cal_ok,point2_cal_step;

uint32      inner_code_step;
uint32      auto_zer0_valid;   //�Զ������Ƿ���Ч ��������У�����еĻ���
float       weigh_coef1;
float       tare_weight,gross_weight,net_weight;  //�������
////////////////////���²����Ǹ���ģʽ��ʹ�õ��ı���
float       weight_per_pcs;    //������
uint32      pcs_number;        //����                              
float       weight_compare_100,number_100;        //�ٷֱȵıȽϻ���
float       limit1_weight,limit2_weight;          //checkmode
float       differ_weight; 
float       total_weight;
uint32      total_times,do_totaladd_flag; 
uint32      animal_weight,animal_real_time,animal_time,animal_start,animal_ok; 
float       peak_weight;
uint32      peak_start,peak_end;
float       target_weight;
uint32      do_samtarget_flag;
float       density_temp,density_value,density_weight1,density_weight2;
uint32      density_start;
////////////////////////////wireless configuration
uint32      wireless_pin1_configuration,wireless_pin2_configuration;

float       temp_float,temp_cmp_point;
uint32      temp_cmp_flag;
////////////////////////////�����Զ������������
uint32      no_key_time;
////////////////////////////���������������ι���
uint8       speaker_volume,sound_switch;    //����������
uint32      which_para,password,model_id,user_id;                       //��������

////////////////////////////���Ӽ�¼����
uint32      rec_total_num,rec_real_num;  //��¼��ǰ�洢�ļ�¼λ�ã���ʼ����ʱ������
uint32      rec_total_pages,rec_curre_index;

/////////////////�����ͺſ�ѡ�� �������ı���
uint32      system_division,dot_position,inner_code_min,inner_code_max,adc_times;
float       additional_num,display_min_value,full_range,weight_alarm;
//������У׼
uint32      motor_run_time,motor_err_flag,point2_cal_flag,inner_cal_full,inner_cal_flag;

//�����û���У׼���ݣ����ڵ���ʹ��
uint32      user_inncal_data;
//add printer function
uint8      print_busy,Uart_Printer_Flag;
//���ӹ������£� ���ض������
uint8      logo_display_flag,model_display_flag;
uint8      repeat_check_flag,load_track_flag,adjust_0000_flag;
//���ӵ��������������
uint32     debug_data_flag,debug_disp_data1,debug_disp_data2;
///////////////////////////////////////////////////
//functions 
//////////////////////////////////////////////////	

//�����ͺŶ�Ӧ���ַ���ʾ
float const machine_alarm[3] = 
 {
  0.090,  0.0090, 0.00090
 };

//�����ͺŶ�Ӧ��������
//����Ĭ��С���� 3λ ���� 4λ ��������
//        С���� 5λ ������������ϳ���10

/////////////////////////////////////////
//�����ͺŶ�Ӧ�ķֱ���
//����Ĭ��С���� 4λ ���� 5λ �ķֱ��ʣ
//        С���� 3λ ������������ϳ���10
uint32 const machine_division[] = {
1000,
10000,
100000
};
/////////////////////////////////////////
//�����ͺŶ�Ӧ��ADC ת���ٶ�
//����Ĭ��С���� 4λ ���� 5λ �ķֱ��ʣ
//        С���� 3λ ������������ϳ���10
uint32 const machine_speed[] = {
112*3,
112*4,
112*4
};

//�����ͺŶ�Ӧ����������
float const machine_4s5r[] = {
0.0004,        //С���������λʱ���4��5��ֵ
0.00004,
0.000004
};
//�����ͺŶ�Ӧ����С��ʾ��ֵ
float const machine_display_min[] = {
0.004,       
0.0007,
0.00009
};

//////////////////////////////////////////////////
//����˶����̣�����ΪĿ��λ��
//��ʱ�򷵻ش���
///////////////////////////////////////////////////

uint32  motor_run_proc(uint8 target_pos)
{
 uint8 tmp;
  if(target_pos == MOTOR_POSITION)
     return(0);
  else   
    {
     motor_run_time = 0;
     FIO0CLR = EPM7_MOTOR_P1;FIO0SET = EPM7_MOTOR_P2;
     while(1)
        {           
         tmp =  MOTOR_POSITION;
         if(1 == tmp)
           FIO2SET = EO2_LED_CPU_S;
         else
           FIO2CLR = EO2_LED_CPU_S;  
         if((motor_run_time==400)||(tmp==target_pos))
            {
             FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;
             break;
            } 
        }
     /////////////////////////
     if(motor_run_time==400)  	       
     {
      return(1);
     }
     else
     {
      return(0);
     }
    }  
}
//////////////////////////////////////////////////
//1
//������ʼ�� ����ȫ�ֱ���
///////////////////////////////////////////////////
void InitGlobalVarible(void)
	{
	 //Key_Status = KEY_RELEASE ;   //�Ƶ������г�ʼ��
	 ////////////////////////////added after
	 sleepmode         = FALSE;
	 stable_flag       = FALSE;
	 power_on_flag     = TRUE;
	 point2_cal_start  = FALSE;
	 point2_cal_step   = 0;
	 point10_cal_start = FALSE;
	 power_on_clr_cnt  = POWER_ON_STABLE_TIMES; //�����������ȶ���5���������㴦��
     current_mode      = STAT_WEIGH;
     current_unit      = UNIT_G;
     //////////////////////////////////
     do_zer0_flag      = FALSE;
     do_tare_flag      = FALSE;
     do_sam100_flag    = FALSE;
     tare_data         = 0;
     weight_per_pcs    = 0.0;
     weight_compare_100= 0.0;
     ////////////////////////////////
     no_key_time = 0;
     
     temp_cmp_flag = FALSE;
     
    
	}
 //////////////////////////////////////////////////
 //2
 //������ȡ��������
 ///////////////////////////////////////////////////
uint8  InitLanguageVarible(void)
	{
	 uint32 i;
	 uint8  LangCalData[4];
	 
	 Read_EEPROM(EEP_LANGUAGE_ADDR, LangCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 //Set_Language_Type(ENGLISH);
	 //return(OK);
	 
	 if(LangCalData[3] != LangCalData[0] + REC_CHECK_DATA1)
	   {
	    Set_Language_Type(CHINESE);
	    return(ERROR);
	   } 
	  else
	   {
	    Set_Language_Type(LangCalData[0]);
	    
	    return(OK);
	   }
	   
	 } 	
 //////////////////////////////////////////////////
 //3a
 //������ȡ��������
 ///////////////////////////////////////////////////
uint8  InitSpeakerVarible(void)
	{
	 uint32 i,j;
	 uint8  SpeakerData[4];
	 
	 Read_EEPROM(EEP_SPEAK_ADDR, SpeakerData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 //����Ϊ�����Ŀ���
	 //У��
	 i = (SpeakerData[1]<<8) + SpeakerData[0];
	 j = (SpeakerData[3]<<8) + SpeakerData[2];
	 if((i+REC_CHECK_DATA1)==j)
	  {
	   sound_switch = i;
	   return(OK);
	  }
	 else
	  {
	   sound_switch = ON;  //Ĭ�Ͽ���
	   return(ERROR);
	  } 
	 /*
	 if((SpeakerData[3]==SpeakerData[0]+REC_CHECK_DATA1)&&(SpeakerData[0]>0)&&(SpeakerData[0]<= SPEAK_CLASS_TOTAL))
	   {
	    speaker_volume = SpeakerData[0];  
	    Set_Speaker_Volume(speaker_volume);
	    return(OK);
	   } 
	  else
	   {
	    speaker_volume = DEFAULT_SPEAKER_VOLUME;          //default 
	    Set_Speaker_Volume(speaker_volume);
	    return(ERROR);
	   }*/
	   
	 } 		
	 
//////////////////////////////////////////////////
 //3a
 //������ȡ����
 ///////////////////////////////////////////////////	 
uint8  InitPasswordVarible(void)
{
	 uint32 i,j;
	 uint8  SpeakerData[4];
	 
	 Read_EEPROM(EEP_PASSWORD_ADDR, SpeakerData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 //У��
	 i = (SpeakerData[1]<<8) + SpeakerData[0];
	 j = (SpeakerData[3]<<8) + SpeakerData[2];
	 if((i+REC_CHECK_DATA1)==j)
	  {
	    password = i;
	    if(password>9999)
	       password = PASSWORD_DEFAULT;
	    return(OK);
	  }
	 else
	  {
	   password = PASSWORD_DEFAULT;
	   return(ERROR);
	  } 
} 		
	 	 
//////////////////////////////////////////////////
 //3a
 //������ȡ�û� ID
 ///////////////////////////////////////////////////	 
uint8  InitUserIDVarible(void)
{
	 uint32 i,j;
	 uint8  SpeakerData[4];
	 
	 Read_EEPROM(EEP_USER_ID_ADDR, SpeakerData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 //У��
	 i = (SpeakerData[1]<<8) + SpeakerData[0];
	 j = (SpeakerData[3]<<8) + SpeakerData[2];
	 if((i+REC_CHECK_DATA1)==j)
	  {
	    user_id = i;
	    if(user_id>999)
	       user_id = USER_ID_DEFAULT;
	    return(OK);
	  }
	 else
	  {
	   user_id = USER_ID_DEFAULT;
	   return(ERROR);
	  } 
} 	

///////////////////////////////////////////////////
//����˫���� �л�
//////////////////////////////	 
void Set_Relay_Oper(void)
     {
      //����С������������л�
      if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
        {
         FIO0SET = RELAY_OUT_PIN;
        }
      else
        {
         FIO0CLR = RELAY_OUT_PIN;
        } 
      }	
//////////////////////////////////////////////////
 //3a
 //������ȡ�����ͺ�
 ///////////////////////////////////////////////////	 
uint8  InitModelIDVarible(void)
{
	 uint32 i,j;
	 uint8  buf[8];
	
 	 ///////////////�˴���ȡ���еĹ�������
 	 Read_EEPROM(EEP_FACT_PARA_ADDR, buf, 8);
     for(i=0;i<80000;i++)	{;}	             //delay
    
     if(buf[7] == (buf[0]+buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6]+REC_CHECK_DATA1))
      {
       logo_display_flag    = buf[0]&0x01;
       model_display_flag   = buf[1]&0x01;
       adjust_0000_flag     = buf[2]&0x01;
       load_track_flag      = buf[3]&0x01;
       repeat_check_flag    = buf[4]&0x01;
       Uart_Printer_Flag    = buf[5]&0x01;
       debug_data_flag      = buf[6]&0x01;   
	  }
     else
      { 
       logo_display_flag    = 1;
       model_display_flag   = 1;
       adjust_0000_flag     = 1;
       load_track_flag      = 0;
       repeat_check_flag    = 0;
       Uart_Printer_Flag    = 1;
       debug_data_flag      = 0;
	   } 
	 
	 
	 for(i=0;i<8;i++)
	    buf[i] = 0x00;
	 Read_EEPROM(EEP_MODEL_ID_ADDR, buf, 4);
	 for(i=0;i<80000;i++){;}   //delay
	 //У��
	 if(buf[3]==(buf[0]+buf[1]+buf[2]))
	   {
	        model_id = (buf[3]<<24)+(buf[2]<<16)+(buf[1]<<8)+buf[0];
	        return(OK);
	    }
	 else
	   {
	        model_id = MODEL_ID_DEFAULT;
	        return(ERROR);
	   }
}

///////////////////////////////////////

void  get_cal_para(void)
{
 uint16  i;
 i = model_id&0xffff;
 if(i<150)
 {
  line_cal_para = 100;
  user_cal_para = 100;
 }
 else if(i<250)
 {
  line_cal_para = 200;
  user_cal_para = 200;
 }
 else if(i<350)
 {
  line_cal_para = 300;
  user_cal_para = 300;
 }
 else if(i<450)
 {
  line_cal_para = 400;
  user_cal_para = 400;
 }
 else 
 {
  line_cal_para = 500;
  user_cal_para = 500;
 }
 
}

///////////////////////////////////////////////
//���ݻ����ͺ� ��ʼ��ϵͳ���� ������
// �ֱ���
// ����������
// ������������С��ֵ
// ���������С��ֵ
// 
// ����������ֵ
// ��С��ʾ��ֵ
/////////////////////////////////////////////// 
void  Init_MachineVarible(void)
{	 
  dot_position = (model_id>>16)&0xff;
  Set_Relay_Oper();
  
  adc_times          = machine_speed[dot_position];
  additional_num     = machine_4s5r[dot_position];
  display_min_value  = machine_display_min[dot_position];
  weight_alarm       = (model_id&0xffff) + machine_alarm[dot_position];  
             
  switch(dot_position)
   {
    case WAN10_DIVISION:
           full_range        = (model_id & 0xffff) / 10;
           system_division   = full_range * machine_division[dot_position];
           break;
    default:
           full_range        = model_id & 0xffff;
           system_division   = full_range * machine_division[dot_position];
           break;
   }
   
   inner_code_min    = system_division*INNER_CODE_STEP_DEF/2;
   inner_code_max    = system_division*INNER_CODE_STEP_DEF*3;   
} 
		 
 //////////////////////////////////////////////////
 //3
 //������ȡ��Ļ��������
 ///////////////////////////////////////////////////
uint8  InitBrightnessVarible(void)
	{
	 uint32 i;
	 uint8  BriCalData[4];
	 
	 Read_EEPROM(EEP_BRIGHT_ADDR, BriCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 if((BriCalData[3] == BriCalData[0] + REC_CHECK_DATA1)&&(BriCalData[0]<=BRIGHT_CLASS_TOTAL))
	   {
	    lcd_brightness = BriCalData[0];  
	    Set_LCD_Brightness(lcd_brightness);
	    return(OK);
	   } 
	  else
	   {
	    lcd_brightness = DEFAULT_BRIGHTNESS;          //default 
	    Set_LCD_Brightness(lcd_brightness);
	    return(ERROR);
	   }
	 } 		
 //////////////////////////////////////////////////
 //3a
 //������ȡ�������õ�����
 ///////////////////////////////////////////////////
uint8  InitWirelessVarible(void)
	{
	 uint32 i;
	 uint8  WirelessData[4];
	 
	 Read_EEPROM(EEP_WIRELESS_ADDR, WirelessData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 //��־0x55aa is OK, ����������ֽ����ݿ����ж�ΪOK
	 if((WirelessData[3]==REC_CHECK_DATA2)&&(WirelessData[2]==REC_CHECK_DATA1)&&(WirelessData[1]<FUNCTION_END)&&(WirelessData[0]<FUNCTION_END))
	   {
	    wireless_pin1_configuration = WirelessData[0];
	    wireless_pin2_configuration = WirelessData[1];
	    return(OK);
	   } 
	  else
	   {
	    wireless_pin1_configuration = FUNCTION_NULL;          //default 
	    wireless_pin1_configuration = FUNCTION_NULL;
	    return(ERROR);
	   }
	  
	 } 
//////////////////////////////////////////////////
 //3a
 //������ȡ��У׼����
 ///////////////////////////////////////////////////	 	
uint8  InitInnerCalVarible(void)
	{
	 uint32 i;
	 uint8  buf[8];
	 
	 un_rec_data tmp;
	 
	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
	    Read_EEPROM(EEP_INNER_CAL2_ADDR, buf, 8);
	 else
	 	Read_EEPROM(EEP_INNER_CAL1_ADDR, buf, 8);
	 	
	 for(i=0;i<1000;i++){;}   //delay
	 //
	  tmp.rec_uint32  = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	
	 //�ж���������
	 if((REC_CHECK_DATA1==buf[4])&&(REC_CHECK_DATA2==buf[5])&&(tmp.rec_float>90)&&(tmp.rec_float<110))
	   {
        //inner_cal_full = i;
        inner_cal_flag = OK; 
	    inner_cal_real = tmp; 
	    return(OK);
	   } 
	 else 
	   {
        inner_cal_flag = FALSE; 
	    inner_cal_real.rec_float = 108.1234; 
	    return(FALSE);
	   } 
	   
	} 		 	  
 //////////////////////////////////////////////////
 //4
 //������ȡ��ĻУ׼���ݣ�������̫����ʹ��Ĭ������
 ///////////////////////////////////////////////////
uint8  InitScreenVarible(void)
	{
	 uint32 i;
	 uint16 x00,y00,xmid0,ymid0,xmax0,ymax0;
	 uint8  ScreenCalData[13];
	 uint8  CheckSum;
	 
	 Read_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);
	 for(i=0;i<1000;i++){;}   //delay
	 CheckSum = 0;
	 for(i=0;i<12;i++)
	 CheckSum += ScreenCalData[i];
	 x00   = (ScreenCalData[0] << 8)  + ScreenCalData[1];
     y00   = (ScreenCalData[2] << 8)  + ScreenCalData[3];
	 xmid0 = (ScreenCalData[4] << 8)  + ScreenCalData[5];
	 ymid0 = (ScreenCalData[6] << 8)  + ScreenCalData[7];
	 xmax0 = (ScreenCalData[8] << 8)  + ScreenCalData[9];
	 ymax0 = (ScreenCalData[10] << 8) + ScreenCalData[11];
	 if((CheckSum != ScreenCalData[12])||Set_Originalx0y0(x00,y00)||Set_XmidYmid(xmid0,ymid0)||Set_XmaxYmax(xmax0,ymax0)) 
      {
	  x0 =   X0_DEFAULT;
	  y0 =   Y0_DEFAULT;
	  xmid = XMID_DEFAULT;
	  ymid = YMID_DEFAULT;
	  xmax = XMAX_DEFAULT;
	  ymax = YMAX_DEFAULT;
	  return(ERROR);                 //error
   	  }
	  else
	  return(OK);
	}
 //////////////////////////////////////////////////
 //5
 //������ȡzhongliangУ׼����
  ///////////////////////////////////////////////////
uint8  Init_Cal_Varible(void)
	{
	 uint32 i,full_bak;
	 uint8  WeighCalData[12];
	 
	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position)) //�����˫�ֶ� ������Ϊʮ���֮һ
	    Read_EEPROM(EEP_WEIFULL2_ADDR, WeighCalData, 12);
	 else
	    Read_EEPROM(EEP_WEIFULL1_ADDR, WeighCalData, 12);
	
	 for(i=0;i<1000;i++){;}   //delay
	 full_code = (WeighCalData[0]<<24) + (WeighCalData[1]<<16) + (WeighCalData[2]<<8) + WeighCalData[3];
	 full_bak  = (WeighCalData[4]<<24) + (WeighCalData[5]<<16) + (WeighCalData[6]<<8) + WeighCalData[7];
	 //���û���У������Ҳ������
	 user_inncal_data  = (WeighCalData[8]<<24) + (WeighCalData[9]<<16) + (WeighCalData[10]<<8) + WeighCalData[11];
	 
	 //������Ч������1      
	 if(full_code==full_bak) //&&(full_code>inner_code_min)&&(full_code<inner_code_max))
	 //if(full_code == full_bak)
	 {
   	  inner_code_step = full_code/system_division;
   	  weigh_coef1     = full_range / full_code;
   	  point2_cal_ok   = TRUE;
	  return(OK);
	  }
	  else if((full_bak>inner_code_min)&&(full_bak<inner_code_max)) 
	  {//������Ч������2
	  full_code       = full_bak;
	  inner_code_step = full_bak/system_division;
   	  weigh_coef1     = full_range / full_bak;
   	  point2_cal_ok   = TRUE;
	  if((inner_code_step > INNER_CODE_STEP_MAX)||(inner_code_step < INNER_CODE_STEP_MIN))      
   	  inner_code_step = INNER_CODE_STEP_DEF;
	  return(OK);
	  }
	  else if(QIAN_DIVISION == dot_position)
	  {
	   full_code       = 200000 * INNER_CODE_STEP_MAX;
	   inner_code_step = INNER_CODE_STEP_DEF * 5;
	   weigh_coef1     = 200.0/full_code;    //Ĭ�ϵ�ϵ��
	   point2_cal_ok   = FALSE;
	   return(ERROR);                        //error
   	  }
   	  else
   	  {
	   full_code       = 2000000 * INNER_CODE_STEP_DEF;
	   inner_code_step = INNER_CODE_STEP_DEF;
	   weigh_coef1     = 200.0/full_code;    //Ĭ�ϵ�ϵ��
	   point2_cal_ok   = FALSE;
	   return(ERROR);                 //error
   	  }
   	  
    }	
 //////////////////////////////////////////////////
 //6 
 //������ȡ lingdian ����У׼���� ���
  ///////////////////////////////////////////////////
uint8  Init_Zer0_Weigh1_Varible(void)
	{
	 uint32 i,zer0_cal,zer0_cal_bak;
	 uint8  WeighCalData[16];
	 
	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position)) //�����˫�ֶ� ������Ϊʮ���֮һ
	    Read_EEPROM(EEP_WEIZERO_L2_ADDR, WeighCalData,16);
	 else
	    Read_EEPROM(EEP_WEIZERO_L1_ADDR, WeighCalData,16);
	 for(i=0;i<1000;i++){;}   //delay
	 zer0_cal     = (WeighCalData[0]<<24) + (WeighCalData[1]<<16) + (WeighCalData[2]<<8) + WeighCalData[3];
	 zer0_cal_bak = (WeighCalData[4]<<24) + (WeighCalData[5]<<16) + (WeighCalData[6]<<8) + WeighCalData[7];
	 
	 //�ж���������
	 if((zer0_cal==zer0_cal_bak)&&(zer0_cal>USER_ZER0_CODE_MIN)&&(zer0_cal<USER_ZER0_CODE_MAX))
	  {
   	   clr_data  = zer0_cal;       //����У׼���
   	  }
   	 else if((zer0_cal_bak>USER_ZER0_CODE_MIN)&&(zer0_cal_bak<USER_ZER0_CODE_MAX))
   	  {
   	   clr_data = zer0_cal_bak;    //����У׼���
   	  }
   	 else 
   	   clr_data = 0;
   	  /////////////////////////////////////////// 
   	   if(clr_data==0)  
   	   {
	    return(ERROR);                 //error
   	   }
   	 else   
   	   {
   	    return(OK);
	   }  
	}   
   	  
   	  
  //////////////////////////////////////////////////
 //6
 //������ȡ lingdian У׼���� �û�У׼ ���
  ///////////////////////////////////////////////////
uint8  Init_Zer0_Weigh2_Varible(void)
	{
	 uint32 i,zer0_cal,zer0_cal_bak;
	 uint8  WeighCalData[16];  	  
   	  ///////////////////////////////////////////////////////////////////////////////////////////////
   	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position)) //�����˫�ֶ� ������Ϊʮ���֮һ
	    Read_EEPROM(EEP_WEIZERO_U2_ADDR, WeighCalData,16);
	 else
	    Read_EEPROM(EEP_WEIZERO_U1_ADDR, WeighCalData,16);
	 for(i=0;i<1000;i++){;}   //delay
   	 zer0_cal     = (WeighCalData[0]<<24) + (WeighCalData[1]<<16) + (WeighCalData[2]<<8) + WeighCalData[3];
	 zer0_cal_bak = (WeighCalData[4]<<24) + (WeighCalData[5]<<16) + (WeighCalData[6]<<8) + WeighCalData[7];
	 //�ж���������
	 if((zer0_cal==zer0_cal_bak)&&(zer0_cal>USER_ZER0_CODE_MIN)&&(zer0_cal<USER_ZER0_CODE_MAX))
	  {
   	   zer0_data  = zer0_cal;       //�û�У׼���
   	  }
   	 else if((zer0_cal_bak>USER_ZER0_CODE_MIN)&&(zer0_cal_bak<USER_ZER0_CODE_MAX))
   	  {
   	   zer0_data = zer0_cal_bak;    //�û�У׼���
   	  }
   	 else if((clr_data>USER_ZER0_CODE_MIN)&&(clr_data<USER_ZER0_CODE_MAX))
   	   zer0_data = clr_data;
   	 else 
   	   zer0_data = 0;  
   	 ////////////////////////////////////////////////////////  
   	 if(zer0_data==0)  
   	   {
   	    point2_cal_ok = FALSE;
	    return(ERROR);                 //error
   	   }
   	 else   
   	   {
   	    point2_cal_ok = TRUE;
	    return(OK);
	   } 
    }	   

 //////////////////////////////////////////////////
 //7
 //���¼������Ա���
 ///////////////////////////////////////////////////
void   Get_Linecal_Var_4(void)
    {
     uint32 i,j;
     // [1] [2]  [3]  [4]
     // 50  100  150  200
     expected_100g = Point10_Cal_Buf[4]/2;
     expected_50g  = expected_100g/2;
     expected_25g  = expected_50g/2;
     
     if(expected_100g > Point10_Cal_Buf[2]) //������100�˱Ƚ�
        offset_polar = 0;             //�Ӵ���
     else
        offset_polar = 1;             //������
     
     offset_100g = abs(expected_100g-Point10_Cal_Buf[2]); //�õ�100����Ҫ��������
     ///////////////////////////////////////////////////////�õ� 50����Ҫ��������
     if(expected_50g > Point10_Cal_Buf[1])
       {
         if(0 == offset_polar)
           {
             i = expected_50g-Point10_Cal_Buf[1];
             if(i> offset_100g/2)
                offset_50g = i-offset_100g/2;
             else
                offset_50g = 0;  
           }
         else 
           offset_50g = 0;
         
       }
     else
       {
         if(1 == offset_polar)
           {
             i = Point10_Cal_Buf[1] - expected_50g;
             if(i> offset_100g/2)
                offset_50g = i-offset_100g/2;
             else
                offset_50g = 0;  
           }
         else 
           offset_50g = 0; 
       }  
     ///////////////////////////////////////////////////////�õ�150����Ҫ��������
     j = expected_100g+expected_50g;
     
     if(j > Point10_Cal_Buf[3])
       {
         if(0 == offset_polar)
           {
             i = j - Point10_Cal_Buf[3];
             if(i> offset_100g/2)
                offset_150g = i-offset_100g/2;
             else
                offset_150g = 0;  
           }
         else 
           offset_150g = 0;
         
       }
     else
       {
         if(1 == offset_polar)
           {
             i = Point10_Cal_Buf[3] - j;
             if(i> offset_100g/2)
                offset_150g = i-offset_100g/2;
             else
                offset_150g = 0;  
           }
         else 
           offset_150g = 0; 
       }  
	}	
	
////////////////////////////////////////////////////////////
// ���� ������ֱ ����
////////////////////////////////////////////////////////////	
uint32  Get_Line_data(uint32 weight1)
{  
  uint32 i,j,k,weight2;
  float  offset_tmp[3];  
  float  offset_temp;
  
  if(weight1 < clr_data)
    return(weight1);
    
  weight2 = weight1 - clr_data;
  
  if(weight2 > Point10_Cal_Buf[4])  //������Χ
    {
      i = weight2 - Point10_Cal_Buf[4];
      offset_temp = i*(offset_100g/(expected_50g+1.01));
      if(0 == offset_polar)       
       return(weight1-(uint32)offset_temp); 
      else
       return(weight1+(uint32)offset_temp);
    }
  else
    { //����ƫ���� �ۼ�
      offset_tmp[0] = 0.0;
      offset_tmp[1] = 0.0;
      offset_tmp[2] = 0.0;
      for(i=0;i<3;i++)
      {
        if(i==0)
         {
          j = abs(expected_100g-abs(expected_100g-weight2));
          offset_tmp[0] = j*(offset_100g/(expected_100g+1.01)); 
          k = abs(expected_100g-weight2);
         }
        if(i==1)
         {
          j = abs(expected_50g-abs(expected_50g-k));
          if(weight2 > expected_100g)
          offset_temp = j*(offset_150g/(expected_50g+1.01)); 
          else
          offset_temp = j*(offset_50g/(expected_50g+1.01));
          offset_tmp[1] = offset_temp;  
          k = abs(expected_50g-k);  
         }    
        if(i==2)
         {
          j = abs(expected_25g-abs(expected_25g-k));
          if(weight2 > expected_100g)
          offset_temp = j*(offset_150g/(expected_50g+1.01)); 
          else
          offset_temp = j*(offset_50g/(expected_50g+1.01));
          offset_tmp[2] = offset_temp/2;
          //offset_tmp[2] = 0;  
         }
      }  //ƫ�����������  
   
      k = (uint32)(offset_tmp[0]+offset_tmp[1]+offset_tmp[2]);
     
      if(0 == offset_polar)       
        return(weight1+k); 
      else
       {
        return(weight1-k);
         
       }
    } 
}
/////////////////////////////////////////////////
//3 ��������У׼�������
//  ���û������У׼���������ݲ�������У׼�ķ�Χ�ڣ�����
//  ԭʼ����
//  weight1 ��0�������
//  weight2 ��������
/////////////////////////////////////////////////
uint32  Get_Line_data_10(uint32 data_no_line)
{  
    uint32 where_k,data_offset;
    uint32 pure_wei_code;
    ///////////////�̨���????��??
    if(data_no_line < clr_data)  //����?��0??��??? 2?��|����
      return(data_no_line);
    /////////////////////��???��??��??  
    pure_wei_code = data_no_line - clr_data;
    
    if(pure_wei_code > Point10_Cal_Buf[10])   //
     {     //////////////////�̨�?t???��??��?��1��?��?o����???D��?��
      data_offset = pure_wei_code - Point10_Cal_Buf[10]; 
      data_offset = data_offset * Point10_k[9];
      return((uint32)(clr_data + 10*percent_10_data + data_offset));
     }
    ////////////////////////////////////////?y3��???? ??????��?��?
    for(where_k=1;where_k<11;where_k++)    //1 2��?��????
    {                                         
     if(pure_wei_code < Point10_Cal_Buf[where_k])
     	break;
     }
    /////////////////////////////////////////////2 3?3??3??D���?��?��?��y?Y
    data_offset = pure_wei_code - Point10_Cal_Buf[where_k-1]; //
    /////////////////////////////////////////////3 ��-??��?D���?��?
    data_offset = data_offset * Point10_k[where_k-1]; 
    return((uint32)(clr_data + (where_k-1)*percent_10_data + data_offset));
}	

 //////////////////////////////////////////////////
 //7
 //���¼������Ա���
 ///////////////////////////////////////////////////
void   Get_Linecal10_para(uint32 full_data)
    {
     uint32 i;
     if(TRUE == point10_cal_ok)  //���������У׼����
      {
       percent_10_data =  full_data / 10;  //����10�ȷ�
       for(i=0;i<10;i++)
	    Point10_k[i] = percent_10_data/(Point10_Cal_Buf[i+1]-Point10_Cal_Buf[i]); //����ϵ�� �õ�ƫ����
	  }
	  else
	  {
	   percent_10_data = 0.0;
	   for(i=0;i<10;i++)
	    Point10_k[i] = 0.0;
	  }   
	  
	}
	

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//8
//������ȡ����У׼����
///////////////////////////////////////////////////	     
///////////////////////////////////////////////////////
uint8  Init_LineCal_Varible(void)
	{
	 uint32 i,j;
	 uint32 checksum;
	 uint8  buf[16];
	 
	 for(i=0;i<3;i++)
	 {
	  if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
	     Read_EEPROM(EEP_LINCAL2_ADDR01+i*16, buf, 16);
	  else
	     Read_EEPROM(EEP_LINCAL1_ADDR01+i*16, buf, 16);
	  
	  for(j=0;j<1000;j++){;}   //delay
	  for(j=0;j<4;j++)
	  Point10_Cal_Buf[4*i+j] = (buf[4*j+0]<<24) + (buf[4*j+1]<<16) + (buf[4*j+2]<<8) + buf[4*j+3];
	 }
	  //////////////check2
	  checksum = 0;
	  for(j=0;j<11;j++)
	  checksum += Point10_Cal_Buf[j];
	   
	  ///////////////////////////////////////////////////
	  if(checksum == Point10_Cal_Buf[11])
	  {//
	   point10_cal_ok = TRUE;
	   Get_Linecal10_para(Point10_Cal_Buf[10]);
	   return(OK);
	  }
	  else
	  {//????
	   point10_cal_ok = FALSE;
	   return(ERROR);
	  }
   }	    
//////////////////////////////////////////////////
//8a
//�������ҵ�ǰ��¼�Ĵ��λ��
//ÿ����¼ռ��16���ֽ�

///////////////////////////////////////////////////	     
void  Search_Rec_Pos(void)
     {
       uint32 i,j;
 	   uint8  BufferData[8];
	 
       Read_EEPROM(EEP_REC_TOTAL_ADDR,BufferData,8);
	   for(j=0;j<1000;j++){;}   //delay
	   
	   i = (BufferData[0]<<24)+(BufferData[1]<<16)+(BufferData[2]<<8)+BufferData[3];
	   j = (BufferData[4]<<24)+(BufferData[5]<<16)+(BufferData[6]<<8)+BufferData[7];
	   
	   if((0xffffffff==i)&&(0xffffffff==j)||(i!=j))
	   {//��оƬ
	    for(i=0;i<8;i++)
	       BufferData[i] = 0;
	    Write_EEPROM(EEP_REC_TOTAL_ADDR,BufferData, 8); //��ʼ��Ϊ0
		for(j=0;j<40000;j++)	{;}	                 //delay
		//Ĭ��״̬
		rec_total_num = 0;  //�ۼ����ݶ��٣�����ṩ��Ѱַ
		rec_real_num  = 0;  //��Ч�����ж��٣����ֻ����ϵͳ�ɴ洢��Ŀ
		rec_curre_index = 0;
		rec_total_pages = 1;
	   }
	   ////////////////////////////////////////////���
	   else if(i==j)
	   {                        //��ȷ,���ص�ǰ������ӵ�
	    rec_total_num = i;      //��ǰ�ܼ�¼��
	   
	    if(i < RECNUM_PER_PAGE) //��ǰ��¼��С��һҳ
	     {
	      rec_real_num    = i;
	      rec_curre_index = i;
		  rec_total_pages = 1;
	     }
	    else if(i < REC_MAX_NUM) //��ǰ��¼����δ���ڸ���״̬
	     {
	      rec_real_num    = i;
	      rec_curre_index = i;
	      rec_total_pages = i/RECNUM_PER_PAGE + 1; //�ж�ҳ
	     } 
	    else
	     { //��ǰ��¼���Ѿ����ڸ���ģʽ
	      rec_real_num    = REC_MAX_NUM;  //��Ч���̶�Ϊ�����
	      rec_curre_index = i%REC_MAX_NUM;  
	      rec_total_pages = REC_MAX_NUM/RECNUM_PER_PAGE; //ҳ���̶�
	     }
	   }
	  /* else
	   {//�쳣������Ϊ������
	    rec_total_num = 0;
		rec_curre_index = 0;
		rec_total_pages = 1;
	   }*/
    }   
///////////////////////////////////////////////////////////
//9      ��ʾ�Ѻķ�ʱ��
///////////////////////////////////////////////////////////
void  Display_elapsed_time(uint32 time_sec)
   {  
       uint32 i,hour,mint,sec;
       //time_sec = 3600* 100+ time_sec;
       hour = 0;
       mint = 0;
       sec  = 0;        
       if(time_sec>3599)
         {
          hour = time_sec/3600;
          time_sec = time_sec%3600;
          }
       if(time_sec>59)
         {
          mint = time_sec/60;
          time_sec = time_sec%60;
          }        
          sec = time_sec;
         
         if(hour>99)
         Put_Data(515-16,     30,"%3d", hour);
         else   
         Put_Data(515,        30,"%02d", hour);
	    
	     Put_Char(485+4*15+3, 30,0x3a);
         Put_Data(560,        30,"%02d", mint);
	     Put_Char(485+7*15+3, 30,0x3a);
	     Put_Data(605,        30,"%02d", sec); 
     }	     
	    
//////////////////////////////////////////////////
//10  ����������ʾ
///////////////////////////////////////////////////
void  Display_Sleep_Area(void)
	{
      Clr_Bar(0,0,640,480,COL_BLACK);
      /*Put_Pixel(0,0,COL_WHITE);
      Put_Pixel(638,478,COL_WHITE);
      Put_Pixel(639,479,COL_WHITE);
      Put_Pixel(640,480,COL_WHITE);
      while(1){;}*/
      //Put_Bmp(0,0,logo22560,0);       //display LOGO
	  if(ENGLISH == Get_Language_Type())
	     Put_Bmp(520,410,start11060e,0);    //display START
      else
         Put_Bmp(520,410,start11060c,0);    //display START
      
      Set_ASC_Lib(Arial16B_Font);
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      Put_Str(Language_Pos(400,370), 5,Language_Str("ϵͳʱ��"," System Time"));
      Put_Str(Language_Pos(400,370),30,Language_Str("����ʱ��","Elapsed Time"));
     }

///////////////////////////////////////////////////////////
//11
//    ����ʱ����ʾ
///////////////////////////////////////////////////////////
void Display_Sleep_Time(void)
  { 
   RTCTime local_time_buf;
   if(Sec_Change_Flag)
        {
         Sec_Change_Flag = FALSE;
      
         local_time_buf = RTCGetTime();
         Set_ASC_Lib(Arial20B_Font);
         Set_Chr_Color(COL_WHITE,COL_BLACK);
         //display time
	     Put_Data(515,        5,"%02d", local_time_buf.RTC_Hour);
	     Put_Char(485+4*15+3, 5,0x3a);
         Put_Data(560,        5,"%02d", local_time_buf.RTC_Min);
	     Put_Char(485+7*15+3, 5,0x3a);
	     Put_Data(605,        5,"%02d", local_time_buf.RTC_Sec);
	     Display_elapsed_time(elapsed_time);
        } 
   }  
///////////////////////////////////////////////////////////
//12
//    ����Ԥ��ʱ�䲻�����������ģʽ
///////////////////////////////////////////////////////////      
void  Sleep_process(void)
    {
      
      uint32 i,X_position,Y_position,Quit_flag;
      
      Quit_flag = FALSE;
         
      InitScreenVarible();         //����ĻУ׼������ǰ������
      InitLanguageVarible();       //�����Բ�����ǰ������ 
      InitBrightnessVarible();     //��Ļ����
      InitSpeakerVarible();        //����
      InitPasswordVarible();       //�Ѵ�������������
      
      Key_Status = KEY_RELEASE+KEY_DO_OVER ;   //key release
      elapsed_time = 0;            //
         
      Display_Sleep_Area();  
        
      do
      {
       ///////////////////////////////keyscan
       for(i=0;i<1000;i++){;}   //delay
       key_value = Get_PositionXY();   //����ɨ��
       if(Key_Status == KEY_DO_START)
       {
	     X_position = (key_value>>16)&0x0000ffff;
	     Y_position =  key_value & 0x0000ffff;
	     if((X_position>520)&&(X_position<630)&&(Y_position>410)&&(Y_position<470))
	         Quit_flag = 0x55aa;
	      else
	         Quit_flag = FALSE;
	     Key_Status = KEY_DO_OVER;  
        }
       /////////////////////////////display time
       Display_Sleep_Time();
      
       ////////////////////////////////////////////
       if((0x55aa == Quit_flag)&&(elapsed_time > PREHOT_TIME)) //Ԥ��ʱ�����30����
           Quit_flag = TRUE;
       /////////////////////////////////////////////////////////////////////
       if((0x55aa == Quit_flag)&&(elapsed_time < PREHOT_TIME)) //Ԥ��ʱ��С��30����
        {
         ////////////////////////����PCS����״̬�ɼ���������
         PCSInput_Interface();
         Set_ASC_Lib(Arial16B_Font);
         Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
         Put_Str(Language_Pos(230-100,200-100),85,Language_Str("ϵͳԤ��ʱ�䲻�� �����뿪������","Not Enough Prehot,Please input Password")); 
       
         do
         {
           i = PCSInput_Key_Pro(Get_PositionXY(),PASSWORD_INPUT_ID); //
               PCSInput_Disp_Pro();
           if((i==password)||(i==PASSWORD_DEFAULT))
             {
              Quit_flag = TRUE;  
             }
             else if(i != PASSWORD_INPUT_ID) //������Ч������ˢ������
              PCSInput_Interface(); 
          } 
          while(TRUE !=Quit_flag);
         }
      }   
      while(FALSE == Quit_flag);
    }
 
//////////////////////////////////////////////////
//13 ������ʼ������ȫ�ֱ���
//return 
///////////////////////////////////////////////////
uint8  SystemInit(void)
{   
    uint32 key_times=0;
    uint32 key_buf[4]={0,0,0,0};
    uint32 i,j,key_posx,key_posy;
     
    InitGlobalVarible();
    Search_Rec_Pos();           //���ҵ�ǰ��ŵļ�¼λ��
     
    //InitModelIDVarible();       //�Ȱѻ����ͺŵĲ���������
    //Init_MachineVarible();
    //InitBrightnessVarible();             
    /*///////////////////////////////TEST START
    Clr_Bar(0,0,800,480,COL_BLACK);
    Put_Pixel(0,0,COL_WHITE);
    Put_Pixel(799,479,COL_WHITE);
    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Set_ASC_Lib(Arial14B_Font);
    Put_Data_Float(200,120,"%7.4f",123.4567);
    while(1){;}
    ////////////////////////////////TEST END*/
    //DRAW TITLE
    Clr_Bar(0,0,640,100,COL_BLACK);								//TITLE
	
	//Put_Bmp(0,0,logo22560,0);
   
	Clr_Bar(0,100,640,380,COL_BLACK);
	
	Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);   
   
   
  	//Put_Str(20,20,Language_Str("Benchmark Scientific","Benchmark Scientific"));
  	 
    if(1 == model_display_flag) 
    {
      if(INNER_CAL_VERSION)
       Put_Str( 280-80,   160,"SYXXXC");
      else
       Put_Str( 280-80,   160,"SYXXX");
     
       Put_Data(280-80+22,160, "%02d", (model_id&0xffff)/10);
       if(1 == dot_position)
       Put_Data(280-80+44,160,"%01d" , 4);
       else
       Put_Data(280-80+44,160,"%01d" , 3);
       
       Put_Str(280,160,Language_Str(" ������ƽ"," Electronic Balance"));
    }
    
    
  	Put_Str(280-80,210,Language_Str("ϵͳ��ʼ�� ......","System Initiating......"));
	
	Clr_Bar(0,300,640,35,COL_DEEPRED);
	
	for(i=0;i<SCR_XSIZE/2;i++)
	{
	Clr_Bar(0,300,i*2,35,COL_DEEPGREEN);
    if(i < SCR_XSIZE/4)
	Set_Chr_Color(COL_BLACK,COL_DEEPRED);
	else
	Set_Chr_Color(COL_BLACK,COL_DEEPGREEN);
	Put_Numeral(310, 310,i/4,1);
	Put_Char(310+30,310,'%');     //display 88%
	for(j=0;j<300;j++){;}    //��������������
//////////////////////////////////////////////////////STEP1:MODEL_data
	/*if(i == 20)      //
	 {
	 if(ERROR == InitScreenVarible())
     {
      Set_Chr_Color(COL_DEEPRED,COL_BLACK);
	  Put_Str(200-100,380,"ScreenCAL ERROR!,Please Recal Screen");
	 }
	else
     {
      Set_Chr_Color(COL_THINGREEN,COL_BLACK);
      Put_Str(200-100,380,"Screen CAL Data OK!");
	 }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}*/
///////////////////////////////////////////////////////////////
	if(i == 50)      //
	 {
	 if(ERROR == InitScreenVarible())
     {
      Set_Chr_Color(COL_DEEPRED,COL_BLACK);
	  Put_Str(200-100,380,"ScreenCAL ERROR!,Please Recal Screen");
	 }
	else
     {
      Set_Chr_Color(COL_THINGREEN,COL_BLACK);
      Put_Str(200-100,380,"Screen CAL Data OK!");
	 }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}
////////////////////////////////////////////////////STEP2:CAL_DATA
	if(i == 80)
	{
	
     if( ERROR == Init_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200-100,380,"WeightCal ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"WeighCal Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}		
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 100)
	{
	   if( ERROR == Init_Zer0_Weigh1_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200-100,380,"Zer0 data1 ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"Zer0 Data1 OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}

	if(i == 120)
	{
	   if( ERROR == Init_Zer0_Weigh2_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200-100,380,"Zer0 data2 ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"Zer0 Data2 OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}

	if(i == 140)
	{
	   if( ERROR == Init_LineCal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200-100,380,"Line Cali ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"Line Cali Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}

////////////////////////////////////////////////////STEP3:USER ID data
	if(i == 160)
	{
	   if( ERROR == InitUserIDVarible())
   	     {
          Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	      Put_Str(200-100,380,"User ID Data ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"User ID Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}
////////////////////////////////////////////////////STEP3:WIRELESS data
	if(i == 200)
	{
	   if( ERROR == InitWirelessVarible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200-100,380,"Wireless Data ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"Wireless Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}
///////////////////////////////////////////////////////
	if(INNER_CAL_VERSION &&(i == 240))
	{
	   if( ERROR == InitInnerCalVarible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200-100,380,"Inner_CAL Data ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200-100,380,"Inner_CAL Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200-100,380,400,30,COL_BLACK);
	}	
	
////////////////////////////////////////////////////STEP4:��⿪�����̲�������Ӧ�Ľ���
	if((i>50) && (key_times<4))  //���4������
	{
	 key_value = Get_PositionXY();   //����ɨ��
	 if(Key_Status == KEY_DO_START)
     {
      Key_Status = KEY_DO_OVER;           //��ʼ����
      key_times++;
      /*key_posx = (key_value>>16)&0xffff;
      key_posy = key_value&0xffff;
      if((key_posx>0)&&(key_posx<SCR_XSIZE/2)&&(key_posy>0)&&(key_posy<SCR_YSIZE/2))//area1
        { key_buf[key_times] = 1; key_times++;}
      if((key_posx>SCR_XSIZE/2)&&(key_posx<SCR_XSIZE)&&(key_posy>0)&&(key_posy<SCR_YSIZE/2))//area2
        { key_buf[key_times] = 2; key_times++;}
      //if((key_posx>0)&&(key_posx<SCR_XSIZE/2)&&(key_posy>SCR_YSIZE/2)&&(key_posy<SCR_YSIZE))//area3
      //  { key_buf[key_times] = 3; key_times++;}
      //if((key_posx>SCR_XSIZE/2)&&(key_posx<SCR_XSIZE)&&(key_posy>SCR_YSIZE/2)&&(key_posy<SCR_YSIZE))//area4
      //  { key_buf[key_times] = 4; key_times++;}*/
                                            //�������
     }	
	}
   }  
/////////////////////////////////////////////////STEP6:GLOBAL VARIABLE
    if(key_times > 1)
     return(MACHINE_SERVICE_MODE);     //ά��ģʽ
    else 
     return(MACHINE_NORMAL_MODE);        //����ģʽ
}
///////////////////////////////////////////////////
//14  ����X Yλ�ù����������� 
//////////////////////////////////////////////////
void  Create_ButtonList(uint32 (*p)[2],uint32 num)
	{
	         uint32 i;
	 struct  KeyStruct *pKey;
	 pKey = Key;
	 for(i=0;i<num;i++)
	 	{
		(*pKey).X_Position = (*p)[0];
		(*pKey).Y_Position = (*p)[1];
		(*pKey).Value      = i+1;
		 pKey++;
		 p++;
		}		
	}
	
///////////////////////////////////////////////////////////
//15 ���������־
///////////////////////////////////////////////////////////	
void  Set_Zer0_Flag(void)
{
  do_zer0_flag = TRUE;
}
///////////////////////////////////////////////////////////
//16 ����ȥƤ��־
///////////////////////////////////////////////////////////
void  Set_Tare_Flag(void)
{
  do_tare_flag = TRUE;
}
///////////////////////////////////////////////////////////
//17 ���òɼ��ٷֱȻ�׼��־
///////////////////////////////////////////////////////////
void  Set_Sam100_Flag(void)
{
  do_sam100_flag = TRUE;
}
///////////////////////////////////////////////////////////
//18 �����ۼӱ�־
///////////////////////////////////////////////////////////
void  Set_Totaladd_Flag(void)
{
  do_totaladd_flag = TRUE;
}
///////////////////////////////////////////////////////////
//19 ���òɼ�Ŀ��ֵ���ݱ�־
///////////////////////////////////////////////////////////
void  Set_SamTarget_Flag(void)
{
  do_samtarget_flag = TRUE;
}

///////////////////////////////////////////////////////////
//20 ���浽ָ��λ��һ����¼
//��������ر���
//����0:  ok
//    1:  not stable
//    2:  ee2prom fault
///////////////////////////////////////////////////////////
void  Save_Rec(uint32 mode)
    {
     un_rec_data rec_data;
     uint32 i;
     uint8 check_data;
     uint8 Send_Data[16];
     RTCTime LocalTime;   
     if((FALSE == positive_flag)||(FALSE == stable_flag))
     {
      Log_Alm(&tips_save_fault,ALM_LEV_TIPS);  //
     }
     else
     { 
       LocalTime = RTCGetTime();
       Send_Data[0] = REC_CHECK_DATA1;
       Send_Data[1] = REC_CHECK_DATA2;
       Send_Data[2] = LocalTime.RTC_Year-2000;
       Send_Data[3] = LocalTime.RTC_Mon;
       Send_Data[4] = LocalTime.RTC_Day;
       Send_Data[5] = LocalTime.RTC_Hour;
       Send_Data[6] = LocalTime.RTC_Min;
       Send_Data[7] = LocalTime.RTC_Sec;
       Send_Data[8] = mode;      //���ģʽ
       switch(mode)
        {
         case STAT_PCS:
              Send_Data[9]  = (pcs_number>>24)&0xff;
              Send_Data[10] = (pcs_number>>16)&0xff;
              Send_Data[11] =  (pcs_number>>8)&0xff;
              Send_Data[12] =       pcs_number&0xff;
              break;
         case STAT_100:
              rec_data.rec_float = number_100;
              Send_Data[9]  = (rec_data.rec_uint32>>24)&0xff;
              Send_Data[10] = (rec_data.rec_uint32>>16)&0xff;
              Send_Data[11] =  (rec_data.rec_uint32>>8)&0xff;
              Send_Data[12] =       rec_data.rec_uint32&0xff;
              break;
         case STAT_TOTAL:
              rec_data.rec_float = total_weight;
              Send_Data[9]  = (rec_data.rec_uint32>>24)&0xff;
              Send_Data[10] = (rec_data.rec_uint32>>16)&0xff;
              Send_Data[11] =  (rec_data.rec_uint32>>8)&0xff;
              Send_Data[12] =       rec_data.rec_uint32&0xff;
              break; 
         default:
              rec_data.rec_float = net_weight;
              Send_Data[9]  = (rec_data.rec_uint32>>24)&0xff;
              Send_Data[10] = (rec_data.rec_uint32>>16)&0xff;
              Send_Data[11] =  (rec_data.rec_uint32>>8)&0xff;
              Send_Data[12] =       rec_data.rec_uint32&0xff;
              break;
         }
       ////////////////////////����USERID �ֶ�
       Send_Data[13] = (user_id>>8)&0xff;
       Send_Data[14] =  user_id&0xff;
       
       ////////////////////////����13���ֽڵ� У���
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       
       Send_Data[15] = check_data; 
       //save one rec
       Write_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16); 
	   for(i=0;i<40000;i++)	{;}	         //delay
	  
	   for(i=0;i<16;i++) Send_Data[i] = 0;	 //clear buf
       //readback
	   Read_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16);	 
	   for(i=0;i<1000;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       if(Send_Data[15] == check_data)
       {
        rec_total_num++;               //�����ܼ�¼��
        
        if(rec_real_num < REC_MAX_NUM) //������Ч����
        rec_real_num++;
                                       //������ҳ�� 
        rec_total_pages =  rec_real_num/RECNUM_PER_PAGE + 1;
                                      
        rec_curre_index++;             //��������
        if(REC_MAX_NUM == rec_curre_index)
        rec_curre_index = 0;
                                       
        Send_Data[0] = (rec_total_num>>24)&0xff;
        Send_Data[1] = (rec_total_num>>16)&0xff;
        Send_Data[2] =  (rec_total_num>>8)&0xff;
        Send_Data[3] =       rec_total_num&0xff;
        Send_Data[4] = Send_Data[0];
        Send_Data[5] = Send_Data[1];
        Send_Data[6] = Send_Data[2];
        Send_Data[7] = Send_Data[3];
        Write_EEPROM(EEP_REC_TOTAL_ADDR,Send_Data, 8); 
        for(i=0;i<20000;i++)	{;}	
        Log_Alm(&tips_save_ok,ALM_LEV_TIPS);  //��������
       }
       else
       {
        Log_Alm(&tips_EE_bad,ALM_LEV_TIPS);
       }
    }
  }  

 ////////////////////////////////////
//���������������� ����ԭ��Ϊ
// < 2    0
// < 3    2
// < 5    3
/////////////////////////////////
static  float display_to_int(float net_tmp)
 {
    uint32 tmp,tmp2;
	if((WAN_DIVISION==dot_position)&&(1==adjust_0000_flag))
	{
      tmp  = (uint32)(net_tmp * 10000);
      tmp  = tmp % 10000;
      if((tmp<2)||(tmp>9998))
       {
        return(net_tmp);
       } 
      else if(tmp<4)
       {
        return(net_tmp-0.0002);
       }
      else if(tmp<6)
       {
        return(net_tmp-0.0003);
       }
      else if(tmp>9996)
       {
        return(net_tmp+0.0002);
       }
      else if(tmp>9994)
       {
        return(net_tmp+0.0003);
       }
      else 
        return(net_tmp);
     }
	/*
  else if(WAN10_DIVISION == dot_position) 
     {
      tmp  = (Uint32)(net_tmp * 100000);
      tmp  = tmp % 10000;
      if((tmp<2)||(tmp>9998))
       {
        return(net_tmp);
       } 
      else if(tmp<4)
       {
        return(net_tmp-0.00002);
       }
      else if(tmp<6)
       {
        return(net_tmp-0.00003);
       }
      else if(tmp>9996)
       {
        return(net_tmp+0.00002);
       }
      else if(tmp>9994)
       {
        return(net_tmp+0.00003);
       }
      else 
        return(net_tmp);
     }*/
  else
        return(net_tmp);   
 }
 
