
//**********************************************************************
//�ļ�˵��              
//����
//��1������ģʽ�˵��Ĵ���
//��2�����Բ˵��Ĵ���
//�����б�
//1   �����������������ʾ         MoreCfg_Disp_Pro
//2   ����������ʾ                 Brig_Adjust_Disp_Pro                  
//3   ����������ʾ                 WirelessCfg_Disp_Pro
//4   ��ĻУ׼��ʾ                 ScrCal_Disp_Pro
//5   ����ģʽ��ʾ      	       ModeSel_Disp_Pro
//6   ����������������ж� 	       Action_MoreCfg
//7   ���ȵ��������ж�      	   Action_BrightCfg
//8   �������ð����ж�      	   Action_WirelessCfg
//9   ��ĻУ׼��������      	   Action_ScreenCal
//10  ����ģʽ�����ж�      	   Action_ModeSelMenuCfg
//11  ��������������� ��ʼ��      MoreCfgInput_Interface
//12  ���ȵ������� ��ʼ��   	   Brig_Adjust_Interface
//13  �������ý��� ��ʼ��    	   WirelessCfgInput_Interface
//14  ��ĻУ׼���� ��ʼ��    	   ScrCalInput_Interface
//15  ����ģʽ���� ��ʼ��    	   ModeSel_Interface
//16  ����������������Ĵ���  	   MoreCfg_Key_Pro
//17  ���ȵ��� �����Ĵ���     	   Brig_Adjust_Key_Pro
//18  �������� �����Ĵ���     	   WirelessCfg_Key_Pro
//19  ��ĻУ׼ �����Ĵ���     	   ScrCal_Key_Pro
//20  ����ģʽ �����Ĵ���     	   ModeSel_Key_Pro
//21  ����ѡ���ͼ�λ���      	   Display_SelLang ������
//22  ����ѡ����水���ж�    	   Action_LangMenuCfg
//23  ����ѡ����� ��ʼ��    	   LangSel_Interface
//24  ����ѡ��ѡ��İ������� 	   LangSel_Key_Pro
//25  ����ѡ��ѡ�����ʾ����       LangSel_Disp_Pro
//26  ���������������ʾ����       NumberInput_Disp_Pro
//27  ������������� �����ж�      Action_NumInputMenuCfg
//28  ������������ ���� ��ʼ��     NumberInput_Interface
//29  ������������İ�������       NumInput_Key_Pro
//30  С���� �Ĵ���                dot_key_process
//31  �˸�� �Ĵ���                back_key_process
//32  �õ�������������� ��ʱ����  Get_NumInput_Tmp
//33  ������������ ��ʾ          PCSInput_Disp_Pro
//34  �������밴�� ���ж�          Action_PCSInputMenuCfg
//35  ������������ ��ʼ��        PCSInput_Interface
//36  ��������İ�������           PCSInput_Key_Pro
//37  �õ������������ʱ����       Get_PCSInput_Tmp
// 
//**********************************************************************
#include "Font_Lib.h"

#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "normal_operation.h"
#include "type_def.h"
#include "disp.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "ad7843.h"
#include "alarm_pro.h"
#include "speaker.h"

static uint32 language_var_tmp;
static uint32 sound_switch_tmp,bright_var_tmp;
static uint32 rec_page;             //��Ҫ��ʾ�ڼ�ҳ
static uint32 current_display_page; //��ǰ��ʾ���ǵڼ�ҳ
static uint32 rec_called_num;       //��ǰ�Ѿ���ʾ�˶�������¼
static uint32 curre_page_num;
////////////////////////////////////////////////////////
static uint32 num_buf[7],num_buf_full;
static float  Num_Input_tmp;
static uint32 Num_Input_Pos;//����ĵڼ�λ
////////////////////////////////////////////////////////
static uint32 PCSInput_tmp;
////////////////////////////////////////////////////////
static uint32 wireless_cfg_tmp,wireless_pin1_tmp,wireless_pin2_tmp;

//��ĻУ׼����
static uint32   Screen_CalStep,screen_step_tmp;
///////////////////////////////////////////////////////

void   Get_NumInput_Tmp(uint32 data);
void   Get_PCSInput_Tmp(uint32 data);
/*///////////////////////////////////////////////////
/*
/*1�����������������ʾ
//////////////////////////////////////////////////*/
void MoreCfg_Disp_Pro(void)
{
      
 }
/*///////////////////////////////////////////////////
/*
/*2����������ʾ
//////////////////////////////////////////////////*/ 
void  Speak_Adjust_Disp_Pro(void)
 {
   uint32 i;
   if(sound_switch_tmp != sound_switch)
   {
     sound_switch_tmp = sound_switch;
  
     Clr_Bar(450,130,20,100,COL_DEEPGRAY);    //erase
     //redraw
     Set_Graphic_Color(COL_DEEPBLUE,COL_WHITE);
     if(sound_switch_tmp&0x01)
      {
       Circle(460,210,9);
       Circle(460,210,8);
       Circle(460,210,7);
       Fill_Circle(460,210,3);
      }
     else
      {
       Circle(460,150,9);
       Circle(460,150,8);
       Circle(460,150,7);
       Fill_Circle(460,150,3);
      } 
   }
}
/*///////////////////////////////////////////////////
/*
/*2����������ʾ
//////////////////////////////////////////////////*/ 
void Brig_Adjust_Disp_Pro(void)
{
 uint32 i;
 if(bright_var_tmp != lcd_brightness)
  {
   bright_var_tmp = lcd_brightness;
   
   Clr_Bar(328,180,140,40,COL_BLACK);    //������ʾ��
   for(i=0;i<lcd_brightness;i++)
    {
     Clr_Bar(328+7*i,180,5,40,COL_DEEPGREEN); //4
     //Clr_Bar(352+10*i,180,2,40,COL_BLACK); //2
     }
   }
}
/*///////////////////////////////////////////////////
/*
/*2c �����Ļ����
//////////////////////////////////////////////////*/
void  Clr_Scr_Data(void)
    {
     uint32 i;
     for(i=0;i<10;i++)
     {
      Clr_Bar(95,  135+28*i, 40, 22, COL_DEEPGRAY); //���
      Clr_Bar(150, 135+28*i,155, 22, COL_DEEPGRAY); //����
      Clr_Bar(312, 135+28*i, 77, 22, COL_DEEPGRAY); //ģʽ
      Clr_Bar(400, 135+28*i,100, 22, COL_DEEPGRAY); //����
      Clr_Bar(515, 135+28*i, 40, 22, COL_DEEPGRAY); //UID
     }
    }
/*///////////////////////////////////////////////////
/*
/*2b ��ȡ�ƶ���ַǰ��һҳ����
//////////////////////////////////////////////////*/
static  void display_rec_weight_data(uint32 row,float rec_float)
  {
   if(QIAN_DIVISION == dot_position)
      Put_Data_Float(400, 135+28*row, "%7.3f",rec_float);
   else if(WAN_DIVISION == dot_position)
      Put_Data_Float(400, 135+28*row, "%8.4f",rec_float);
   else
      Put_Data_Float(400, 135+28*row, "%8.5f",rec_float);
  
  }
void Display_Page_Data(uint32 page_num)
    {
     un_rec_data rec_data;
     uint32 i,j,tmp;
     uint8  BufferData[16];
          
     if((0==rec_real_num)||(0 == page_num)||(page_num > rec_total_pages)) 
      {
       curre_page_num = 0;
       return; //�Ƿ����� ���߿�����
      }
     
     curre_page_num = rec_real_num-rec_called_num;
     if(0 == curre_page_num)   return;

     if(curre_page_num >= RECNUM_PER_PAGE) //����һҳ����һҳ��
       curre_page_num = RECNUM_PER_PAGE;
     
      //curre_page_num  ��ǰ��Ҫ��ʾ�ļ�¼��    
      //page_num        ��ǰҪ��ʾ��ҳ��
      Set_Chr_Color(COL_BLACK,COL_DEEPGRAY); 
      Set_ASC_Lib(Arial16B_Font);           //ʹ��16������
      for(i=0;i<curre_page_num;i++)
      {
       //���ȵõ��ü�¼�ĵ�ַ��ע���ַ�ķ�ת
       tmp = rec_curre_index-1 - (rec_called_num+i);
       if(tmp > rec_curre_index)                    //��ַ��ת��ļ��㷽��
       tmp = (REC_MAX_NUM-1) - (0xffffffff - tmp);  //ȡ�� ����ĩβȡֵ
         
       Read_EEPROM(EEP_REC_START_ADDR+tmp*EEP_BYTE_PER_REC, BufferData,16); //��ȡһ����¼
       for(j=0;j<1000;j++){;} //��ʱ
       //��֤��¼����Ч��
       //��һ�����
       Put_Data(95, 135+28*i, "%3d", (page_num-1)*RECNUM_PER_PAGE+i+1);//��ʾ���
       if((REC_CHECK_DATA1==BufferData[0])&&(REC_CHECK_DATA2==BufferData[1]))
       {//��֤OK����ʾһ����¼
        rec_data.rec_uint32 = (BufferData[9]<<24)+(BufferData[10]<<16)+(BufferData[11]<<8)+BufferData[12];
        //�ڶ�����ʾʱ�� ������ Сʱ����
        Put_Data(150, 135+28*i, "%02d",      BufferData[2]);
        Put_Char(172, 135+28*i,'-');
        Put_Data(182, 135+28*i, "%02d",      BufferData[3]);
        Put_Char(204, 135+28*i,'-');
        Put_Data(214, 135+28*i, "%02d",      BufferData[4]);
        
        Put_Data(245, 135+28*i, "%02d",      BufferData[5]);
        Put_Char(267, 135+28*i,':');
        Put_Data(277, 135+28*i, "%02d",      BufferData[6]);
        //������ģʽ
        //����������
        switch(BufferData[8])
         {
          case  STAT_WEIGH: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("����",    "WEIGH"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            break;
          case    STAT_PCS: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("����",    "COUNT"));
                            Put_Data(450, 135+28*i, "%d",        rec_data.rec_uint32);
                            break;
          case    STAT_100: Put_Str(Language_Pos(315,312), 135+28*i,Language_Str("�ٷֱ�","PERCENT"));
                            Put_Data_Float(410, 135+28*i, "%7.2f",rec_data.rec_float);
                            break;
          case  STAT_CHECK: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("����",    "CHECK"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            //Put_Data_Float(430, 135+28*i, "%8.4f",rec_data.rec_float);
                            break;
          case STAT_DIFFER: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("����",   "DIFFER"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            //Put_Data_Float(430, 135+28*i, "%8.4f",rec_data.rec_float);
                            break;
          case  STAT_TOTAL: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("�ۼ�",    "TOTAL"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            //Put_Data_Float(430, 135+28*i, "%8.4f",rec_data.rec_float);
                            break;
          case STAT_ANIMAL: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("����",   "ANIMAL"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            //Put_Data_Float(430, 135+28*i, "%8.4f",rec_data.rec_float);
                            break;
          case   STAT_PEAK: Put_Str(Language_Pos(325,325), 135+28*i,Language_Str("��ֵ",     "PEAK"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            //Put_Data_Float(430, 135+28*i, "%8.4f",rec_data.rec_float);
                            break;
          case STAT_TARGET: Put_Str(Language_Pos(325,320), 135+28*i,Language_Str("Ŀ��",   "TARGET"));
                            display_rec_weight_data(i,rec_data.rec_float);
                            //Put_Data_Float(430, 135+28*i, "%8.4f",rec_data.rec_float);
                            break;
          default:break;
         } 
         ////////////////ADD UID DISPLAY
         Put_Data(520, 135+28*i, "%03d", ((BufferData[13]<<8)+BufferData[14])%1000);//��ʾUID
       }
       else
       {//����¼��ȡ�д���ʱ���˳��������ѱ�ҳ�����ļ�¼��ʾ����
         Put_Str(Language_Pos(180,180), 135+28*i,Language_Str("������¼��","BAD RECORDER"));
        //break; 
       }
     } 
      rec_called_num += curre_page_num; //��������ʾ��¼��
   }  
/*///////////////////////////////////////////////////
/*
/*2a������ʾ����
//////////////////////////////////////////////////*/
void Data_Call_Disp_Pro(void)
{
 uint32 i;
 
 if(rec_page != current_display_page)
  {
   current_display_page = rec_page;
   Clr_Scr_Data();             //����Ļ�ϵ��������
   Display_Page_Data(rec_page);
  }   //ȷ����ҳ�����
}     //end

/*///////////////////////////////////////////////////
/*3a �ڹ̶�λ�� ��ʾ ������Ϣ
//////////////////////////////////////////////////*/
void WirelessCfg_Disp(uint32 x,uint32 channel,uint32 display_flash)
{
 Set_ASC_Lib(Arial16B_Font);
 if(1 == channel)
   {
    if(0 == display_flash)
    Clr_Bar(65,165,80,30,COL_DEEPGRAY);
    else
    {
     Clr_Bar(65,165,80,30,COL_DEEPGREEN); 
     Set_Chr_Color(COL_BLACK,COL_DEEPGREEN);
     switch(x)
      {
       case FUNCTION_NULL:  Put_Line_Str(85, 170,Language_Str("��",   "NULL"));  break;
       case FUNCTION_QP:    Put_Line_Str(85, 170,Language_Str("ȥƤ", "TARE"));  break;
       case FUNCTION_ZER0:  Put_Line_Str(85, 170,Language_Str("����", "ZER0"));  break;
       case FUNCTION_CAL:   Put_Line_Str(85, 170,Language_Str("У׼", "CAL"));   break;
       case FUNCTION_IDLE:  Put_Line_Str(85, 170,Language_Str("����", "IDLE"));  break;
       case FUNCTION_INCAL: Put_Line_Str(75, 170,Language_Str("��У", "INCAL")); break;
       default: break;
     } 
    }     
   }
 else
  {
    if(0 == display_flash)
    Clr_Bar(270,165,80,30,COL_DEEPGRAY);
    else
    { 
     Clr_Bar(270,165,80,30,COL_DEEPGREEN);
     Set_Chr_Color(COL_BLACK,COL_DEEPGREEN);
     switch(x)
     {
      case FUNCTION_NULL:  Put_Line_Str(290, 170,Language_Str("��",   "NULL"));  break;
      case FUNCTION_QP:    Put_Line_Str(290, 170,Language_Str("ȥƤ", "TARE"));  break;
      case FUNCTION_ZER0:  Put_Line_Str(290, 170,Language_Str("����", "ZER0"));  break;
      case FUNCTION_CAL:   Put_Line_Str(290, 170,Language_Str("У׼", "CAL"));   break;
      case FUNCTION_IDLE:  Put_Line_Str(290, 170,Language_Str("����", "IDLE"));  break;
      case FUNCTION_INCAL: Put_Line_Str(290, 170,Language_Str("��У", "INCAL")); break;
      default: break;
     }
    }      
   }   
}
/*///////////////////////////////////////////////////
/*3 �������� ��ʾ
//////////////////////////////////////////////////*/
void WirelessCfg_Disp_Pro(void)
{
 static  uint32 display_flash_tmp = 0;
 //////////////////////////////////////////////////////
 display_flash_tmp++;
 
 if(0 == (wireless_cfg_tmp&0x01))
 {
  /////////////////////////////////
  WirelessCfg_Disp(wireless_pin2_tmp,2,1);
 
  if(0 == (display_flash_tmp%3))
    WirelessCfg_Disp(wireless_pin1_tmp,1,0);
  else
    WirelessCfg_Disp(wireless_pin1_tmp,1,1);
  }  
 //////////////////////////////////////////////////////
 else
  {
   /////////////////////////////////
   WirelessCfg_Disp(wireless_pin1_tmp,1,1);
   
   if(0 == (display_flash_tmp%3))
   WirelessCfg_Disp(wireless_pin2_tmp,2,0);
   else
   WirelessCfg_Disp(wireless_pin2_tmp,2,1);
  }   
 }
 
/*///////////////////////////////////////////////////
/*4 ��ĻУ׼ ��ʾ
//////////////////////////////////////////////////*/
void ScrCal_Disp_Pro(void)
{
 if(Screen_CalStep != screen_step_tmp)
 {
  screen_step_tmp = Screen_CalStep;
  switch(screen_step_tmp)
  {
  case 1:  Draw_Point(            5,            5,     COL_WHITE);break;
  case 2:  Draw_Point(  SCR_XSIZE/2,  SCR_YSIZE/2,     COL_WHITE);break;
  case 3:  Draw_Point(  SCR_XSIZE-6,  SCR_YSIZE-6,     COL_WHITE);break;
  case 4:  Draw_Point(            5,            5, COL_DEEPGREEN);break;
  case 5:  Draw_Point(  SCR_XSIZE/2,  SCR_YSIZE/2, COL_DEEPGREEN);break;
  case 6:  Draw_Point(  SCR_XSIZE-6,  SCR_YSIZE-6, COL_DEEPGREEN);break;
  default:break;
  }
  }      
}
/*///////////////////////////////////////////////////
/*
/*5 ����ģʽ ������ʾ
//////////////////////////////////////////////////*/
void ModeSel_Disp_Pro(void)
{
      
 }
/*///////////////////////////////////////////////////
/*
/*6 ���������������ѡ��
//////////////////////////////////////////////////*/
uint32  Action_MoreCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<9;i++)
	   {
	     if((abs((*p).X_Position-x)< 90/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     {
	       switch(Key_Code)
	      {
	       case  1:    Put_Bmp( 10, 120,   brightness9060,1); break; //
           case  2:    Put_Bmp(110, 120,      speaker9060,1); break; //
           case  3:    Put_Bmp(210, 120,         time9060,1); break; //
    
           case  4:    Put_Bmp( 10, 200,     language9060,1); break; //
           case  5:    Put_Bmp(110, 200,    screencal9060,1); break; //
           case  6:    Put_Bmp(210, 200,       userid9060,1); break; //
    
           case  7:    Put_Bmp( 10, 280,     password9060,1); break; //
           case  8:    
                       if(DOUBLE_DIVISION)
                       Put_Bmp(110, 280,      factory9060,1); 
                       if(INNER_CAL_VERSION)
                       Put_Bmp(110, 280,  wirelesscfg9060,1);
                                                              break; //
           case  9:    Put_Bmp(215, 280,         exit8060,1); break; //
	       default:break;
           } 
          Key_Code = 0xAA55;
	     }
 	  //���ӷ�����ʾ ֱ�������ɿ� 
	  if(Key_Status == KEY_DO_START)  
	    {
	     Key_Status = KEY_DO_OVER; 
	     Key_Valid_Sound();
	      switch(Key_Code)
	      {
	       case  1:    Put_Bmp( 10, 120,   brightness9060,0); break; //
           case  2:    Put_Bmp(110, 120,      speaker9060,0); break; //
           case  3:    Put_Bmp(210, 120,         time9060,0); break; //
    
           case  4:    Put_Bmp( 10, 200,     language9060,0); break; //
           case  5:    Put_Bmp(110, 200,    screencal9060,0); break; //
           case  6:    Put_Bmp(210, 200,       userid9060,0); break; //
    
           case  7:    Put_Bmp( 10, 280,     password9060,0); break; //
           case  8:    
                       if(DOUBLE_DIVISION)
                       Put_Bmp(110, 280,      factory9060,0); 
                       if(INNER_CAL_VERSION)
                       Put_Bmp(110, 280,  wirelesscfg9060,0);
                                                              break; //
           case  9:    Put_Bmp(215, 280,         exit8060,0); break; //
           default:break;
           } 
	     }
	   return(Key_Code);
	}	
/*///////////////////////////////////////////////////
/*
/*7 ���ȵ��������ж�
//////////////////////////////////////////////////*/
uint32  Action_BrightCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<3;i++)
	   {
	     if((abs((*p).X_Position-x)< 120/2) && (abs((*p).Y_Position-y)< 50/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	  //���ӽ���10��У׼  ������ʵ���������� ������
	   for(;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 7) && (abs((*p).Y_Position-y)< 7))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER; 
	      Key_Valid_Sound();
	     } 
	   return(Key_Code);
	}
/*///////////////////////////////////////////////////
/*
/*7a ������ʾ�����ж�
//////////////////////////////////////////////////*/
uint32  Action_DataCallCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 60/2) && (abs((*p).Y_Position-y)< 50/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	     //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER; 
	      Key_Valid_Sound();
	     } 
	   return(Key_Code);
	}	
/*///////////////////////////////////////////////////
/*
/*8 �������ð����ж�
//////////////////////////////////////////////////*/
uint32  Action_WirelessCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<8;i++)
	   {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 40/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	    for(;i<11;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER; 
	      Key_Valid_Sound();
	     } 
	   return(Key_Code);
	}	
	
/*///////////////////////////////////////////////////
/*
/*9 ��ĻУ׼ ���̴���
//////////////////////////////////////////////////*/
void  Action_ScreenCal(uint16 x,uint16 y)
	{
	   static uint8  ScreenCalData[13];
	          uint8  CheckSum;
	   static uint16 Temp_x0,Temp_y0,Temp_xmid,Temp_ymid,Temp_xmax,Temp_ymax;
              uint32 i;
		
		switch(Screen_CalStep)
		 {
		 case 1:
		     if ((x>10) & (x<500) & (y>10) & (y<500))
			   {
			    Temp_x0 = x;Temp_y0 = y;
			    Screen_CalStep++;
			   }
			  break;
		 case 2:
		     if ((abs(x-XMID_DEFAULT)<300) & (abs(y-YMID_DEFAULT)<300))
			   {
			    Temp_xmid = x;Temp_ymid = y;
			    Screen_CalStep++;
			   }
			  break;
	     case 3:
		     if ((x>XMAX_DEFAULT-200) & (x<XMAX_DEFAULT+200) & (y>YMAX_DEFAULT-200) & (y<YMAX_DEFAULT+200))
			   {
			    Temp_xmax = x;Temp_ymax = y;
			    Screen_CalStep++;
			   }
			  break;
		 case 4:
		     if ((x>10) & (x<500) & (y>10) & (y<500))
			   {
			   Temp_x0 = (Temp_x0+x)/2;Temp_y0 = (Temp_y0+y)/2;
			   Screen_CalStep++;
			   }
			  break;
		 case 5:
		     if ((abs(x-XMID_DEFAULT)<300) & (abs(y-YMID_DEFAULT)<300))
			   {
			    Temp_xmid = (Temp_xmid+x)/2;Temp_ymid = (Temp_ymid+y)/2;
			    Screen_CalStep++;
			   }
			  break;
		 case 6:
		     if ((x>XMAX_DEFAULT-200) & (x<XMAX_DEFAULT+200) & (y>YMAX_DEFAULT-200) & (y<YMAX_DEFAULT+200))
			   {
			   Temp_xmax = (Temp_xmax+x)/2;Temp_ymax = (Temp_ymax+y)/2;
			   ScreenCalData[0]=  Temp_x0>>8;
			   ScreenCalData[1]=  Temp_x0&0xff; 
			   ScreenCalData[2]=  Temp_y0>>8;
			   ScreenCalData[3]=  Temp_y0&0xff; 
			   ScreenCalData[4]=  Temp_xmid>>8;
			   ScreenCalData[5]=  Temp_xmid&0xff; 
			   ScreenCalData[6]=  Temp_ymid>>8;
			   ScreenCalData[7]=  Temp_ymid&0xff; 
			   ScreenCalData[8]=  Temp_xmax>>8;
			   ScreenCalData[9]=  Temp_xmax&0xff; 
			   ScreenCalData[10]= Temp_ymax>>8;
			   ScreenCalData[11]= Temp_ymax&0xff; 
			   ScreenCalData[12]= 0;  
			   for(i=0;i<12;i++)
			   ScreenCalData[12] += ScreenCalData[i]; 
			   Write_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);   /* ����ʼ��ַ0x00��ʼд��10������*/
			   for(i=0;i<200000;i++)	{;}	                 //delay
			   for(i=0;i<13;i++) ScreenCalData[i] = 0;	 //clear buf
			   Read_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);	
			   for(i=0;i<1000;i++)	{;}					 //delay
               CheckSum = 0;							 //clear checksum
			   for(i=0;i<12;i++)
			   CheckSum += ScreenCalData[i];			 //get Pre_checksum
			   //////////////////////////////////////////////////
			   Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);
			   Set_ASC_Lib(Arial16B_Font);
			   //Set_Graphic_Color(COL_WHITE,COL_WHITE);
               //Rectangle(230,200,440,290); 
			   if(CheckSum != ScreenCalData[12])
			      {
			       Set_Chr_Color(COL_THINRED,COL_BLACK);
			       Put_Line_Str(250,  220,Language_Str("��ĻУ׼ʧ��","Screen Cal Failed!"));
			       Put_Line_Str(250,  250,Language_Str("��������������","Please Restart Machine!"));
				   while(1){;}
				  }
				  else
				  {
				   Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
				   Put_Line_Str(250,  220,Language_Str("��ĻУ׼�ɹ�","Screen Cal Successed!"));
				   Put_Line_Str(250,  250,Language_Str("��������������","Please Restart Machine!"));
				   while(1){;}
			      }
			  }
			  break;
		  default: break;
		 }
	}
/////////////////////////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*
/*10 ����ģʽѡ�� �����ж�
//////////////////////////////////////////////////*/
uint32  Action_ModeSelMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<9;i++)
	   {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 50/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     {
	       switch(Key_Code)
	      {
	       case  1:    Put_Bmp( 15, 120,   checkmode8060,1); break; //
           case  2:    Put_Bmp(115, 120,   totalmode8060,1); break; //
           case  3:    Put_Bmp(215, 120,    peakmode8060,1); break; //
    
           case  4:    Put_Bmp( 15, 200,  targetmode8060,1); break; //
           case  5:    Put_Bmp(115, 200,  animalmode8060,1); break; //
           
           case  6:    Put_Bmp(215, 200,  differmode8060,1); break; //
           //case  7:  Put_Bmp( 15, 280,  targetmode8060,1); break; //
           //case  8:  Put_Bmp(115, 280,  differmode8060);   break; //
           case  9:    Put_Bmp(215, 280,        exit8060,1); break; //
	       default:break;
           } 
          Key_Code = 0xAA55;
	     }
 	  //���ӷ�����ʾ ֱ�������ɿ� 
	  if(Key_Status == KEY_DO_START)  
	    {
	     Key_Status = KEY_DO_OVER; 
	     Key_Valid_Sound();
	      switch(Key_Code)
	      {
	       case  1:    Put_Bmp( 15, 120,   checkmode8060,0); break; //
           case  2:    Put_Bmp(115, 120,   totalmode8060,0); break; //
           case  3:    Put_Bmp(215, 120,    peakmode8060,0); break; //
    
           case  4:    Put_Bmp( 15, 200,  targetmode8060,0); break; //
           case  5:    Put_Bmp(115, 200,  animalmode8060,0); break; //
           case  6:    Put_Bmp(215, 200,  differmode8060,0); break; //
         
           //case  7:  Put_Bmp( 15, 280,  targetmode8060,0); break; //
           //case  8:  Put_Bmp(115, 280,  differmode8060,0); break; //
           case  9:    Put_Bmp(215, 280,        exit8060,0); break; //
           default:break;
           } 
	     }
	   return(Key_Code);
	}		
	
//**********************************************************************
//11������:MoreCfgInput_Interface
//��������:�����������������
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void MoreCfgInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[9][2];
    
    //Create window
    Create_Window_Frame(0,80,300,270,3,COL_THINGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("ϵͳ����","System Configuration")); 
       
    Clr_Bar(    3,100+90,300,   3,COL_THINGREEN);//���߳�Ϊ�Ź��� 
    Clr_Bar(    3,180+90,300,   3,COL_THINGREEN); 
    Clr_Bar(100+3,   113,  3, 240,COL_THINGREEN); 
    Clr_Bar(200+3,   113,  3, 240,COL_THINGREEN); 
    ///////////////////////////////////////
    //ʹ��ͼƬ����
    Put_Bmp( 10, 120,brightness9060,0);    //����
    Put_Bmp(110, 120,speaker9060,0);       //����
    Put_Bmp(210, 120,time9060,0);          //ʱ�����
    
    Put_Bmp( 10, 200,language9060,0);      //����
    Put_Bmp(110, 200,screencal9060,0);     //��ĻУ׼
    Put_Bmp(210, 200,userid9060,0);        //����Ա
    
    Put_Bmp( 10, 280,password9060,0);      //����
    if(DOUBLE_DIVISION)
    Put_Bmp(110, 280,factory9060,0);       //�ָ�����
    if(INNER_CAL_VERSION)
    Put_Bmp(110, 280,wirelesscfg9060,0);   //����Ϊ��������
    
    Put_Bmp(215, 280,exit8060,0);          //�˳�
     
    //create button list
    xy[0][0] =  10+90/2;    xy[0][1] = 130+60/2;
    xy[1][0] = 110+90/2;    xy[1][1] = 130+60/2;
    xy[2][0] = 210+90/2;    xy[2][1] = 130+60/2;
    xy[3][0] =  10+90/2;    xy[3][1] = 210+60/2;
    xy[4][0] = 110+90/2;    xy[4][1] = 210+60/2;
    xy[5][0] = 210+90/2;    xy[5][1] = 210+60/2;
    xy[6][0] =  10+90/2;    xy[6][1] = 290+60/2;
    xy[7][0] = 110+90/2;    xy[7][1] = 290+60/2;
    xy[8][0] = 215+80/2;    xy[8][1] = 290+50/2;
                
    Create_ButtonList(xy,9);                            
}

//**********************************************************************
//12������:Brig_Adjust_Interface
//��������:���ȵ��� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Speak_Adjust_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[3][2];
    
     //Create window
     Create_Window_Frame(260+6,80,230,220,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(290,90,Language_Str("��������","Speaker Enable"));
    
     Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
     Bar(260+25,130, 260+25+150,130+40);
     Bar(260+25,190, 260+25+150,190+40);
    
     Put_Button_Bar(330, 250,330+100, 250+40,3,COL_THINBLUE);//OK
     //fill text
     Set_ASC_Lib(Arial16B_Font); 
     Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
     Put_Line_Str(Language_Pos(315,305), 140,Language_Str("�����ر�",  "SPEAKER OFF"));
     Put_Line_Str(Language_Pos(315,305), 200,Language_Str("��������",  "SPEAKER ON"));
     Set_Chr_Color(COL_WHITE,COL_THINBLUE);
     Put_Line_Str(Language_Pos(360,360), 260,Language_Str("ȷ��"," OK"));
   
     //create button list
     xy[0][0] =  285 + 150/2;    xy[0][1] = 130+40/2;
     xy[1][0] =  285 + 150/2;    xy[1][1] = 190+40/2;
     xy[2][0] =  330 + 100/2;    xy[2][1] = 250+40/2;
                
     Create_ButtonList(xy,3);   
    
     sound_switch_tmp = (sound_switch+1)&0x01;           
}	

//**********************************************************************
//12������:Brig_Adjust_Interface
//��������:���ȵ��� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Brig_Adjust_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
    
     //Create window
     Create_Window_Frame(260+6,80,260,220,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(290,85,Language_Str("���ȵ���","Brightness Adjust"));
    
     Set_Chr_Color(COL_THINCYAN,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font); 
     Put_Line_Str(Language_Pos(345,330), 130,Language_Str("��ǰ����", " Brightness"));    
    
     Clr_Bar(320+8,180,140,40,COL_BLACK);    //������ʾ��
      
     Put_Button_Bar(260+6, 170,260+6+60,   170+60,3,COL_THINBLUE);//add
     Put_Button_Bar(460+10,170,460+10+60,  170+60,3,COL_THINBLUE);//sub
     Put_Button_Bar(340+6, 240,340+6+110,  240+60,3,COL_THINBLUE);//ok
     
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     Put_Line_Str(Language_Pos(305-20,300-20), 190,Language_Str("��","SUB"));
     Put_Line_Str(Language_Pos(480+10,475+10), 190,Language_Str("��","ADD"));
     Put_Line_Str(Language_Pos(380,375), 260,Language_Str("ȷ��"," OK"));
   
     //create button list
     xy[0][0] =  266 + 60/2;    xy[0][1] = 170+60/2;
     xy[1][0] =  470 + 60/2;    xy[1][1] = 170+60/2;
     xy[2][0] =  346 + 110/2;   xy[2][1] = 240+60/2;
     xy[3][0] =         15;     xy[3][1] =       15;       //���Ӱ� ���̣����Խ���10��У׼
     xy[4][0] =         15;     xy[4][1] = SCR_YSIZE-15;   //���Ӱ� ���̣�������У��ƽ������ʵ����������
                  
    Create_ButtonList(xy,5);   
    
    bright_var_tmp = lcd_brightness+1; //��Ϊ���첻һ��             
}	

//**********************************************************************
//12a������:data_call_Interface
//��������:���ݻ���ʾ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Data_Call_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
    
     //Create window
     Create_Window_Frame(80,70,480,400,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(90,80,Language_Str("���ݼ�¼","Recorder"));
     Put_Char(190,80,'-');
     Put_Data(205,80,"%03d",rec_real_num);   //�ڱ�������ʾ��¼����
     
     //���ߵ���ɫ 
     Set_Graphic_Color(COL_THINRED,COL_DEEPGRAY);
     for(i=0;i<12;i++) //ÿҳ��ʾ10����¼ 
       Hor_Line(84,562,103+28*i);
   
     Ver_Line(103,411,83); 
     Ver_Line(103,411,140); //���
     Ver_Line(103,411,310); //����
     Ver_Line(103,411,390); //ģʽ
     Ver_Line(103,411,505); //����
     Ver_Line(103,411,562); //UID
       
     Set_Chr_Color(COL_BLACK,COL_DEEPGRAY);
     Put_Line_Str(Language_Pos(90,100),   108,Language_Str("���",   "No."));
     Put_Line_Str(Language_Pos(205,205), 108,Language_Str("����",  "Date"));
     Put_Line_Str(Language_Pos(325,325), 108,Language_Str("ģʽ",  "Mode"));
     Put_Line_Str(Language_Pos(425,425), 108,Language_Str("����",  "DATA"));
     Put_Line_Str(Language_Pos(520,520), 108,Language_Str("UID",    "UID"));
     ////////////////////////////////////////////////////////////////////
     Put_Button_Bar(130, 420, 130+80,  420+45,3,COL_THINBLUE);//clearALL
     Put_Button_Bar(230, 420, 230+80,  420+45,3,COL_THINBLUE);//prepage
     Put_Button_Bar(330, 420, 330+80,  420+45,3,COL_THINBLUE);//nextpage
     Put_Button_Bar(430, 420, 430+80,  420+45,3,COL_THINBLUE);//exit
    
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     Put_Line_Str(Language_Pos(140+5,140),     435,Language_Str("���",   "DelALL"));
     Put_Line_Str(Language_Pos(230+3,230+3),   435,Language_Str("��һҳ", "PrePage"));
     Put_Line_Str(Language_Pos(330+3,330+3),   435,Language_Str("��һҳ", "NexPage"));
     Put_Line_Str(Language_Pos(430+15,430+20), 435,Language_Str("�˳�",   "Exit"));
   
     //create button list
     xy[0][0] =  130 + 80/2;    xy[0][1] = 420+50/2;
     xy[1][0] =  230 + 80/2;    xy[1][1] = 420+50/2;
     xy[2][0] =  330 + 80/2;    xy[2][1] = 420+50/2;
     xy[3][0] =  430 + 80/2;    xy[3][1] = 420+50/2;
                  
     Create_ButtonList(xy,4); 
     curre_page_num = 0;       //��ǰҳ����ʾ�ļ�¼����
     rec_called_num = 0;       //��ǰ��ʾ�ļ�¼��Ϊ0
     current_display_page = 0; 
     rec_page = 1;             //�ӵ�һҳ��ʼ��ʾ
          
}
//**********************************************************************
//13����  ��:WirelessCfgInput_Interface
//��������:�������� ��ʾ����ĳ�ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void WirelessCfgInput_Interface(sint32 cur_menu_op)
{
      uint32 xy[11][2];
      uint32 i;
     //Create window
     Create_Window_Frame(0,80,400,380-80,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
     Put_Line_Str(20,85,Language_Str("��������", "Wireless Config")); 
     //���ָ���
     Clr_Bar( 198, 113,   3,  97,COL_WHITE);
     Clr_Bar(   3, 207, 400,   3,COL_WHITE);
     
     Set_Chr_Color(COL_BLACK,COL_DEEPGRAY);
     
     Put_Line_Str( Language_Pos(50,55),  130,Language_Str("��������A", "WirelessA"));  
     Put_Line_Str( Language_Pos(250,255),130,Language_Str("��������B", "WirelessB"));  
     
     Clr_Bar( 10,220, 80,40,COL_DEEPBLUE);  //FUNCTION 1
     Clr_Bar(110,220, 80,40,COL_DEEPBLUE);
     Clr_Bar(210,220, 80,40,COL_DEEPBLUE);
     Clr_Bar(310,220, 80,40,COL_DEEPBLUE);
     
     Clr_Bar( 10,280, 80,40,COL_DEEPBLUE);  
     Clr_Bar(110,280, 80,40,COL_DEEPBLUE);  //FUNCTION 6
     
     Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
     Put_Line_Str( 10+15, 230,Language_Str("ȥƤ","TARE"));
     Put_Line_Str(110+15, 230,Language_Str("����","ZER0"));
     Put_Line_Str(210+20, 230,Language_Str("У׼","CAL"));
     Put_Line_Str(310+15, 230,Language_Str("����","IDLE"));
     
     Put_Line_Str( 10+20, 290,Language_Str( "��","NULL"));
     
     if(INNER_CAL_VERSION)
     Put_Line_Str(110+15, 290,Language_Str("��У","INCAL"));

     Put_Button_Bar( 55, 330, 55+80,  330+50,3,COL_THINBLUE);//ESC
     Put_Button_Bar(155, 330,155+80,  330+50,3,COL_THINBLUE);//OK
     Put_Button_Bar(255, 330,255+80,  330+50,3,COL_THINBLUE);//SWITCH
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     Put_Line_Str( 55+10, 345,Language_Str("ȡ��","CANCEL"));
     Put_Line_Str(Language_Pos(175,185),    345,Language_Str("ȷ��","OK"));
     Put_Line_Str(Language_Pos(275,275-10), 345,Language_Str("�л�","SWITCH"));
    
    //create button list
    for(i=0;i<4;i++)
    {
     xy[i][0] =  50 +     i*100;   xy[i][1] = 240;
    }
    for(;i<8;i++)
    {
     xy[i][0] =  50 + (i-4)*100;   xy[i][1] = 300;
    }
    for(;i<11;i++)
    {
     xy[i][0] =  95 + (i-8)*100;   xy[i][1] = 355;
    }
    Create_ButtonList(xy,11);   
    
    wireless_cfg_tmp  = 0;
    wireless_pin1_tmp = wireless_pin1_configuration;
    wireless_pin2_tmp = wireless_pin2_configuration;
    WirelessCfg_Disp(wireless_pin1_configuration,1,1);
    WirelessCfg_Disp(wireless_pin2_configuration,2,1);
}	
//**********************************************************************
//14����  ��:ScrCalInput_Interface
//��������:��ĻУ׼ ����ĳ�ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void ScrCalInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
      
     //Create window
      machine_mode = MACHINE_SCREENCAL_MODE;
      Screen_CalStep = 1;
      screen_step_tmp = 0;
      
}	
//**********************************************************************
//15����  ��:ModeSel_Interface
//��������:����ģʽ �Ľ����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void ModeSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[9][2];
    
    //Create window
    Create_Window_Frame(0,80,300,270,3,COL_THINGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("����ģʽѡ��","Mode Select")); 
       
    Clr_Bar(    3,100+90,300,   3,COL_THINGREEN);//���߳�Ϊ�Ź��� 
    Clr_Bar(    3,180+90,300,   3,COL_THINGREEN); 
    Clr_Bar(100+3,   113,  3, 240,COL_THINGREEN); 
    Clr_Bar(200+3,   113,  3, 240,COL_THINGREEN); 
    ///////////////////////////////////////
    //ʹ��ͼƬ����
    Put_Bmp( 15, 120,checkmode8060,0);  //����
    Put_Bmp(115, 120,totalmode8060,0);  //�ۼ�
    Put_Bmp(215, 120,peakmode8060,0);   //��ֵ
    
    Put_Bmp( 15, 200,targetmode8060,0); //Ŀ��ֵ
    Put_Bmp(115, 200,animalmode8060,0); //����
   
    #if(1 == INNER_CAL_VERSION)
    Put_Bmp(215, 200,differmode8060,0);  // ���Ӳ��컯 ���ڲɼ���������
    #endif
    //Put_Bmp( 15, 280,densitymode8060,0);  //
    //Put_Bmp(115, 280,differmode8060); //
    Put_Bmp(215, 280,exit8060,0);  //
     
    //create button list
    xy[0][0] =  15+80/2;    xy[0][1] = 130+50/2;
    xy[1][0] = 115+80/2;    xy[1][1] = 130+50/2;
    xy[2][0] = 215+80/2;    xy[2][1] = 130+50/2;
    xy[3][0] =  15+80/2;    xy[3][1] = 210+50/2;
    xy[4][0] = 115+80/2;    xy[4][1] = 210+50/2;
    xy[5][0] = 215+80/2;    xy[5][1] = 210+50/2;
    xy[6][0] =  15+80/2;    xy[6][1] = 290+50/2;
    xy[7][0] = 115+80/2;    xy[7][1] = 290+50/2;
    xy[8][0] = 215+80/2;    xy[8][1] = 290+50/2;
                
    Create_ButtonList(xy,9);               
}
//**********************************************************************
//16����  ��:MoreCfg_Key_Pro
//��������:
//��ڲ���:ѡ����������������ã�
//          
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 MoreCfg_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MoreCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //ѡ������
  	           op_id = MENU_BRIG_CFG_OPER;
  	           break;
    	case 2://ѡ������
    	       op_id = MENU_SPEAK_CFG_OPER;
    	       break;
       	case 3://ʱ�����
       	       op_id = MENU_DATE_CFG_OPER; 	       
       	       break;
   		case 4://����
   		       op_id = MENU_LANG_SEL_OPER;
   		       break;
   	    case 5://��ĻУ׼
   		       op_id = MENU_SCR_CAL_OPER;
   		       break;
   		case 6://����Ա
   		       op_id =  MENU_OPERID_CFG_OPER;
   		       break;
   	    case 7://����
   	           op_id = MENU_PASSW_CFG_OPER;
   	           break;
   	    case 8://�ָ�����
   		       //op_id = MENU_FACTORY_CFG_OPER;
   		       if(DOUBLE_DIVISION)
   		       op_id = MENU_RANGE_SEL_OPER;
   		       if(INNER_CAL_VERSION)
   		       op_id = MENU_WIRELESS_CFG_OPER;
   		       
   		       //else
   		       //op_id = MENU_OP_OPER;
   		       break;
   	    case 9://�˳�
   		       op_id = MENU_OP_OPER;
   		       break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//17a ���ȵ����µİ�������
//**********************************************************************
sint32 Speak_Adjust_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  speak_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_BrightCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //�ر�
  	           sound_switch = OFF;
  	           /*if(speaker_volume > SPEAK_CLASS_TOTAL)
  	              speaker_volume = SPEAK_CLASS_TOTAL;
  	           Set_Speaker_Volume(speaker_volume);*/
  	           break;
    	case 2://����
    	       sound_switch = ON;
    	       /*if(speaker_volume>1)
    	       speaker_volume--;
    	       Set_Speaker_Volume(speaker_volume);*/
    	       break;
       	case 3://save to eeprom
       	       speak_tx_buf[0] = (uint8)(sound_switch&0xff);
  	           speak_tx_buf[1] = 0;
  	           speak_tx_buf[2] = (uint8)((sound_switch+REC_CHECK_DATA1)&0xff);
  	           speak_tx_buf[3] = 0; 
  	           Write_EEPROM(EEP_SPEAK_ADDR,speak_tx_buf, 4);
  	           ////////////////////////////////
       	       op_id = MENU_OP_OPER; //���ص����ò˵�
       	       break;
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//17 ���ȵ����µİ�������
//**********************************************************************
sint32 Brig_Adjust_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  brightness_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_BrightCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 2: //ADD
  	           lcd_brightness++;
  	           if(lcd_brightness > BRIGHT_CLASS_TOTAL)
  	              lcd_brightness = BRIGHT_CLASS_TOTAL;
  	           Set_LCD_Brightness(lcd_brightness);
  	           break;
    	case 1://SUB
    	       if(lcd_brightness>1)
    	       lcd_brightness--;
    	       Set_LCD_Brightness(lcd_brightness);
    	       break;
       	case 3://save to eeprom
       	       brightness_tx_buf[0] = (uint8)(lcd_brightness);
  	           brightness_tx_buf[1] = 0;
  	           brightness_tx_buf[2] = 0;
  	           brightness_tx_buf[3] = brightness_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,brightness_tx_buf, 4);
  	           ////////////////////////////////
       	       op_id = MENU_OP_OPER; //���ص����ò˵�
       	       break;
       	case 4:
       	       point10_cal_ok = FALSE;   //��¼ԭʼ����
       	       op_id = MENU_CAL_10POINT_OPER; //����10��У׼  
       	       break;
       	case 5:  //����������ʵ�������� 
       	       if(STAT_DIFFER == current_mode)
       	       op_id = MENU_NUM_INPUT_OPER;
       	       break;
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//17a ������ʾ��������
//**********************************************************************
sint32 Data_Call_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  buffer[8];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_DataCallCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //clear ALL
  	           for(i=0;i<8;i++)
  	           buffer[i] = 0;
  	           Write_EEPROM(EEP_REC_TOTAL_ADDR,buffer, 8);
  	           for(i=0;i<1000;i++) {;}
  	           Log_Alm(&tips_clear_ok,ALM_LEV_TIPS);
  	           rec_total_num   = 0;
  	           rec_real_num    = 0;
		       rec_curre_index = 0;
		       rec_total_pages = 1;
  	           break;
    	case 2://��һҳ
    	       if(1 == rec_page)
    	       {
    	        //��ʾ��Ϣ�ǵ�һҳ
    	         Log_Alm(&tips_first_page,ALM_LEV_TIPS);
    	       }
    	       else if(rec_page>1)
    	       {rec_page--;
    	        rec_called_num = rec_called_num - RECNUM_PER_PAGE-curre_page_num; //������ʾ��¼������ȥ
    	       }
    	       
    	       break;
       	case 3://��һҳ
       	       rec_page++;
       	       if(rec_page > rec_total_pages)
       	       {
       	        rec_page--;
       	        //��ʾ��Ϣ�����һҳ
       	        Log_Alm(&tips_last_page,ALM_LEV_TIPS);
       	       } 
       	       break;
       	case 4:
       	       op_id = MENU_OP_OPER; //���ص����˵�
       	       break;
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//18  ���������µİ�������
//**********************************************************************
sint32 WirelessCfg_Key_Pro(uint32 key_c,sint32 op)
{   
    static uint8  eeprom_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    /*if(key_c != 0)
    {
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINBLUE);
    Put_Data(100,20,"%03d", (key_c>>16)&0xffff);
    Put_Data(150,20,"%03d",       key_c&0xffff);
    }*/
    i = Action_WirelessCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://qp
  	           if(0 == (wireless_cfg_tmp&0x01))
  	           wireless_pin1_tmp = FUNCTION_QP;
  	           else
  	           wireless_pin2_tmp = FUNCTION_QP;
  	           break;
    	case 2://zer0
    	       if(0 == (wireless_cfg_tmp&0x01))
  	           wireless_pin1_tmp = FUNCTION_ZER0;
  	           else
  	           wireless_pin2_tmp = FUNCTION_ZER0;
  	           break;
        case 3://caling
  	           if(0 == (wireless_cfg_tmp&0x01))
  	           wireless_pin1_tmp = FUNCTION_CAL;
  	           else
  	           wireless_pin2_tmp = FUNCTION_CAL;
  	           break;
    	case 4://IDLE
  	           if(0 == (wireless_cfg_tmp&0x01))
  	           wireless_pin1_tmp = FUNCTION_IDLE;
  	           else
  	           wireless_pin2_tmp = FUNCTION_IDLE;
  	           break;
  	    case 5:
  	           if(0 == (wireless_cfg_tmp&0x01))
  	           wireless_pin1_tmp = FUNCTION_NULL;
  	           else
  	           wireless_pin2_tmp = FUNCTION_NULL;
  	           break;
  	    case 6://��У׼
  	           if(INNER_CAL_VERSION)
  	            {
  	             if(0 == (wireless_cfg_tmp&0x01))
  	             wireless_pin1_tmp = FUNCTION_INCAL;
  	             else
  	             wireless_pin2_tmp = FUNCTION_INCAL;
  	             }
  	           break;
  	    case 7:
  	    case 8:
  	           break; //null       
    	case 9://esc
    	       op_id = MENU_OP_OPER; //���ص����ò˵�
    	       break;
        case 10://ok
               wireless_pin1_configuration = wireless_pin1_tmp;
               wireless_pin2_configuration = wireless_pin2_tmp;
  	           eeprom_tx_buf[0] = (uint8)(wireless_pin1_configuration);
  	           eeprom_tx_buf[1] = (uint8)(wireless_pin2_configuration);
  	           eeprom_tx_buf[2] = REC_CHECK_DATA1;
  	           eeprom_tx_buf[3] = REC_CHECK_DATA2; 
  	           Write_EEPROM(EEP_WIRELESS_ADDR,eeprom_tx_buf, 4);
  	           ////////////////////////////////
       	       op_id = MENU_OP_OPER; //���ص����ò˵�
  	           break;
        case 11://switch
    	       wireless_cfg_tmp++;
    	       break;	        	       
        default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//19  ��ĻУ׼
//**********************************************************************
sint32 ScrCal_Key_Pro(uint32 key_c,sint32 op)
{   
    sint32 op_id;
    op_id = op;     
    if(KEY_DO_START == Key_Status)
     {
      Action_ScreenCal((key_c>>16)&0xffff,key_c&0xffff); 
   	  Key_Status = KEY_DO_OVER;
     }
     return(op_id);
}
//**********************************************************************
//20����  ��:ModeSel_Key_Pro
//��������:����ģʽ�µ�ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 ModeSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_ModeSelMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //
  	           current_mode = STAT_CHECK;
  	           break;
    	case 2://
    	       current_mode = STAT_TOTAL;
    	       break;
       	case 3://
       	       current_mode = STAT_PEAK;
       	       break;
   		case 4://
   		       current_mode = STAT_TARGET;
   		       break;
   		case 5://NULL
   		       current_mode = STAT_ANIMAL;
   		       break;
   	    case 6: //ѡ��
   	            #if(1 == INNER_CAL_VERSION)
  	           current_mode = STAT_DIFFER;
  	           #endif
  	           break;
    	case 7://ѡ�����ģʽ
    	       //current_mode = STAT_TARGET;
    	       break;
       	case 8://
       	       //current_mode = STAT_100;
       	       break;
   		case 9:
   		       //op_id = MENU_OP_OPER;
   		       break;
   	  	default:break;
   	   }
   	    op_id = MENU_OP_OPER;
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
////////22222
/*///////////////////////////////////////////////////
/* 21  ����ѡ��ѡ�� �� ���Ż���
//////////////////////////////////////////////////*/
void Display_SelLang(void)
{
  
   Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
   switch(language_var_tmp)
   {case CHINESE:
                 Circle(430,150,9);
                 Circle(430,150,8);
                 Circle(430,150,7);
                 Fill_Circle(430,150,4);
                 Clr_Bar(430-10,200-10,20,20,COL_DEEPGRAY);
                 break;
    case ENGLISH:
                 Circle(430,200,9);
                 Circle(430,200,8);
                 Circle(430,200,7);
                 Fill_Circle(430,200,4);
                 Clr_Bar(430-10,150-10,20,20,COL_DEEPGRAY);
                 break;
    default:break;
    }
 }  
/*///////////////////////////////////////////////////
//  22  ����ѡ��� �����ж�
//////////////////////////////////////////////////*/
uint32  Action_LangMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 40/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER; 
	      Key_Valid_Sound();
	     } 
	   return(Key_Code);
	}
//**********************************************************************
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void LangSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
    
//Create window
    Create_Window_Frame(256,80,200,220,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(270,85,Language_Str("����ѡ��","Language Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
    Bar(260+10,130,260+10+130,130+40);    //chinese
    Bar(260+10,180,260+10+130,180+40);    //english
    
    Put_Button_Bar(270, 250,270+80, 250+40,3,COL_THINBLUE);//esc
    Put_Button_Bar(360, 250,360+80, 250+40,3,COL_THINBLUE);//ok
  
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270+40,  130+10,Language_Str("����",   "����"));
    Put_Line_Str(270+25,  190+ 0,Language_Str("ENGLISH","ENGLISH"));
    Set_Chr_Color(COL_BLACK,COL_THINBLUE);
    Put_Line_Str(Language_Pos(290,280),  250+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(380,390),  250+10,Language_Str("ȷ��",   "OK"));
    //create button list
    xy[0][0] = 270+130/2;    xy[0][1] = 130+40/2;
    xy[1][0] = 270+130/2;    xy[1][1] = 180+40/2;
    xy[2][0] = 270+80/2;     xy[2][1] = 250+40/2;
    xy[3][0] = 360+80/2;     xy[3][1] = 250+40/2;
                  
    Create_ButtonList(xy,4); 
    ///////////////////////////////
    language_var_tmp = Get_Language_Type();
    Display_SelLang();   
}
//**********************************************************************
//24 ����  ��:LangSel_Key_Pro
//��������:����ѡ�� �˵��µİ�������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 LangSel_Key_Pro(uint32 key_c,sint32 op)
{
    uint8  eeprom_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LangMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://CHINESE
  	           language_var_tmp = CHINESE ; //Set_Language_Type(CHINESE);
  	           break;
    	case 2://ENGLISH
    	       language_var_tmp = ENGLISH;  //Set_Language_Type(ENGLISH);
    	       break;
       	case 3://Esc
       	       op_id =  MENU_OP_OPER;  // return
       	       break;
       	case 4://OK
       	       Set_Language_Type(language_var_tmp);
       	       eeprom_tx_buf[0] = (uint8)(language_var_tmp);
  	           eeprom_tx_buf[1] = 0;
  	           eeprom_tx_buf[2] = 0;
  	           eeprom_tx_buf[3] = eeprom_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_LANGUAGE_ADDR,eeprom_tx_buf, 4);
  	           ////////////////////////////////
       	       op_id =  MENU_OP_OPER;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//25 ����  ��:LangSel_Disp_Pro               
//��������:����ѡ�� �˵���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void LangSel_Disp_Pro(void)
{
   Display_SelLang();
  }

////////////////////////////////////////////////33333
/*///////////////////////////////////////////////////
/* 26 ������������ ����ʾ����
//////////////////////////////////////////////////*/
void NumberInput_Disp_Pro(void)
{
   static float display_tmp;
   if(display_tmp != Num_Input_tmp)
   {
    display_tmp = Num_Input_tmp;
    Clr_Bar(240,120,130,30,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    Set_ASC_Lib(Arial20B_Font);
    if(TRUE == num_buf_full)
    {
     Put_Data_Float(240,   125, "%8.4f",        Num_Input_tmp);
    }
    else
    {
     switch(Num_Input_Pos)
     {
      case 0:
      case 1:
      case 2:
      case 3:        Put_Data(260,   125,   "%3d",(uint32)Num_Input_tmp);break;
      case 4:  Put_Data_Float(260,   125, "%5.1f",        Num_Input_tmp);break;
      case 5:  Put_Data_Float(250,   125, "%6.2f",        Num_Input_tmp);break;
      case 6:  Put_Data_Float(250,   125, "%7.3f",        Num_Input_tmp);break;
      default:
               break;
      } //switch end
     }  //if     end
    }   //if     end
   }    //func   end
/*///////////////////////////////////////////////////
/*
/*
//////////////////////////////////////////////////*/
#define  NUMBER_WIDTH       80
#define  NUMBER_HEIGTH      60
#define  NUM_WEI_INPUT      01
#define  NUM_PCS_INPUT      02
/*///////////////////////////////////////////////////
/* 27  �������� �İ��� ��Ӧ
//////////////////////////////////////////////////*/
uint32  Action_NumInputMenuCfg(uint16 x,uint16 y,uint32 from_window)
	{
       uint32 i,Key_Code,a,b;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	  
	    for(i=0;i<15;i++)
	    {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	    }
	  
	   //////////////////////////////////////////////////////////�а�������
	   if((i<15)&&(Key_Status == KEY_PRESS))
		{                              //��ס����ʾ
 	      //���ӷ�����ʾ ֱ�������ɿ�
		 a = Key[i].X_Position - NUMBER_WIDTH/2;
		 b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 
		 Set_Chr_Color(COL_BLACK,COL_THINGRAY);
		 Set_ASC_Lib(Arial16B_Font);
		 Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,3,COL_THINGRAY);
	
		 Key_Code = 0xaa55;
		  switch(i)
		   {
		    case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			 Set_ASC_Lib(Arial20B_Font);
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-10,0x30+i);   
		     break;
            case 10:  //�˸�
			
		     Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("�˸�","BACK"));   
		     break;
		    case 11:  //.
			       
			         Set_ASC_Lib(Arial20B_Font);
		             Put_Char(190, 310,'.'); 
		                
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("�˸�","BACK"));   
		     break;
		    case 13:  //����
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("����","ESC"));   
		     break;
		    case 14:  //ȷ��
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("ȷ��","OK"));   
		     break;
		    default:break;
		   }
		 }
		///////////////////////////////////////////
		 else if(Key_Status == KEY_DO_START)
		{ 
		 Key_Status = KEY_DO_OVER; 
		 Key_Valid_Sound();                          
 	     //���ӷ�����ʾ ֱ�������ɿ� 
		 if(i<15)
		 { 
		  a = Key[i].X_Position - NUMBER_WIDTH/2;
		  b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 
		  Set_Chr_Color(COL_WHITE,COL_BLACK);
		  Set_ASC_Lib(Arial16B_Font);
		  Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,3,COL_BLACK); 
		 
		  switch(i)
		   {
		    case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			 Set_ASC_Lib(Arial20B_Font);
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-10,0x30+i);   
		     break;
            case 10:  //�˸�
			
		     Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("�˸�","BACK"));   
		     break;
		    case 11:  //.
			      if(NUM_WEI_INPUT == from_window)
			       {
			         Set_ASC_Lib(Arial20B_Font);
		             Put_Char(190, 310,'.'); 
		            }    
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("�˸�","BACK"));   
		     break;
		    case 13:  //����
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("����","ESC"));   
		     break;
		    case 14:  //ȷ��
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("ȷ��","OK"));   
		     break;
		    default:break;
		   }
		 }  
	   } 
	   return(Key_Code);
	}
//**********************************************************************
//28����  ��:NumberInput_Interface
//��������:�������� ����ĳ�ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void NumberInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[15][2];
    //static  const uint32 display_code[] = {};
    //Create window
    Create_Window_Frame(150-100,80,500,300,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(110,85,Language_Str("������","Please Input")); 
    
    Clr_Bar(300-100,120,200,32,COL_THINGRAY);    //������ʾ��
    
    for(i=0;i<5;i++)
    Put_Button_Bar(60+100*i,160,60+100*i+80,160+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(60+100*i,230,60+100*i+80,230+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(60+100*i,300,60+100*i+80,300+60,3,COL_BLACK);
   
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Data( 90+100*i, 180,"%d",i);
    for(i=0;i<5;i++)
    Put_Data( 90+100*i, 250,"%d",5+i);
    Put_Char(190, 310,'.');
   
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(75, 320,Language_Str("�˸�", "BACK"));
    //Put_Line_Str(380, 270,Language_Str("��", "NULL"));
    Put_Line_Str(380, 320,Language_Str("����", "ESC"));
    Put_Line_Str(480, 320,Language_Str("ȷ��", "OK"));
    //create button list
    for(j=0;j<3;j++)
     { for(i=0;i<5;i++)  
        {                //start+off+
         xy[i+5*j][0] =   60 + 100*i + 80/2;   
         xy[i+5*j][1] =  160 +  70*j + 60/2;
        }
     }
    ////////////////////init variable 
    for(i=0;i<7;i++)
     num_buf[i]  = 0;
     Num_Input_tmp   = 0.0;
     Num_Input_Pos = 0;  
               
    Create_ButtonList(xy,15);               
}
//**********************************************************************
//29����  ��:NumInput_Key_Pro
//��������:�������� ����İ�������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 NumInput_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    uint8   buf[8];
    op_id = op;         //first
    i = Action_NumInputMenuCfg((key_c>>16)&0xffff,key_c&0xffff,NUM_WEI_INPUT); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
    	case 2:
       	case 3:
   		case 4:
   		case 5:
   	    case 6:
    	case 7:
       	case 8:
   		case 9:
   		case 10:
   		case 11:  //back    
   		case 12:  //.
   		      Get_NumInput_Tmp(i-1);
   		      break;
   		case 13:   //null
   	          break;
   		case 14:   //return
   		      op_id = MENU_OP_OPER;   //�������˵�
   		      break;
   		case 15:   //ok   
   		       switch(current_mode)  //��������ģʽ�������ĸ�����
   		       {
   		        case  STAT_PCS:
   		                      if((Num_Input_tmp <= WPS_MAX)&&(Num_Input_tmp >= WPS_MIN))  //���������ķ�Χ
   		                          weight_per_pcs = Num_Input_tmp;
   		                      else
   		                      Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);   
   		                      break;
   		        case  STAT_100:
   		                      if((Num_Input_tmp <= MAX_100)&&(Num_Input_tmp >= MIN_100))  //��׼��ķ�Χ
   		                          weight_compare_100 = Num_Input_tmp;
   		                      else
   		                      Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);    
   		                      break;
   		        case  STAT_CHECK:
   		                      if((Num_Input_tmp <= CHECK_MAX)&&(Num_Input_tmp >= CHECK_MIN))  //�����޵ķ�Χ
   		                        {
   		                         if(abs(Num_Input_tmp-limit1_weight)<abs(Num_Input_tmp-limit2_weight))//��ǰ���뿿���ĸ���
   		                           {
   		                             if(Num_Input_tmp > limit1_weight)
   		                             limit2_weight = Num_Input_tmp;
   		                             else
   		                             {
   		                              limit2_weight = limit1_weight;
   		                              limit1_weight = Num_Input_tmp;
   		                             }
   		                            }
   		                          else
   		                          {
   		                             if(Num_Input_tmp < limit2_weight)
   		                             limit1_weight = Num_Input_tmp;
   		                             else
   		                             {
   		                              limit1_weight = limit2_weight;
   		                              limit2_weight = Num_Input_tmp;
   		                             }
   		                           } 
   		                        }
   		                        else
   		                        Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);     
   		                      break;
   		       case  STAT_TARGET:
   		                      if((Num_Input_tmp <= TARGET_MAX)&&(Num_Input_tmp >= TARGET_MIN))  //Ŀ��ֵ�ķ�Χ
   		                        {     
   		                         target_weight = Num_Input_tmp;
   		                        }
   		                        else
   		                        Log_Alm(&tips_invalid_data,ALM_LEV_TIPS); 
   		                      break;
   		       case  STAT_DENSITY:
   		                      if((Num_Input_tmp <= DENSITY_TEMP_MAX)&&(Num_Input_tmp >= DENSITY_TEMP_MIN))  //ˮ�¶ȵķ�Χ
   		                        {     
   		                         density_temp = Num_Input_tmp;
   		                        }
   		                        else
   		                        Log_Alm(&tips_invalid_data,ALM_LEV_TIPS); 
   		      
   		       case  STAT_DIFFER:   //�ڴ�ģʽ��������ʵ��������
   		                      if((Num_Input_tmp > 90)&&(Num_Input_tmp < 110))  //��ʵ��������
   		                        {     
   		                         //
   		                         inner_cal_real.rec_float =  Num_Input_tmp;
   		                         buf[0] = (uint8)( inner_cal_real.rec_uint32>>24      );
  	                             buf[1] = (uint8)((inner_cal_real.rec_uint32>>16)&0xff);
  	                             buf[2] = (uint8)((inner_cal_real.rec_uint32>>8) &0xff);
  	                             buf[3] = (uint8)( inner_cal_real.rec_uint32     &0xff);
  	                             buf[4] = REC_CHECK_DATA1;
  	                             buf[5] = REC_CHECK_DATA2;
  	                             buf[6] = 0x00;
  	                             buf[7] = 0x00;

  	                             if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	                             Write_EEPROM(EEP_INNER_CAL2_ADDR, buf, 8);
  	                             else
  	                             Write_EEPROM(EEP_INNER_CAL1_ADDR, buf, 8);
   		                        }
   		                         
   		                      break;                   
   		       default:break;
   		       }
   		       op_id =  MENU_OP_OPER; //�������˵�
   		      break;
   	  	default:break;
   	   }
   	  //Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
///////////////////////////////////////////////////
//30  С�������봦��
///////////////////////////////////////////////////
//С���㴦��
void   dot_key_process(void)
   {
    uint32 i;
    
    if(0 == Num_Input_Pos)
      {
       for(i=0;i<7;i++)
       num_buf[i] = 0;
       Num_Input_Pos = 3;
      }
     /*else if(1 == Num_Input_Pos)
      {//��ʾ�Ѿ�������1������
       num_buf[2] = num_buf[0];
       Num_Input_Pos = 3;
      }
     else if(2 == Num_Input_Pos) 
      {//��ʾ�Ѿ�������1������
       num_buf[2] = num_buf[1];
       num_buf[1] = num_buf[0];
       num_buf[0] = 0;
       Num_Input_Pos = 3; 
      }*/
     else
     {
      Num_Input_Pos = 3; 
     }
   }
///////////////////////////////////////////   
// 31 �˸�����봦��
//////////////////////////////////////////
void   back_key_process(void)
	{
	 uint32 i;
     if(TRUE == num_buf_full) //ĩβ
       {
        Num_Input_Pos = 6;
        num_buf[6] = 0;
        num_buf_full = FALSE;
       }
     else if(Num_Input_Pos > 3)
       { //С�������˸���
        num_buf[Num_Input_Pos-1] = 0;
        Num_Input_Pos--; 
       }
     else 
      {
       if(3 == Num_Input_Pos)
         {
          num_buf[2] = num_buf[1];
          num_buf[1] = num_buf[0];
          num_buf[0] = 0;
          Num_Input_Pos--;
         }
  	   else if(2 == Num_Input_Pos)
         {
          num_buf[2] = num_buf[1];
          num_buf[1] = 0;
          num_buf[0] = 0;
          Num_Input_Pos--;
  	     } 
  	   else if(1 == Num_Input_Pos)
         {
          num_buf[2] = 0;
          num_buf[1] = 0;
          num_buf[0] = 0;
          Num_Input_Pos--;
         }
       else
       {;}
       //////////////////////////////////
       if((0 == num_buf[0])&&(0 == num_buf[1])&&(0 == num_buf[2]))
       Num_Input_Pos =0;   
     }    
  	}	
////////////////////////////////////////////////
//32  �õ��������ݵ���ʱ����
///////////////////////////////////////////////
void  Get_NumInput_Tmp(uint32 i)
{
 uint32 tmp_var;
 if(11 == i)                //С����
    dot_key_process(); 
    else if(10 == i)        //�˸�
    back_key_process();
    else
    {   //1-10 ���ּ�
      if(0 == Num_Input_Pos)
        { //��һ������
         Num_Input_Pos++;
         num_buf[0] = 0;
         num_buf[1] = 0;
         num_buf[2] = i;
        }
      else if(1 == Num_Input_Pos)
        {
         Num_Input_Pos++;
         num_buf[0] = 0;
         num_buf[1] = num_buf[2];
         num_buf[2] = i; 
        }
      else if(2 == Num_Input_Pos)
        {
         Num_Input_Pos++;
         num_buf[0] = num_buf[1];
         num_buf[1] = num_buf[2];
         num_buf[2] = i;       
        }   
      else   
        num_buf[Num_Input_Pos++] = i;
     ////////////////////////////////// 
     //�ж��Ƿ�������7λ��
     if(7 == Num_Input_Pos)
       {
        num_buf_full = TRUE;
        Num_Input_Pos = 6;
       }
      else
        num_buf_full = FALSE;
    }
    
  ////////////////////////////////////update tmp
  Num_Input_tmp = num_buf[0]*100+num_buf[1]*10+num_buf[2]+num_buf[3]*0.1+num_buf[4]*0.01+num_buf[5]*0.001+num_buf[6]*0.0001;
}
/////////////////////////////////////////4444444444444444444444444444444
////////////////////////////////////////////////33333
/*///////////////////////////////////////////////////
/* 33 �������� ��ʾ����
//////////////////////////////////////////////////*/
void PCSInput_Disp_Pro(void)
{
   static uint32 display_tmp;
   if(PCSInput_tmp != display_tmp)
   { 
    display_tmp = PCSInput_tmp;
    Clr_Bar(260,120,100,30,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    Set_ASC_Lib(Arial20B_Font);
    Put_Data(260,     125, "%4d",PCSInput_tmp);
   }
 }
/*///////////////////////////////////////////////////
/*34  �������밴�����ж�
/*
//////////////////////////////////////////////////*/
uint32  Action_PCSInputMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<15;i++)
	   {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   return(Key_Code);
	}
//**********************************************************************
//35����  ��:PCSInput_Interface
//��������:������������ ��ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void PCSInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[15][2];
    //static  const uint32 display_code[] = {};
    //Create window
    Create_Window_Frame(150-100,80,500,300,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(110,85,Language_Str("������","Please Input"));
    
    Clr_Bar(300-100,120,200,32,COL_THINGRAY);    //������ʾ��
    
    for(i=0;i<5;i++)
    Put_Button_Bar(160-100+100*i,160,160-100+100*i+80,160+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160-100+100*i,230,160-100+100*i+80,230+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160-100+100*i,300,160-100+100*i+80,300+60,3,COL_BLACK);
    
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Data( 190-100+100*i, 180,"%d",i);
    for(i=0;i<5;i++)
    Put_Data( 190-100+100*i, 250,"%d",5+i);
    
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(175-100, 320,Language_Str("�˸�", "BACK"));
    //Put_Char(280, 250,'.');
    //Put_Line_Str(280, 270,Language_Str("��", "NULL"));
    //Put_Line_Str(380, 270,Language_Str("��", "NULL"));
    Put_Line_Str(Language_Pos(480-100,480-100), 320,Language_Str("����", "ESC"));
    Put_Line_Str(580-100,                   320,Language_Str("ȷ��", " OK"));
    //create button list
    for(j=0;j<3;j++)
     { for(i=0;i<5;i++)  
        {                //start+off+
         xy[i+5*j][0] =   160-100 + 100*i + 80/2;   
         xy[i+5*j][1] =   160 +  70*j + 60/2;
        }
     }
    ////////////////////init variable 
    PCSInput_tmp = 0;
               
    Create_ButtonList(xy,15);               
}
//**********************************************************************
//36����  ��:PCSInput_Key_Pro
//��������:�������� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 PCSInput_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint8  eeprom_tx_buf[4];
    uint32 i;
    float  x,y;
    op_id = op;         //first
    i = Action_NumInputMenuCfg((key_c>>16)&0xffff,key_c&0xffff,NUM_PCS_INPUT); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
    	case 2:
       	case 3:
   		case 4:
   		case 5:
   	    case 6:
    	case 7:
       	case 8:
   		case 9:
   		case 10:
   		case 11:  //back    
   		//case 12:  //.
   		      Get_PCSInput_Tmp(i-1);
   		      break;
   		case 13:   //null
   	          break;
   		case 14:   //return
   		        if(PASSWORD_INPUT_ID == op_id)  //������ɼ��׶�
                 {;}
                else
   		        op_id = MENU_OP_OPER;   //�������˵�
   		      break;
   		case 15:   //ok  
   		       if(PASSWORD_INPUT_ID == op_id)  //������ɼ��׶�
   		        op_id = PCSInput_tmp;
   		        else
   		        {
   		          switch(which_para)
   		            {
   		             case DATA_WEIGHT_PER_PCS:
   		                  if((TRUE==positive_flag)&&(TRUE == stable_flag))
   		                     {
   		                      x = net_weight/PCSInput_tmp;
   		                      if((PCSInput_tmp<=PCS_IN_MAX)&&(PCSInput_tmp>=PCS_IN_MIN)&&(x>=WPS_MIN)&&(x<=WPS_MAX))  //PCS�ķ�Χ
   		                         weight_per_pcs = x;
   		                      else
   		                         Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);
   		                     } 
   		                   else
   		                      Log_Alm(&tips_bad_envir,ALM_LEV_TIPS); //����������
   		                                      break;  
   		             case DATA_BAIFENBI_LIMIT:
   		                  if((PCSInput_tmp<=CHECK_LIMIT2_MAX)&&(PCSInput_tmp>=CHECK_LIMIT2_MIN))  //�����ްٷֱ�
   		                    {
   		                      x = limit2_weight * (100-PCSInput_tmp)/100;
   		                      y = limit2_weight * (100+PCSInput_tmp)/100;
   		                      if((x >= CHECK_MIN)&&(y <= CHECK_MAX))
   		                       {
   		                        limit1_weight = x;
   		                        limit2_weight = y;
   		                       }
   		                      else
   		                        Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);        
   		                    }
   		                  else
   		                        Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);  
   		                                      break;
   		             case DATA_ANIMAL_TIME:
   		                  if((PCSInput_tmp <= ANIMAL_TIME_MAX)&&(PCSInput_tmp>0))  //��Ӧʱ��
   		                      animal_time = PCSInput_tmp;
   		                  else  
   		                      Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);
   		                                      break;
   		             case DATA_PASSWORD:
   		                               if(PCSInput_tmp>9999)
   		                                 Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);
   		                               else  
   		                                 {
   		                                  password = PCSInput_tmp; //save
   		                                  eeprom_tx_buf[0] = (uint8)(password&0xff);
  	                                      eeprom_tx_buf[1] = (uint8)((password>>8)&0xff);
  	                                      eeprom_tx_buf[2] = (uint8)((password+REC_CHECK_DATA1)&0xff);
  	                                      eeprom_tx_buf[3] = (uint8)(((password+REC_CHECK_DATA1)>>8)&0xff); 
  	                                      Write_EEPROM(EEP_PASSWORD_ADDR,eeprom_tx_buf, 4);
  	                                     } 
   		                                 break;
   		             case DATA_USER_ID:
   		                               if(PCSInput_tmp>999)
   		                                 Log_Alm(&tips_invalid_data,ALM_LEV_TIPS);
   		                               else  
   		                                 {
   		                                  user_id = PCSInput_tmp; //save
   		                                  eeprom_tx_buf[0] = (uint8)(user_id&0xff);
  	                                      eeprom_tx_buf[1] = (uint8)((user_id>>8)&0xff);
  	                                      eeprom_tx_buf[2] = (uint8)((user_id+REC_CHECK_DATA1)&0xff);
  	                                      eeprom_tx_buf[3] = (uint8)(((user_id+REC_CHECK_DATA1)>>8)&0xff); 
  	                                      Write_EEPROM(EEP_USER_ID_ADDR,eeprom_tx_buf, 4);
  	                                     } 
   		                                 break;                             
   		             default:break;
   		            } 
   		          //////////////////////////////////////���� 
   		          op_id =  MENU_OP_OPER; //�������˵�
   		        }
   		        break;
   	  	default:break;
   	   }
   	    //Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
///////////////////////////////////////////////////
//37  �õ������������ʱ����
///////////////////////////////////////////////////
void  Get_PCSInput_Tmp(uint32 i)
{
 if((10==i)&&(PCSInput_tmp>0)) //back
    {
     PCSInput_tmp = PCSInput_tmp/10; 
    }
 else if((i<10)&&(PCSInput_tmp<1000)) //���ּ������������������9999
        {
         PCSInput_tmp =PCSInput_tmp*10+i;
        }
}
