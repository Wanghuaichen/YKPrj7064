//**********************************************************************
//文件名:normal_oprarion.c
//说  明:1.工作模式下的菜单管理
//       2.工作模式下的功能调度
//函数说明
//2   更新新数据                update_new_data(void)
//3   计算线性校准后的数据      cali_off_newdata(uint32 data_no_line)
//4   进入待机后的辅助刷新      Scr_Assi_Null
//5   主工作界面的辅助刷新函数  Oper_Assi_Pro  每300ms执行一次
//6   待机状态下按键判断        Action_SleepCfg(uint16 x,uint16 y)
//7   主界面状态下的按键判断    Action_MainCfg(uint16 x,uint16 y)
//8   强制结束校准              End_Cal_Pro
//9   附属按键处理              Addition_Key_Do(uint32 key_number,sint32 op_id)
//10  待机状态下的按键处理      Sleep_Key_Pro(uint32 key_c,sint32 op)
//11  主工作界面的按键处理      Main_Key_Pro
//12  待机模式下显示            Oper_Sleep_Pro
//13  主工作界面下的显示        Oper_Disp_Pro
//14  设置函数当前所处的界面    Set_Menu_Op
//15  待机界面的初始化          Put_Sleep_Interface
//16  主界面的初始化            Put_Oper_Interface  

/*********************************************************/
/*17  主显示状态下的调度管理函数 Normal_Operation        */
/*********************************************************/

//18  单位显示                  Unit_Display(void)
//19  稳定符号显示              Stable_Status_Display(void)
//20  运行符号显示              Mode_Run_Display
//21  显示工作状态信息          Weigh_Status_Display
//22  显示称重 新数据           New_Data_Display
//23  主显示区的数据显示        display_weight_main_area
//24  称重模式下的数据显示      display_weight_data
//25  计数模式下的数据显示      display_pcs_data
//26  百分比模式下的数据显示    display_100_data
//27  检重模式下的数据显示      display_check_data
//28  差异模式下的数据显示      display_differ_data
//29  累加模式下的数据显示      display_total_data
//30  动物模式下的数据显示      display_animal_data
//31  密度模式下的数据显示      display_density_data
//32  峰值模式下的数据显示      display_peak_data
//33  目标模式下的数据显示      display_target_data
//34  校准模式下的数据显示      display_caling_data
//**********************************************************************
#include "stdio.h"
#include "stdlib.h"

#include "eeprom.h"
#include "Font_Lib.H"
#include "Font_Lib2.H"
#include "graph.h"
#include "global.h"
#include "i2c_com.h"
#include "normal_operation.h"
#include "timer.h"
#include "type_def.h"
#include "menu.h"
#include "screencal.H"
#include "filter.h"
#include "mainmenu.h"
#include "mainmenu2.h"
#include "datecfg.h"

#include "port.h"
#include "disp.h"
#include "lcd2478.h"
#include "rtc.h"
#include "Calendar.h"
#include "lpc2478_uart.h"
#include "alarm_pro.h"

#include "epm7064s.h"

///////////////////////////////usb

//function1
static void   Unit_Display(void);  
static void   Stable_Status_Display(void);
static void   Mode_Run_Display(void);
static void   Weigh_Status_Display(void);
static void   New_Data_Display(void);

static void   display_caling_data(void);
static void   display_weight_data(void);
static void   display_pcs_data(void);
static void   display_100_data(void);

static void   display_check_data(void);
static void   display_differ_data(void);
static void   display_total_data(void);
static void   display_animal_data(void);
static void   display_density_data(void);
static void   display_peak_data(void);
static void   display_target_data(void);
static void   display_caling_inner_data();

static uint32 manual_zer0_data;

static void   point2_cal_proc(uint32 weight_tmp);
static void   point2_cal_proc_inner(uint32 weight_tmp);

static sint32 Addition_Key_Do(uint32 key_number,sint32 op_id);    //第几个键 ，总共3个,返回哪个窗口
//function2
static void   Put_Oper_Interface(sint32 op);
static sint32 Main_Key_Pro(uint32 key_c,sint32 op);
static void   Oper_Disp_Pro(void);
static void   Oper_Assi_Pro(void);

//static sint32   menu_op;
static uint32   reflash_flag;  //满量程后 刷屏幕

//////////////////////////////////////////////////////////
//0A 进入2点校准状态
//////////////////////////////////////////////////////////
void Enter_2Point_Cal(void)
   {
    if(FALSE == point2_cal_start)
     {
      current_mode     = STAT_CALING;
      point2_cal_start = TRUE;
      point2_cal_flag  = CAL_MODE_OUT;
      point2_cal_step  = USER_CAL_ZER0;
      Set_Zer0_Flag();
      stable_manual_break();    
     }
   }
//////////////////////////////////////////////////////////
//0A 进入2点内部校准状态
//////////////////////////////////////////////////////////   
void Enter_2PointInner_Cal(void)
   {
   
    if((FALSE == point2_cal_start) && (0 == motor_err_flag))
     {
       motor_err_flag = motor_run_proc(UNLOAD_POSITION);
       if(0 == motor_err_flag)
       {
        current_mode     = STAT_CALING_INNER;
        point2_cal_start = TRUE;
        point2_cal_flag  = CAL_MODE_INNER;
        point2_cal_step  = USER_CAL_ZER0;
        Set_Zer0_Flag();
        stable_manual_break();
       }     
     }
    else 
     {//提示信息：校准中 或者 马达错误
      Log_Alm(&tips_cal_stop,ALM_LEV_TIPS);
     }
   }   
//**********************************************************************
//0函数 名:Wireless_Process()
//函数功能:无线信号的检测与判断处理 每300ms执行一次
//入口参数:无
//出口参数:无
//**********************************************************************
static sint32 Wireless_Process(sint32 op)
{
  static  uint32 signal1_run_time,signal1_avai_flag,signal2_run_time,signal2_avai_flag;
  sint32 op_tmp;
  op_tmp = op;
  
   ///////////////////////////////WIRELESS 2   左边 
if(0==(FIO0PIN & WIRELESS_RCV_PIN2))
    {
     if(signal1_avai_flag == 1)
       {  //认为是有效输入信号
        no_key_time = 0;
        FIO0CLR = WIRELESS_LED2; //ON
        signal1_avai_flag = 0;
        signal1_run_time = 2;
        switch(wireless_pin1_configuration)
          {
           case FUNCTION_QP:
                       Set_Tare_Flag();   //去皮操作
                       break;
           case FUNCTION_ZER0:
                       Set_Zer0_Flag();  
                       break;
           case FUNCTION_CAL:
                       Enter_2Point_Cal();
                       break;
           case FUNCTION_IDLE:
                if( MENU_SLEEP_OP_OPER == op) //当前处于休眠状态
                 {
                  Set_LCD_Brightness(lcd_brightness); //返回到正常亮度
                  op_tmp = MENU_OP_OPER;
                 }
                else
                  op_tmp = MENU_SLEEP_OP_OPER;
                  break; 
           case FUNCTION_INCAL:
                       if(INNER_CAL_VERSION)
                          Enter_2PointInner_Cal();
                       break;                        
           default:break;
          }
       }
       else
       {
        if(0!=signal1_run_time)
            signal1_run_time--;
        else    
            {
             FIO0SET = WIRELESS_LED2;
             //Key_Valid_Sound();
            }
       }
     }
    else
    { //做有效标志
     FIO0SET = WIRELESS_LED2;
     if(FUNCTION_NULL!=wireless_pin1_configuration)
     signal1_avai_flag = 1;
     
    }
    
  ///////////////////////////////WIRELESS 右边
  if(0 == (FIO0PIN & WIRELESS_RCV_PIN1))//无遮挡
    {
     //FIO0CLR = WIRELESS_LED1;
     if(signal2_avai_flag == 1)
       {  //认为是有效输入信号
        no_key_time = 0;
        FIO0CLR = WIRELESS_LED1;   
        signal2_avai_flag = 0;
        signal2_run_time = 2;
        switch(wireless_pin2_configuration)
          {
           case FUNCTION_QP:
                       Set_Tare_Flag();   //去皮操作
                       break;
           case FUNCTION_ZER0:
                       Set_Zer0_Flag();  
                       break;
           case FUNCTION_CAL:
                       Enter_2Point_Cal();
                       break;
           case FUNCTION_IDLE:
                if( MENU_SLEEP_OP_OPER == op) //当前处于休眠状态
                 {
                  Set_LCD_Brightness(lcd_brightness); //返回到正常亮度
                  op_tmp = MENU_OP_OPER;
                 }
                else
                  op_tmp = MENU_SLEEP_OP_OPER;
                  break; 
           case FUNCTION_INCAL:
                       if(INNER_CAL_VERSION)
                          Enter_2PointInner_Cal();                        
           default:break;
          }
        }
      else
       {   
        if(0!=signal2_run_time)
           signal2_run_time--;
        else   
           FIO0SET = WIRELESS_LED1;
       }
     }
    else
    { //有遮挡
     FIO0SET = WIRELESS_LED1;
     if(FUNCTION_NULL!=wireless_pin2_configuration)
     signal2_avai_flag = 1;
    }

/////////////////////////////////return 
return(op_tmp);   
}

////////////////////////////////////
//修正原则 对200.000
//偏离 < 2个
//偏离 < 3个回2个
//偏离 < 5个回3个
/////////////////////////////////
static  float display_to_int(float net_tmp)
 {
  uint32 tmp,tmp2;
  ////////////////////////////以下几种情况下失效
  //1mg 量程状态下
  //净重量 很小的时候
  //密码非7887的时候
  //识别非趋向于 1g (0.1g)的时候
  //其它非正常状态
  ///////////////////////////////////////////////
  if((QIAN_DIVISION==dot_position)||(net_tmp < 0.01)||(7887!=password))
    return(net_tmp);
 
  if(WAN_DIVISION == dot_position)   
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
  else if(WAN10_DIVISION == dot_position) 
     {
      tmp  = (uint32)(net_tmp * 100000);
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
     }
  else
        return(net_tmp);   
 }

//////////////////////////////////////////////////////////
//2更新 新数据 的步骤
//(1) 首先得到减去0位的 纯重量内码
//(2) 自动跟踪处理 开机置零 手动置零 后重新得到纯重量内码
//(3) 计算线性调教后的数据
//(4) 载荷稳定后的数据处理
//(5) 去皮处理
//(6) 得到净重 以及 符号
//////////////////////////////////////////////////////////
void    update_new_data(void)
	{
	 float  net_tmp;
	 uint32 weight1,weight2,weight_tmp;
	 //////////////////////////////////////////////////
   	 //////////////////////////////////////////////////	  
     weight1 = Get_Filter_Data();                //
     
     debug_disp_data1 = weight1;
     
     //
     if(TRUE == point10_cal_start)          //如果是在线性校准过程中
        //point4_cal_proc(weight1);    
        point10_cal_proc(weight1);
     
      /////////////计算偏移量，得到线性校准后的重量数据
      if(TRUE == point10_cal_ok)
         //weight_tmp = Get_Line_data(weight1);  
          weight_tmp = Get_Line_data_10(weight1); 
      else
          weight_tmp = weight1;
     //////////////////////////////////////////第一种情况 自动跟踪 可能修改0位   
     debug_disp_data2 = weight_tmp;
     
     if(1 == flag_auto_track_enable)
        auto_zer0(weight_tmp);                         
     //////////////////////////////////////////第二种情况 手动置零 可能修改0位
     if((TRUE == do_zer0_flag)&&(TRUE==stable_flag))
      {
       do_zer0_flag = FALSE;
       if(abs(weight_tmp-manual_zer0_data)<((full_code*USER_ZER0_MAX)/100)) 
          {
           zer0_data = weight_tmp;
           tare_data = 0;
          }
       else 
         {
         }                    
      }      
     ///////////////////////////////////////////第三种情况 开机置零 可能修改0位
     if((TRUE == power_on_flag)&&(TRUE == stable_flag))
     {
        power_on_clr_cnt--;
      if(0 == power_on_clr_cnt)
         {
          power_on_flag = FALSE;
          if(abs(weight_tmp-zer0_data) < full_code/START_ZER0_MAX) //开机置零范围
             {
              zer0_data = weight_tmp;                 //开机置零成功
             }
          /////////////////////////////
          manual_zer0_data = zer0_data;      //不管开机置零是否成功，都要设置手动置零的基点             
         } 
      }    
     //////////////////////////////////////////////////////////
      if(TRUE == point2_cal_start)              //用户校准 使用的是线性校准后的数据
         {
          if(CAL_MODE_OUT == point2_cal_flag)
            point2_cal_proc(weight_tmp);
          else 
            point2_cal_proc_inner(weight_tmp);
         }
     //////////////////////////////////////////////////////////
     new_data_big = weight_tmp;      
     weight2 = abs(weight_tmp - zer0_data);    //减去活动 0点 的数据 
    
                                               //各种0位处理后的纯重量内码     
                                               //对线性校准后的重量数据作 稳定处理
     if((1==flag_load_track_enable) && (1==load_track_flag)) //条件满足 且 工厂模式下开启了此功能
      new_data = stable_load_zer0(weight2);    // 有重量稳定后的显示处理 包含了重复性检查
     else
      new_data = weight2;
    ///////////////////////////////
    //////////////////////////////////是否有去皮要求 零位以下不允许去皮
    if((TRUE == do_tare_flag)&&(TRUE==stable_flag)&&(weight_tmp>zer0_data))
      {
       do_tare_flag = FALSE;
       tare_data  = new_data;
      }
          
     //计算各个重量
    //////////////////////////////////////////////////////////////////////////////
    if(TRUE == point2_cal_ok)               //已经用户校准了
      {//判断正负以及 最后的显示(考虑零位以下的净重处理)
         gross_weight  = weigh_coef1  * new_data;    //经过处理的毛重
         tare_weight   = weigh_coef1  * tare_data;   //去皮重    
    
        if(weight_tmp < zer0_data) 
          {
           positive_flag  = FALSE;
           net_tmp     = gross_weight + tare_weight ;
          }
        else
          {
           if(gross_weight < tare_weight)
             {
              positive_flag = FALSE;
              net_tmp     = tare_weight - gross_weight ;
             }
           else        
             {
              positive_flag = TRUE;
              net_tmp = gross_weight - tare_weight  ;
             }
          }
         //////////////////////////////////////重量显示拦截 不改变 内码
         ////////////////////////////////////??êy?? ′|àí
		 if(QIAN_DIVISION == dot_position)	
		    net_tmp += 0.0005;
         else
		    net_tmp += 0.00005;
         net_weight = display_to_int(net_tmp);
         if(net_weight < display_min_value)
            net_weight = 0.0;
       				 /////////////////////////////////////////////?ù?Y???? oí????μ?μ?ê?·?ò?3?
	     //if((net_weight>weight_alarm)||(gross_weight>weight_alarm))
         //   full_flag = TRUE;
		 //else
		 //   full_flag = FALSE;
       
       }
    else
      {
       gross_weight   = new_data;  
       net_weight     = new_data;
      }
    /////////////////////////////////////over
   }    
	


////////////////////////////////////////////////////////////////
//4
//
////////////////////////////////////////////////////////////////
static void Scr_Assi_Null(void)
{
 ///////////////////
 //Wireless_Process(); 
}
////////////////////////////////////////////////////////////////
//5a 显示报警信息
//
////////////////////////////////////////////////////////////////
void  display_alarm_info(uint32 flash_flag)
 { 
  Clr_Bar(120,5,360,65,COL_THINBLUE);    //clr info
  Set_ASC_Lib(Arial16B_Font);
/* if(1 == flash_flag)
    {                                   //display info if needed
     switch(Warning_Info_Flag)
      {
       case OVER_LOAD:
                         Clr_Bar(120+180-100,35,200,30,COL_DEEPRED),
                         Set_Chr_Color(COL_WHITE,COL_DEEPRED);
                         Put_Line_Str(240,38,Language_Str("载荷超重","  OVER LOAD"));    
                         break;
       case WPP_TOO_LOW:  
                         Clr_Bar(120+180-100,35,200,30,COL_DEEPYELLOW),
                         Set_Chr_Color(COL_BLACK,COL_DEEPYELLOW);
                         Put_Line_Str(Language_Pos(230,240),38,Language_Str("单件重量太小","LOW SAMPLE")); 
                         break;
       case COMP_TOO_LOW:  
                         Clr_Bar(120+180-100,35,200,30,COL_DEEPYELLOW),
                         Set_Chr_Color(COL_BLACK,COL_DEEPYELLOW);
                         Put_Line_Str(240,38,Language_Str("基准点太低","LOW BENCHMARK")); 
                         break;
       case PCS_NUM_OVER: 
                         Clr_Bar(120+180-100,35,200,30,COL_DEEPYELLOW),
                         Set_Chr_Color(COL_BLACK,COL_DEEPYELLOW);
                         Put_Line_Str(230,38,Language_Str("称重数目超范围","PCS NUMBER OVER")); 
                         break;
       case CHECK_OVER: 
                         Clr_Bar(120+180-100,35,200,30,COL_DEEPYELLOW),
                         Set_Chr_Color(COL_BLACK,COL_DEEPYELLOW);
                         Put_Line_Str(240,38,Language_Str("超出检重范围","CHECK OVER")); 
                         break;                         
    
       default:            break;
      }
    }  
    else  /////////////////////////////////
     {
      switch(Error_Info_Flag)
       {
        case ERROR_TYPE_TEMP:
                         Clr_Bar(120,5,360,30,COL_DEEPRED);
                         Set_Chr_Color(COL_WHITE,COL_DEEPRED);
                         Put_Line_Str(130, 8,Language_Str("环境温度变化较大逗请重新校准","Please ReCal Because Of Temp")); 
                         break;
        default:           break; 
       }
     }  */
 } 
///////////////////////////////////////////////////////////////////////
//**********************************************************************
//5b函数 名:Oper_Main_Assi_Pro
//函数功能:主工作界面的辅助处理函数  每300ms执行一次
//入口参数:无
//出口参数:无
//**********************************************************************
static void Oper_Main_Assi_Pro(void)
{
 RTCTime local_time_buf;
 uint16   i;
 
 ///////////////////
 if(Sec_Change_Flag)
 {
   Sec_Change_Flag = FALSE;
   local_time_buf = RTCGetTime();
   Set_ASC_Lib(Arial20B_Font);
   Set_Chr_Color(COL_WHITE,COL_THINBLUE);
    
     Put_Data(515-30,     5, "%04d", local_time_buf.RTC_Year);
	 Put_Char(485+4*15+3, 5, 0x2d);
	 Put_Data(560,        5, "%02d", local_time_buf.RTC_Mon);
	 Put_Char(485+7*15+3, 5, 0x2d);
	 Put_Data(605,        5, "%02d", local_time_buf.RTC_Day);
     //display time
	 Put_Data(515,        35,"%02d", local_time_buf.RTC_Hour);
	 Put_Char(485+4*15+3, 35,0x3a);
     Put_Data(560,        35,"%02d", local_time_buf.RTC_Min);
	 Put_Char(485+7*15+3, 35,0x3a);
	 Put_Data(605,        35,"%02d", local_time_buf.RTC_Sec);
	 
	 //首先判断故障,如果故障 就把温度设置为0
	 if(TEMP_SENSOR_NC_VALUE == temp_float)
	 {
	  temp_float = 0.0;
	  Log_Alm(&warn_DS18B20_err,ALM_LEV_WARN);
	 }
	 else  
	 {
	  Del_Alm(&warn_DS18B20_err,ALM_LEV_WARN);
	 }

	 Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	 Set_ASC_Lib(Arial16B_Font);
	 if((temp_float < 100)&&(temp_float > 1))
	 Put_Data_Float(13,285,"%4.1f",temp_float);
	 
	 
	 //判断是否超范围
	 if((temp_float > ENV_TEMP_WARING) && (TRUE==temp_cmp_flag))
	 Log_Alm(&warn_temp_out,ALM_LEV_WARN);
	 else  
	 Del_Alm(&warn_temp_out,ALM_LEV_WARN);
	 //
	 if((abs(temp_cmp_point-temp_float) > TEMP_CHANGE_WARING) && (TRUE==temp_cmp_flag))
	 Log_Alm(&warn_temp_c_out,ALM_LEV_WARN);
	 else
	 Del_Alm(&warn_temp_c_out,ALM_LEV_WARN);
	 ////////////////////////////////////////
	 //出现此报警信息，只能关机重新启动，如果还出现，则加卸载机构出现故障
	 if(INNER_CAL_VERSION&&(1==motor_err_flag)) 
	  Log_Alm(&warn_loadtime_out,ALM_LEV_WARN);
	 
   }
   Put_Alm_Mess();
}

///////////////////////////////////////////////////////////////////////
//**********************************************************************
//5函数 名:Oper_Assi_Pro
//函数功能:主工作界面的辅助处理函数  每300ms执行一次
//入口参数:无
//出口参数:无
//**********************************************************************
static void Oper_Assi_Pro(void)
{
 RTCTime local_time_buf;

 if(Sec_Change_Flag)
 {
   Sec_Change_Flag = FALSE;
   local_time_buf = RTCGetTime();
   /*/Updata_USB_Memory();
   sprintf(&Memory[DATA_ADDR_IN_MEMORY + 16],   "%4d",local_time_buf.RTC_Year);
   sprintf(&Memory[DATA_ADDR_IN_MEMORY + 16+4], "%2d",local_time_buf.RTC_Mon);
   sprintf(&Memory[DATA_ADDR_IN_MEMORY + 16+6], "%2d",local_time_buf.RTC_Day);
   sprintf(&Memory[DATA_ADDR_IN_MEMORY + 16+8], "%2d",local_time_buf.RTC_Hour);
   sprintf(&Memory[DATA_ADDR_IN_MEMORY + 16+10],"%2d",local_time_buf.RTC_Min);
   sprintf(&Memory[DATA_ADDR_IN_MEMORY + 16+12],"%2d",local_time_buf.RTC_Sec);
   //时分秒：5:6:5(*2) 年月日：7:4:5，年取0-119对应1980 - 2099  
   i = (local_time_buf.RTC_Year - 1980)<<9;
   i = local_time_buf.RTC_Mon<<5;
   i = (i&0xffe0)|local_time_buf.RTC_Day;
   Memory[UPDATE_DATE_ADDR]    = (uint8)i;
   Memory[UPDATE_DATE_ADDR+1]  = (i>>8)&0xff;
   i = local_time_buf.RTC_Hour << 11;
   i = local_time_buf.RTC_Min  <<  5;
   i = (i&0xffe0)|local_time_buf.RTC_Sec/2;
   Memory[UPDATE_TIME_ADDR]    = (uint8)i ;
   Memory[UPDATE_TIME_ADDR+1]  = (i>>8)&0xff;
   /////////////////////////////////////////////////////////////*/ 
   Set_ASC_Lib(Arial20B_Font);
   Set_Chr_Color(COL_WHITE,COL_THINBLUE);
    
     Put_Data(515-30,     5, "%04d", local_time_buf.RTC_Year);
	 Put_Char(485+4*15+3, 5, 0x2d);
	 Put_Data(560,        5, "%02d", local_time_buf.RTC_Mon);
	 Put_Char(485+7*15+3, 5, 0x2d);
	 Put_Data(605,        5, "%02d", local_time_buf.RTC_Day);
     //display time
	 Put_Data(515,        35,"%02d", local_time_buf.RTC_Hour);
	 Put_Char(485+4*15+3, 35,0x3a);
     Put_Data(560,        35,"%02d", local_time_buf.RTC_Min);
	 Put_Char(485+7*15+3, 35,0x3a);
	 Put_Data(605,        35,"%02d", local_time_buf.RTC_Sec);
	 
}
 Put_Alm_Mess();
}
////////////////////////////////////////////////////
//6
//待机状态下的按键判断
//
///////////////////////////////////////////////////
static uint32  Action_SleepCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<1;i++)
	    {
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	     //////////////////////////////////////////////////////////有按键按下
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
/*7
/*主界面下的键盘判断 共有10+2+4个按键
//
/////////////////////////////////////////////////*/
static uint32  Action_MainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<11;i++)
	   {
	     if((abs((*p).X_Position-x)< 50) && (abs((*p).Y_Position-y)< 30))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	  //////////////////////////add date&time button
	  /* 不再检查 
	   for(i=0;i<2;i++)
	   {
	     if((abs((*p).X_Position-x)< 70) && (abs((*p).Y_Position-y)< 15))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }*/
	  ///////////////////////////每种模式下附带的key 4
	  for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 50) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   } 
	   
	  //////////////////////////////////////////////////////////有按键按下
	  if(Key_Status == KEY_PRESS)
	     {
	      switch(Key_Code)
	      {
	       case 1: Put_Bmp(550, 70+      12,    basicmode9060,1); break;
           case 2: Put_Bmp(550, 70+1*85+ 12,      pcsmode9060,1); break;
           case 3: Put_Bmp(550, 70+2*85+ 12,     baifenbi9060,1); break;
           case 4: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(550, 70+3*85+ 12,     moremodec9060,1); 
                   else
                   Put_Bmp(550, 70+3*85+ 12,     moremodee9060,1);
                   break;
           case 5: break;
           
           case 6: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*0,     415-80,      morecfgc9060,1); 
                   else
                   Put_Bmp(103*0,     415-80,      morecfge9060,1);
                   break;
           case 7: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*0+5,     415,      printerc9060,1); 
                   else
                   Put_Bmp(103*0+5,     415,      printere9060,1);
                   break; //单位
           
           case 8: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*1+5,     415,      unitcfgc9060,1); 
                   else
                   Put_Bmp(103*1+5,     415,      unitcfge9060,1);
                   break; //单位
           case 9: 
                   if(INNER_CAL_VERSION)
                   {
                    if(CHINESE == Get_Language_Type())
                    Put_Bmp(103*2+5,     415,  innercalc9060,1);
                    else
                    Put_Bmp(103*2+5,     415,  innercale9060,1);  
                   }
                   else
                   {
                    if(CHINESE == Get_Language_Type())
                    Put_Bmp(103*2+5,     415,  wirelesscfgc9060,1);
                    else
                    Put_Bmp(103*2+5,     415,  wirelesscfge9060,1);
                   }
                   break; //无线
           case 10: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*3+5,     415,          calc9060,1);
                   else
                   Put_Bmp(103*3+5,     415,          cale9060,1); 
                   break; //校准
           case 11:
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*4+5,     415,       recallc9060,1);
                   else
                   Put_Bmp(103*4+5,     415,       recalle9060,1); 
                   break; //亮度
           ///////////////////////////////处理辅助按键
           case 13:
                    if((STAT_CALING == current_mode)||(STAT_CALING_INNER == current_mode))
                     break;
                    else
                    Put_Bmp(100, 330,          tare9060,1);     
                     break;
           case 14:
                   switch(current_mode)
                   {
                    case STAT_WEIGH:   
                                    Put_Bmp(210, 330,zer09060,1);
                                    break; 
                    case STAT_PCS:          
                                   if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,inputWPP9060,1);
                                   else
                                    Put_Bmp(210, 330,inputWPP29060,1);
                                    break;
                    case STAT_100: 
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,inputmarkc9060,1);
                                    else
                                    Put_Bmp(210, 330,inputmarke9060,1);
                                    break;
                    case STAT_CHECK:
                                    Put_Bmp(210, 330,samplecheck129060,1);
                                    break; 
                    case STAT_DIFFER: 
                                    //Put_Bmp(210, 330,inputmark9060,1);
                                    break; 
                  
                    case STAT_TOTAL:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,totaladdc9060,1);
                                    else
                                    Put_Bmp(210, 330,totaladde9060,1);
                                    break; 
                  
                    case STAT_ANIMAL:
                                    Put_Bmp(210, 330,animaltime9060,1);
                                    break; 
                  
                    case STAT_DENSITY:
                                    Put_Bmp(210, 330,densitytemp9060,1);
                                    break; 
                  
                    case STAT_PEAK:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,animalstartc9060,1);
                                    else
                                    Put_Bmp(210, 330,animalstarte9060,1);
                                    break; 
                  
                    case STAT_TARGET:
                                    if(CHINESE == Get_Language_Type())  
                                    Put_Bmp(210, 330,inputmarkc9060,1);
                                    else
                                    Put_Bmp(210, 330,inputmarke9060,1);
                                    break; 
                  
                    default:break;
                   }
                   break;
           case 15:
                 switch(current_mode)
                   {
                    case STAT_WEIGH:
                                    Put_Bmp(320, 330,save9060,1);   
                                    break; 
                    case STAT_PCS:          
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,samplePCS9060,1);
                                    else
                                    Put_Bmp(320, 330,samplePCS29060,1);
                                    break;
                    case STAT_100: 
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,samplemarkc9060,1);
                                    else
                                    Put_Bmp(320, 330,samplemarke9060,1);
                                    break;
                    case STAT_CHECK:
                                    Put_Bmp(320, 330,samplecheck9060,1);
                                    break; 
                    case STAT_DIFFER: 
                                    //Put_Bmp(210, 330,inputmark9060,1);
                                    break; 
                  
                    case STAT_TOTAL:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,totalclearc9060,1);
                                    else
                                    Put_Bmp(320, 330,totalcleare9060,1);
                                    break; 
                  
                    case STAT_ANIMAL:
                                   // Put_Bmp(320, 330,animalstart9060,1);
                                   // break; 
                  
                    case STAT_DENSITY:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,animalstartc9060,1);
                                    else
                                    Put_Bmp(320, 330,animalstarte9060,1);
                                    break; 
                  
                    case STAT_PEAK:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,animalendc9060,1);
                                    else
                                    Put_Bmp(320, 330,animalende9060,1);
                                    break; 
                  
                    case STAT_TARGET:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,samplemarkc9060,1);
                                    else
                                    Put_Bmp(320, 330,samplemarke9060,1);
                                    break; 
                  
                    default:break;
                   }
                   break;
           case 16:
                  switch(current_mode)
                   {
                    case STAT_WEIGH:
                                    break; 
                    case STAT_PCS:          
                                    Put_Bmp(430, 330,  save9060,1); 
                                    break;
                    case STAT_100: 
                                    Put_Bmp(430, 330,  save9060,1); 
                                    break;
                    case STAT_CHECK:
                                    Put_Bmp(430, 330,  save9060,1); 
                                    break; 
                    case STAT_DIFFER: 
                                    break; 
                  
                    case STAT_TOTAL:
                                    Put_Bmp(430, 330,  save9060,1);
                                    break; 
                  
                    case STAT_ANIMAL:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(430, 330,  animalendc9060,1);
                                    else
                                    Put_Bmp(430, 330,  animalende9060,1);
                                    break; 
                  
                    case STAT_DENSITY:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(430, 330,  densitynextc9060,1); 
                                    else
                                    Put_Bmp(430, 330,  densitynexte9060,1);
                                    break; 
                  
                    case STAT_PEAK:
                                    Put_Bmp(430, 330,  save9060,1); 
                                    break; 
                  
                    case STAT_TARGET:
                                    Put_Bmp(430, 330,  save9060,1);  
                                    break; 
                  
                    default:break;
                   }
                  break;
          default:break;
           }  
          Key_Code = 0xAA55;
	     }
 	  //增加反白显示 直到按键松开 
	  if(Key_Status == KEY_DO_START)  
	    {
	     Key_Status = KEY_DO_OVER; 
	     Key_Valid_Sound();
	      switch(Key_Code)
	      {
	       case 1: Put_Bmp(550, 70+      12,    basicmode9060,0); break;
           case 2: Put_Bmp(550, 70+1*85+ 12,      pcsmode9060,0); break;
           case 3: Put_Bmp(550, 70+2*85+ 12,     baifenbi9060,0); break;
           
           case 4:
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(550, 70+3*85+ 12,     moremodec9060,0); 
                   else
                   Put_Bmp(550, 70+3*85+ 12,     moremodee9060,0);
                   break;
                   
           case 5: break;
           
           case 6: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*0,     415-80,      morecfgc9060,0); 
                   else
                   Put_Bmp(103*0,     415-80,      morecfge9060,0);
                   break;
           
           case 7: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*0+5,     415,      printerc9060,0); 
                   else
                   Put_Bmp(103*0+5,     415,      printere9060,0);
                   break;
           case 8: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*1+5,     415,      unitcfgc9060,0); 
                   else
                   Put_Bmp(103*1+5,     415,      unitcfge9060,0);
                   break; //单位
           case 9: 
                  if(INNER_CAL_VERSION)
                   {
                    if(CHINESE == Get_Language_Type())
                    Put_Bmp(103*2+5,     415,  innercalc9060,0);
                    else
                    Put_Bmp(103*2+5,     415,  innercale9060,0);  
                   }
                   else
                   {
                    if(CHINESE == Get_Language_Type())
                    Put_Bmp(103*2+5,     415,  wirelesscfgc9060,0);
                    else
                    Put_Bmp(103*2+5,     415,  wirelesscfge9060,0);
                   }
                   break; //无线
           case 10: 
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*3+5,     415,          calc9060,0);
                   else
                   Put_Bmp(103*3+5,     415,          cale9060,0); 
                   break; //校准
           case 11:
                   if(CHINESE == Get_Language_Type())
                   Put_Bmp(103*4+5,     415,       recallc9060,0);
                   else
                   Put_Bmp(103*4+5,     415,       recalle9060,0); 
                   break; //亮度
           //////////////////////////////////////////////
           ///////////////////////////////处理辅助按键
           case 13:Put_Bmp(100, 330,          tare9060,0);        break;
           case 14:
                   switch(current_mode)
                   {
                    case STAT_WEIGH:   
                                    Put_Bmp(210, 330,zer09060,0);
                                    break; 
                    case STAT_PCS:          
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,inputWPP9060,0);
                                    else
                                    Put_Bmp(210, 330,inputWPP29060,0);
                                    break;
                    case STAT_100: 
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,inputmarkc9060,0);
                                    else
                                    Put_Bmp(210, 330,inputmarke9060,0);
                                    break;
                    case STAT_CHECK:
                                    Put_Bmp(210, 330,samplecheck129060,0);
                                    break; 
                    case STAT_DIFFER: 
                                    //Put_Bmp(210, 330,inputmark9060,0);
                                    break; 
                  
                    case STAT_TOTAL:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,totaladdc9060,0);
                                    else
                                    Put_Bmp(210, 330,totaladde9060,0);
                                    break; 
                  
                    case STAT_ANIMAL:
                                    Put_Bmp(210, 330,animaltime9060,0);
                                    break; 
                  
                    case STAT_DENSITY:
                                    Put_Bmp(210, 330,densitytemp9060,0);
                                    break; 
                  
                    case STAT_PEAK:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,animalstartc9060,0);
                                    else
                                    Put_Bmp(210, 330,animalstarte9060,0);
                                    break; 
                  
                    case STAT_TARGET:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(210, 330,inputmarkc9060,0);
                                    else
                                    Put_Bmp(210, 330,inputmarke9060,0);
                                    break; 
                  
                    default:break;
                   }
                    break;
           case 15:
                 switch(current_mode)
                   {
                    case STAT_WEIGH:
                                    Put_Bmp(320, 330,    save9060,0);  
                                    break; 
                    case STAT_PCS:          
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,samplePCS9060,0);
                                    else
                                    Put_Bmp(320, 330,samplePCS29060,0);
                                    break;
                    case STAT_100: 
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,samplemarkc9060,0);
                                    else
                                    Put_Bmp(320, 330,samplemarke9060,0);
                                    break;
                    case STAT_CHECK:
                                    Put_Bmp(320, 330,samplecheck9060,0);
                                    break; 
                    case STAT_DIFFER: 
                                    //Put_Bmp(210, 330,inputmark9060,0);
                                    break; 
                  
                    case STAT_TOTAL:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,totalclearc9060,0);
                                    else
                                    Put_Bmp(320, 330,totalcleare9060,0);
                                    break; 
                  
                    case STAT_ANIMAL:
                                   // Put_Bmp(320, 330,animalstart9060,0);
                                   // break; 
                  
                    case STAT_DENSITY:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,animalstartc9060,0);
                                    else
                                    Put_Bmp(320, 330,animalstarte9060,0);
                                    break; 
                  
                    case STAT_PEAK:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,animalendc9060,0);
                                    else
                                    Put_Bmp(320, 330,animalende9060,0);
                                    break; 
                  
                    case STAT_TARGET:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(320, 330,samplemarkc9060,0);
                                    else
                                    Put_Bmp(320, 330,samplemarke9060,0);
                                    break; 
                  
                    default:break;
                   }
                   break;
           case 16:
                 switch(current_mode)
                   {
                    case STAT_WEIGH:
                                    break; 
                    case STAT_PCS:          
                                    Put_Bmp(430, 330,  save9060,0); 
                                    break;
                    case STAT_100: 
                                    Put_Bmp(430, 330,  save9060,0); 
                                    break;
                    case STAT_CHECK:
                                    Put_Bmp(430, 330,  save9060,0); 
                                    break; 
                    case STAT_DIFFER: 
                                    break; 
                  
                    case STAT_TOTAL:
                                    Put_Bmp(430, 330,  save9060,0);
                                    break; 
                  
                    case STAT_ANIMAL:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(430, 330,  animalendc9060,0);
                                    else
                                    Put_Bmp(430, 330,  animalende9060,0);
                                    break; 
                  
                    case STAT_DENSITY:
                                    if(CHINESE == Get_Language_Type())
                                    Put_Bmp(430, 330,  densitynextc9060,0); 
                                    else
                                    Put_Bmp(430, 330,  densitynexte9060,0);
                                    break; 
                  
                    case STAT_PEAK:
                                    Put_Bmp(430, 330,  save9060,0); 
                                    break; 
                  
                    case STAT_TARGET:
                                    Put_Bmp(430, 330,  save9060,0);  
                                    break; 
                  
                    default:break;
                   }
                  break;
                
           default:break;
           } 
         }   
	  /////////////////////////////////////////////// 
	  return(Key_Code);
	}
//////////////////////////////////////////////////////	

//////////////////////////////////////////////////////
//8强制结束校准状态
//////////////////////////////////////////////////////
void  End_Cal_Pro(void)
    {
     point2_cal_start = FALSE;
     current_mode = STAT_WEIGH;  //switch to weigh mode
    }
    
void  End_Cal_Pro_Inn(void)
    {
     point2_cal_start = FALSE;
     current_mode = STAT_WEIGH;  //switch to weigh mode
     //马达回位
	 motor_err_flag = motor_run_proc(UNLOAD_POSITION); 
    }
//////////////////////////////////////////////////////
//9每种工作状态下的附属按键处理
//////////////////////////////////////////////////////
static sint32  Addition_Key_Do(uint32 key_number,sint32 op_id)
   {
    sint32 op_id_tmp;
    op_id_tmp = op_id;
    switch(current_mode)
    {
     case STAT_WEIGH:
         if(key_number == 13)        Set_Tare_Flag();   //去皮操作
         else if(key_number == 14)   Set_Zer0_Flag();   //置零
         else if(key_number == 15)   Save_Rec(current_mode); //保存一条记录
         break;                  
     case STAT_PCS:
         if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
         else if(key_number == 14)   //手动输入 单件重 
              {
              op_id_tmp = MENU_NUM_INPUT_OPER;
              }
         else if(key_number == 15)    //采集单件重
              {
               which_para = DATA_WEIGHT_PER_PCS;
               op_id_tmp = MENU_PCS_INPUT_OPER;
              }  
         else if(key_number == 16)
              Save_Rec(current_mode);        //保存一条记录        
         
         break;  
     case STAT_100:
          if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if(key_number == 14)
              {
               op_id_tmp = MENU_NUM_INPUT_OPER; //手动输入基准点
              }           
          else if(key_number == 15)
            Set_Sam100_Flag(); //采集基准点
          else if(key_number == 16)
            Save_Rec(current_mode);        //保存一条记录  
          break;
     case STAT_CHECK:
          if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if(key_number == 14)
              {
               op_id_tmp = MENU_NUM_INPUT_OPER; //手动输入上下限
              }           
          else if(key_number == 15)
             {
               which_para = DATA_BAIFENBI_LIMIT;
               op_id_tmp = MENU_PCS_INPUT_OPER; //采集范围百分比
              } 
          else if(key_number == 16)
             Save_Rec(current_mode);   
               
          break;
     case STAT_ANIMAL:
          if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if(key_number == 14)
              {
               which_para = DATA_ANIMAL_TIME;
               op_id_tmp = MENU_PCS_INPUT_OPER; //手动输入响应时间
              }           
          else if((key_number == 15) && (animal_time > 0))
             {
                  animal_start = TRUE; //开始采集动物数据
                  mode_run_flag = TRUE;
                  animal_real_time = animal_time;
              } 
          else if(key_number == 16)    
             {
                mode_run_flag  = FALSE;
                animal_start = FALSE;
                animal_ok    = FALSE;
                //结束本次数据保持
             }
          break; 
     case STAT_PEAK:
           if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if(key_number == 14)
                {
                 mode_run_flag = TRUE;
                 peak_start = TRUE;   //开始 PEAKMODE 
                }     
          else if(key_number == 15)
                {
                 peak_weight = 0;
                 peak_start = FALSE;  //STOP PEAKMODE     
                 mode_run_flag = FALSE;
                }
          else if(key_number == 16)
             Save_Rec(current_mode);         
          break;    
       
     case STAT_TOTAL:
           if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if(key_number == 14)
                {
                 Set_Totaladd_Flag();
                }      
                //total_weight += net_weight;          //累加条件：必须要回零吗 ？累加一次     
          else if(key_number == 15)
              {
               total_weight  = 0.0;
               total_times   = 0;
              }                 //     
          else if(key_number == 16)
             Save_Rec(current_mode);   
          break;
     case STAT_TARGET:
           if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if((key_number == 14) && (TRUE == stable_flag))
                {
                op_id_tmp = MENU_NUM_INPUT_OPER;          //手动输入目标值  
                }
          else if(key_number == 15)
              {
               Set_SamTarget_Flag();
              }                 //     
          else if(key_number == 16)
             Save_Rec(current_mode);   
          
          break;    
     case STAT_DIFFER:
           if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
           // 调整到亮度调节菜单中
           /*
           #if (1==INNER_CAL_VERSION)
           else if(key_number == 15)
                op_id_tmp = MENU_NUM_INPUT_OPER;          //手动输入真实砝码重量
           #endif */     
           break;
                 
     case STAT_DENSITY:
            if(key_number == 13)        Set_Tare_Flag();   // 去皮操作
          else if((key_number == 14) && (TRUE == stable_flag))
                {
                op_id_tmp = MENU_NUM_INPUT_OPER;          //手动输入温度 
                }
          else if(key_number == 15)
              {
                 mode_run_flag = TRUE;
                 density_start = TRUE;   //开始 PEAKMODE 
                }     
          else if(key_number == 16)
              {      
                 density_start = FALSE;  //STOP density MODE     
                 mode_run_flag = FALSE;
              }                 //     
     
          break;
     case STAT_CALING:
          if(key_number == 13)       End_Cal_Pro();   // 强制结束校准
          break;
     case STAT_CALING_INNER:
          if(key_number == 13)       End_Cal_Pro_Inn();   // 强制结束校准
          break;      
     default:break;
    }
    return(op_id_tmp);
   }
//**********************************************************************
//10函数  名:Sleep_Key_Pro
//函数功能:待机界面下的按键处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static  sint32 Sleep_Key_Pro(uint32 key_c,sint32 op)
   {
    sint32 op_id;
    uint32 i;
    op_id = op;  
    no_key_time = 0;      //休眠模式计时清零  
    //if(Key_Status != KEY_DO_START)
    // {
    // return(op_id);
    //}
    i = Action_SleepCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(1 == i)
        {
         Set_LCD_Brightness(lcd_brightness); //返回到正常亮度
         op_id = MENU_OP_OPER;           //返回
        }
    return(op_id);  
   }
//**********************************************************************
//11函数 名:Main_Key_Pro
//函数功能:工作界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static  sint32 Main_Key_Pro(uint32 key_c,sint32 op)
{
    
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //{
    // return(op_id);
    //}
    i = Action_MainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://weigh
  	           current_mode = STAT_WEIGH;
  	           Set_Zer0_Flag();  //确定0点，同时作去皮处理
  	           break;
    	case 2:                              //count
    	       current_mode = STAT_PCS;
    	       Set_Zer0_Flag();  //确定0点，同时作去皮处理
    	       //op_id = MENU_STAT_PCS_OPER;
    	       break;
    	case 3:// 百分比模式
    	       current_mode = STAT_100;
  	           Set_Zer0_Flag();  //确定0点，同时作去皮处理
    	       break;
    	case 4:// more mode
    	       op_id = MENU_MODE_SEL_OPER;
    	       break;     
    	case 5://cal 更改为待机
    	       op_id = MENU_SLEEP_OP_OPER;     
    	       break;  
    	case 6://more config
    	       op_id = MENU_MORE_CFG_OPER;
    	       break;
    
    	case 7://print function
    	       //检查打印条件是否满足
               if((0==print_busy)&&(TRUE==stable_flag)&&(1==Uart_Printer_Flag))
               {
                print_busy = 1;
                UART_PrintData(); 
               }
    	       break;                           
    	       
    	case 8://unit
    	       //op_id = MENU_LNT_SEL_OPER;
    	       op_id = MENU_UNIT_SEL_OPER;
   		       break;                           
    	case 9://更改为无线感应
    	       //内校准版本 修改;
    	       if(INNER_CAL_VERSION)
    	         Enter_2PointInner_Cal();
    	       else
    	       op_id = MENU_WIRELESS_CFG_OPER;
    	       break;
   		case 10://COMM更改为校准
   		       //op_id = MENU_PRT_SEL_OPER;
   		       Enter_2Point_Cal();
   		       //op_id = MENU_WIRELESS_SEL_OPER;
   		       break;
   		case 11://调取数据
   		       op_id = MENU_CALL_DATA_OPER;
   		       break;
   		
   		case 12://time
   		        //op_id = MENU_CALL_DATA_OPER;
   		       break;
   		
   		//////////////////////////////////additional functionkey
   	    case 13: 
   	    case 14: 
   	    case 15: 
   	    case 16:
   	            op_id = Addition_Key_Do(i,op_id);
   	            break; 
   	   	default:break;
   	   }
   	  
   	  //Key_Status = KEY_DO_OVER;
    }
    return(op_id);    
}

//**********************************************************************
//12函数名:Oper_Sleep_Pro               
//函数功能:待机模式下界面显示
//入口参数:无
//出口参数:无
//**********************************************************************
void  Oper_Sleep_Pro(void)
     {
      Display_Sleep_Time();
     
     }

//**********************************************************************
//13函数 名:Oper_Disp_Pro               
//函数功能:工作模式下的显示处理
//入口参数:无
//出口参数:无
//**********************************************************************
void Oper_Disp_Pro(void)
{
  uint32 i;
  static uint32 Process_bar1=0,Process_bar2=0;
  static uint32 stable_change_tmp=TRUE;
  static uint32 mode_change_tmp=STAT_NULL;
  
  /////////////////////////////////////////稳定状态显示       
 	 if(2 == reflash_flag) //满量程刷屏
 	   {
 	    reflash_flag = 0;
 	    Clr_Bar(130,100,540-130,220-100,COL_BLACK);
 	   }
 	 //////////////////////////////////////////////////////  
 	 if(stable_change_tmp != stable_flag)
  	   {  
   	    stable_change_tmp  = stable_flag;
   	    Stable_Status_Display();
  		} 
  //////////////////////////////////////////工作状态显示
     if(mode_change_tmp != current_mode)
       {
        mode_change_tmp = current_mode;
        Weigh_Status_Display();
        }
  ////////////////////////////////////////display weigh data
  if(TRUE == power_on_flag) 
    {
     //Set_ASC_Lib(GulimChe72_Font);
     //Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
     //Put_Char(210+48*3, 120, '.');
     //Put_Line_Str(150, 120, "123.4567");
    }
   else
     New_Data_Display();
}

//**********************************************************************
//14函数名:Set_Menu_Op
//函数功能:设置函数当前所处的界面
//入口参数:cur_menu_op
//出口参数:无
//**********************************************************************
//void Set_Menu_Op(sint32 cur_menu_op)
//{
//    menu_op = cur_menu_op;
//}
/**********************************************************************
//15 待机界面的初始化
//
//////////////////////////////////////////////////////////////////////*/
void   Put_Sleep_Interface(sint32 op)
    {
     uint32 xy[1][2];
     
     Set_LCD_Brightness(SLEEP_BRIGHTNESS);
     ////////////////////////////////////待机状态下把背光设置到较暗的亮度
     Display_Sleep_Area();
     
     xy[0][0] =  520+110/2;
     xy[0][1] =  410+ 60/2;
     Create_ButtonList(xy,1);    
    }
//**********************************************************************
//16函数  名:Put_Oper_Interface
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void Put_Oper_Interface(sint32 op)
{
    uint32 xy[16][2];
    uint32 i;  
    //if(TRUE == power_on_flag)
    //{
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    
    Clr_Bar(0,0,SCR_XSIZE, 70,COL_THINBLUE);   //TITLE
    //
    if(logo_display_flag)
    {
     Put_Bmp(5, 5,logo6060,0);
     
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     if(model_display_flag)
      {
       Put_Line_Str( 75,  10, "SY");
       Put_Data(75+22,    10,  "%02d", (model_id&0xffff)/10);      //显示型号
       if(1 == dot_position)
       Put_Data(75+44,    10,  "%01d", 4);
       else
       Put_Data(75+44,    10,  "%01d", 3);
       if(INNER_CAL_VERSION)
       Put_Char( 130, 10, 'C');
      }
       
      Put_Line_Str( 68, 40,  Language_Str("UID:","UID:"));      //显示操作者编号
      Put_Data(112, 40, "%03d",user_id);
     }
    else
       //显示机器型号 和 操作者编号//////
    { 
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);  
     if(model_display_flag)
      {
       Put_Line_Str( 3,  10, "SY");
       Put_Data(3+22,    10,  "%02d", (model_id&0xffff)/10);      //显示型号
       if(1 == dot_position)
         Put_Data(3+44,    10,  "%01d", 4);
       else
         Put_Data(3+44,    10,  "%01d", 3);
       if(INNER_CAL_VERSION)
       Put_Char( 130-70, 10, 'C');
       }
     
      Put_Line_Str( 68-65, 40,  Language_Str("UID:","UID:"));      //显示操作者编号
      Put_Data(112-65, 40, "%03d",user_id);
     }
     
    ////////////////////////////////////////// 
    Clr_Bar(0,70,  90,340,COL_DEEPGRAY);  //indicate area
   
    for(i=0;i<3;i++)
    Clr_Bar(0, 70+83+85*i,90, 4,COL_DEEPBLUE);  //分割线
    
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPGRAY);
    Put_Line_Str( 20, 75,    Language_Str("模式","Mode"));      //INFO1
    Put_Line_Str( Language_Pos(20,28), 75+85, Language_Str("状态","Run"));       //INFO2
    Put_Line_Str( 20, 75+170,Language_Str("温度","Temp"));      //INFO3 
    
    Clr_Bar(5,105+85*2,80,40,COL_BLACK);
    Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
    Set_ASC_Lib(Arial20B_Font); 
    Put_Char( 60, 280,'#');      //display oC 
    
    //键盘区域
    Clr_Bar(0,415-88,90,85,COL_BLACK);
    Clr_Bar(0,400+3, 90, 3,COL_WHITE);
    //使用图片代替
    for(i=0;i<5;i++)
      Clr_Bar(100+103*i, 410-3,3,70+3,COL_WHITE);  //分割线
    
    if(CHINESE == Get_Language_Type())
      {
       Put_Bmp(103*0, 415-80,morecfgc9060,0);     //更多设置
       //add printer
       
       Put_Bmp(103*0+5, 415,printerc9060,0);     //打印
       Put_Bmp(103*1+5, 415,unitcfgc9060,0);     //单位
       if(INNER_CAL_VERSION)
       Put_Bmp(103*2+5, 415,innercalc9060,0);    //内校准
       else
       Put_Bmp(103*2+5, 415,wirelesscfgc9060,0); //无线
       
       Put_Bmp(103*3+5, 415,calc9060,0);         //校准
       Put_Bmp(103*4+5, 415,recallc9060,0);   //修改为查阅
      }
    else
      {
       Put_Bmp(103*0,   415-80,morecfge9060,0);     //更多设置
      
       Put_Bmp(103*0+5, 415,printere9060,0);     //打印
       Put_Bmp(103*1+5, 415,unitcfge9060,0);     //单位
       if(INNER_CAL_VERSION)
       Put_Bmp(103*2+5, 415,innercale9060,0);    //内校准
       else
       Put_Bmp(103*2+5, 415,wirelesscfge9060,0); //无线
       
       Put_Bmp(103*3+5, 415,cale9060,0);         //校准
       Put_Bmp(103*4+5, 415,recalle9060,0);      //修改为查阅
      } 
    Put_Bmp(525,415,sleep11060,0);           //休眠
    
    //使用图片代替
    Clr_Bar(550,70, 90, 340,COL_THINGRAY);   //right key area 
    for(i=0;i<3;i++)
    Clr_Bar(550, 70+83+85*i,90, 4,COL_DEEPBLUE);  //分割线
    
    Put_Bmp(550, 70+      12,basicmode9060,0);
    Put_Bmp(550, 70+1*85+ 12,pcsmode9060,0);
    Put_Bmp(550, 70+2*85+ 12,baifenbi9060,0);
    if(CHINESE == Get_Language_Type())
    Put_Bmp(550, 70+3*85+ 12,moremodec9060,0);
    else
    Put_Bmp(550, 70+3*85+ 12,moremodee9060,0);
  
    ////////////////////////////////////////create keylist 
    for(i=0;i<5;i++)
    {
    xy[i][0] = 550+90/2;
    xy[i][1] = 82+45+85*i;
    }
   
    xy[i][0]   = 55;
    xy[i++][1] = 350+60/2;
    
    for(;i<11;i++)
    {
    xy[i][0] = 50+(i-6)*103;
    xy[i][1] = 410+60/2;
    }
    
    //////////////////////////create time button
    xy[i][0]    = 500+140/2;
    xy[i++][1]  = 45;
    //////////////////////////additional key total 4
    for(;i<16;i++)
     {
      xy[i][0]  = 100+110*(i-12)+100/2;
      xy[i][1]  = 330+60/2;
     }
     
    Create_ButtonList(xy,16);            

    /////////////////////////////////////REFLASH
    Weigh_Status_Display();

}
//******************************************************************************************************
//17 函数  名:Normal_Operation
//函数功能:管理机器在正常工作模式下的各种操作
//入口参数:无
//出口参数:无
//******************************************************************************************************
void Normal_Operation(void)
{  
    static uint32 cycle_times;
    static uint32 sample_temp_time = 0;
    uint16    ad_tmp;
    float     tmp;
    
    struct menu_str menu_op[MENU_NUM]=
     //菜单ID                菜单界面初始化                     菜单选项处理                显示处理             辅助处理
    {
     {MENU_OP_OPER,           Put_Oper_Interface,               Main_Key_Pro,          Oper_Disp_Pro,  Oper_Main_Assi_Pro},
     {MENU_SLEEP_OP_OPER,     Put_Sleep_Interface,             Sleep_Key_Pro,         Oper_Sleep_Pro,       Scr_Assi_Null},  //待机画面
     {MENU_DATE_CFG_OPER,     TimeCfg_Oper_Interface,        TimeCfg_Key_Pro,       TimeCfg_Disp_Pro,       Oper_Assi_Pro},  //DATE  cfg
     {MENU_RANGE_SEL_OPER,    RangeSel_Interface,           RangeSel_Key_Pro,      RangeSel_Disp_Pro,  		Oper_Assi_Pro},  //range cfg  
     {MENU_UNIT_SEL_OPER,     UnitSel_Interface,             UnitSel_Key_Pro,       UnitSel_Disp_Pro,       Oper_Assi_Pro},  //UNIT cfg
     {MENU_ASD_SEL_OPER,      ASDSel_Interface,               ASDSel_Key_Pro,        ASDSel_Disp_Pro,       Oper_Assi_Pro},  //ASD CFG
     {MENU_LNT_SEL_OPER,      LNTSel_Interface,               LNTSel_Key_Pro,        LNTSel_Disp_Pro,       Oper_Assi_Pro},  //LNT CFG  
     {MENU_PRT_SEL_OPER,      PRTSel_Interface,               PRTSel_Key_Pro,        PRTSel_Disp_Pro,  	  	Oper_Assi_Pro},  //PRINT CFG
     {MENU_MODE_SEL_OPER,     ModeSel_Interface,             ModeSel_Key_Pro,       ModeSel_Disp_Pro,  		Oper_Assi_Pro},  // mode MODE CFG  
     {MENU_LANG_SEL_OPER,     LangSel_Interface,             LangSel_Key_Pro,       LangSel_Disp_Pro,  		Oper_Assi_Pro},  // lange CFG  
     {MENU_CAL_10POINT_OPER,  Cal_10point_Interface,     Cal_10point_Key_Pro,   Cal_10point_Disp_Pro,  		Oper_Assi_Pro},  // 10point CAL
     {MENU_NUM_INPUT_OPER,    NumberInput_Interface,  	    NumInput_Key_Pro,   NumberInput_Disp_Pro,  		Oper_Assi_Pro},  // INPUT num  
     {MENU_PCS_INPUT_OPER,    PCSInput_Interface,           PCSInput_Key_Pro,      PCSInput_Disp_Pro,  		Oper_Assi_Pro},  // INPUT pcs
     {MENU_MORE_CFG_OPER,     MoreCfgInput_Interface,        MoreCfg_Key_Pro,       MoreCfg_Disp_Pro,  		Oper_Assi_Pro},  // Machine cfg
     {MENU_WIRELESS_CFG_OPER, WirelessCfgInput_Interface,WirelessCfg_Key_Pro,   WirelessCfg_Disp_Pro,  		Oper_Assi_Pro},  //Wireless cfg
     {MENU_SCR_CAL_OPER,      ScrCalInput_Interface,          ScrCal_Key_Pro,        ScrCal_Disp_Pro,  		Scr_Assi_Null},  //screen cal
     {MENU_BRIG_CFG_OPER,     Brig_Adjust_Interface,     Brig_Adjust_Key_Pro,   Brig_Adjust_Disp_Pro,  		Oper_Assi_Pro},  //亮度配置
     {MENU_CALL_DATA_OPER,    Data_Call_Interface,         Data_Call_Key_Pro,     Data_Call_Disp_Pro,  		Oper_Assi_Pro},  //数据回显示
     {MENU_SPEAK_CFG_OPER,    Speak_Adjust_Interface,   Speak_Adjust_Key_Pro,  Speak_Adjust_Disp_Pro,  		Oper_Assi_Pro},  //声音配置
     {MENU_PASSW_CFG_OPER,    Password_Adjust_Interface, PasswordCfg_Key_Pro,   PasswordCfg_Disp_Pro,  		Oper_Assi_Pro},  //密码配置
     {MENU_OPERID_CFG_OPER,   OperID_Adjust_Interface,     OperIDCfg_Key_Pro,     OperIDCfg_Disp_Pro,  		Oper_Assi_Pro},  //操作员编号输入
     {MENU_FACTORY_CFG_OPER,  Factory_Restore_Interface,  FactoryCfg_Key_Pro,    FactoryCfg_Disp_Pro,  		Oper_Assi_Pro}   //回出厂设置
      //  
     };
    sint32 cur_op,next_op;    //当前选项和所切换到的选项(bits[7:0]:选项号;bits[15:8]:界面号;bits[23:16]:选项的显示状态)
  
    cur_op =0xffffffff;  
    next_op = MENU_OP_OPER;
    //next_op = MENU_MORE_CFG_OPER;
  
//#ifndef _DEBUG
//非调试模式则使能看门狗
   // Init_WDT(500);                     //看门狗初始化,喂狗间隔=1s
   // WDT_Feed();                        //喂狗
   
   while (1)
     {
        if ((cur_op^next_op)&0x0000ff00)//当前所处的界面和准备切换到的界面不一样
        {//初始化界面
            (*(menu_op[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"界面/菜单/选项"之间的切换
        //Set_Menu_Op(cur_op);
        
        if (TRUE == flag_new_data) 
            {
             flag_new_data = FALSE;
             filter();
             update_new_data();
            }
        if (flag_10ms_ok)    //10ms时间到
            {
             flag_10ms_ok = FALSE;
             //增加GET内部AD 数据功能
             
              cycle_times++;
             /////////////////////////////////////////////
             #if(0 == USE_DS18B20)
             if(0==(cycle_times%6))
                {
                 while(!(ADDR3&0x80000000));
                 ad_tmp = (ADDR3>>6)&0x3ff;
                 
                  filter_ad_NTC(ad_tmp);
               } 
             #endif
            ///////////////////////////////////////////////////////
             
             key_value = Get_PositionXY();        //键盘扫描
             // if(key_value)        //键盘处理流程：根据不同键值，返回不同的窗口界面 
             // {                    //在该窗口按键流程中处理不同的功能，处理完毕再返回到主界面
             next_op = (*(menu_op[(cur_op>>8)&0xff].op_pro))(key_value,cur_op);     
             // }
             }  
      
        if (flag_100ms_ok)
            {//显示时间到,或者要显示新的参数值
             flag_100ms_ok = FALSE;
             (*(menu_op[(cur_op>>8)&0xff].disp_pro))();    //显示处理函数    
            }    
             //  WDT_Feed();     //3次
        if (flag_300ms_ok)
            {                  //500ms时间到
             flag_300ms_ok = FALSE;
             (*(menu_op[(cur_op>>8)&0xff].assi_pro))();    //辅助处理函数
             /////////////////////////////////增加无线辅助功能
             next_op = Wireless_Process(next_op); //该函数可能会改变界面
             ///////////////////////////////////////
             if((MENU_OP_OPER == next_op)&&(no_key_time > ENTER_SLEEP_TIME))       //只有在主界面下才可以进入待机状态
               next_op = MENU_SLEEP_OP_OPER;
             ////////////////////////////////////
             //UART_SendData(); 
             }
         if (flag_500ms_ok)
            {                  //500ms时间到
             flag_500ms_ok = FALSE; 
             LED_CPU_RUN_Cpl(); //CPU工作指示灯闪烁频率1Hz
            }
            /////////////////////////每秒发送数据一次，上位机解释此帧数据    
         if (flag_1s_ok)     
             ////////////////////////////////轮流发送各个状态帧
            {
             flag_1s_ok = FALSE;
             if(0==Uart_Printer_Flag)
             UART_SendData();
             
             sample_temp_time++;
             if(sample_temp_time > 1800)
                 {
                  temp_cmp_flag = TRUE;
                  temp_cmp_point = temp_float;             //保存当前温度 预热温度可能上升几度
                 }
            }
      }
  }

////////////////////////////////////////////////////////
//18  在指定位置用指定颜色显示单位
///////////////////////////////////////////////////////
static void Unit_Display(void)
 {
  
  Clr_Bar(160,110,390,100,COL_BLACK); //把原先显示数据清除
  
  Clr_Bar(95,195, 50, 32,COL_BLACK); //把原先显示数据清除
  Set_ASC_Lib(GulimChe48_Font);
  Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
  if(UNIT_G == current_unit)
  Put_Char(95+15,    195, 'g');
  else if(UNIT_MG == current_unit)
  Put_Line_Str(95,   195, "mg");
  else if(UNIT_LB == current_unit)
  Put_Line_Str(95,   195, "Lb");
  else if(UNIT_OZ == current_unit)
  Put_Line_Str(95,   195, "Oz");
  else
  Put_Line_Str(95,   195, "  ");
  }
  
////////////////////////////////////////////////////////
//19    在指定位置用指定颜色显示稳定符号
///////////////////////////////////////////////////////
static void Stable_Status_Display(void)
 {
  if(TRUE == stable_flag)
   {
   Set_Color(COL_DEEPGREEN);
   Fill_Circle(120,100,10);
   }
   else
   {
    Set_Color(COL_DEEPRED);
    Fill_Circle(120,100,10);
   } 
 }
 
////////////////////////////////////////////////////////
//20    在指定位置用指定颜色显示运行符号
///////////////////////////////////////////////////////
static  void  Mode_Run_Display(void)
 {
    /////////////////////////////////////////////////
  Set_ASC_Lib(Arial16B_Font); 
  if(TRUE == mode_run_flag)
   {
    Clr_Bar(5,190,80,40,COL_DEEPGREEN); 
    Set_Chr_Color(COL_BLACK,COL_DEEPGREEN);
    Put_Line_Str(10, 200,Language_Str("运行中","RUNING"));
   }
   else
   {
    Clr_Bar(5,190,80,40,COL_DEEPRED);
    Set_Chr_Color(COL_BLACK,COL_DEEPRED);
    Put_Line_Str(20, 200,Language_Str("停止","STOP"));
   } 
 }
 
////////////////////////////////////////////////////////
//21 在指定位置用指定颜色显示 工作状态
//   同时刷新小的显示区域
///////////////////////////////////////////////////////
static  void  Weigh_Status_Display(void)
 {
  Clr_Bar(90,70,          460, 170,  COL_BLACK);  //display area
  Clr_Bar(90,70+170,      460,  70,  COL_THINPUPLE);  //副显示区
  Clr_Bar(90,70+170+70,   460, 100,  COL_THINYELLOW);  //操作区
  //ADD 分隔符
  Clr_Bar(0,410-4, SCR_XSIZE, 4 ,COL_BLACK);  //操作区

  Clr_Bar(5,105,80,40,COL_BLACK); 
  Set_ASC_Lib(Arial16B_Font);  
  Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
  switch(current_mode)
  {
  case STAT_CALING:
         Put_Line_Str(10, 115,Language_Str("校准中","CALing"));
         mode_run_flag = TRUE;
         
         Put_Button_Bar(100,330,100+150,330+60,3,COL_THINBLUE);  //强制退出
         Set_Chr_Color(COL_BLACK,COL_THINBLUE);
         Put_Line_Str(Language_Pos(130,130+20), 350,Language_Str("强制退出","ABORT"));
         break;
  case STAT_CALING_INNER:
         Put_Line_Str(10, 115,Language_Str("内校中","INNCAL"));
         mode_run_flag = TRUE;
         
         Put_Button_Bar(100,330,100+150,330+60,3,COL_THINBLUE);  //强制退出
         Set_Chr_Color(COL_BLACK,COL_THINBLUE);
         Put_Line_Str(Language_Pos(130,130+20), 350,Language_Str("强制退出","ABORT"));
         break;
  case STAT_WEIGH :
         Put_Line_Str(20, 115,Language_Str("称重","WEIGH"));
         mode_run_flag = TRUE;
         //使用图片代替
    	 Put_Bmp(100, 330,tare9060,0);
     	 Put_Bmp(210, 330,zer09060,0);
     	 Put_Bmp(320, 330,save9060,0);
     	 /*Put_Button_Bar(120,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	 Put_Button_Bar(290,320,290+150,310+60,3,COL_THINBLUE); //ZER0
     	 Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     	 Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
     	 Put_Line_Str(290+50, 335,Language_Str("置零","ZERO"));*/
         ///////////////////////////////////////////////////显示副窗口信息
         Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	 Put_Line_Str(120, 265,Language_Str("去皮量:","TARE:"));
     	 Set_ASC_Lib(Arial20B_Font); 
     	 Put_Char(360, 265,'g');
     	 Unit_Display();
         break;
  case STAT_PCS:
        Put_Line_Str(20,  115,Language_Str("计数","COUNT"));
        mode_run_flag = TRUE;
     	//使用图片代替
     	Put_Bmp(100, 330,        tare9060,0); 
     	if(CHINESE == Get_Language_Type())
         {
          Put_Bmp(210, 330,  inputWPP9060,0);
     	  Put_Bmp(320, 330, samplePCS9060,0);
     	 }
     	 else
     	 {
     	  Put_Bmp(210, 330, inputWPP29060,0);
     	  Put_Bmp(320, 330,samplePCS29060,0);
     	 }
     	
     	 Put_Bmp(430, 330,       save9060,0);
     	/*
     	Put_Button_Bar(140,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	Put_Button_Bar(280,320,280+140,310+60,3,COL_THINBLUE);  //输入单件重
    	Put_Button_Bar(430,320,430+140,310+60,3,COL_THINBLUE);  //采集单件重 
    	Set_Chr_Color(COL_BLACK,COL_THINBLUE);    
    	Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
    	Put_Line_Str(Language_Pos(300,300), 335,Language_Str("输入单件重","Input Wei"));
     	Put_Line_Str(Language_Pos(440,455), 335,Language_Str("采集单件重","Sample"));
        */
    	/////////////////////////////////////////////////////////
     	Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    	//Set_ASC_Lib(Arial20B_Font);
   		Put_Line_Str(120, 250,Language_Str("单件重:","SAMPLE:"));
   		Put_Line_Str(120, 280,Language_Str("重量:",  " WEIGH:"));
    	Set_ASC_Lib(Arial20B_Font); 
    	Put_Char(370, 250,'g');
   		Put_Char(370, 280,'g');
     
    	Set_ASC_Lib(GulimChe48_Font);
    	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
    	Put_Line_Str(95, 205,Language_Str("PCS","PCS"));       //display PCS
        break;
  case STAT_100:    
        Put_Line_Str(Language_Pos(10,5), 115,Language_Str("百分比","PERCENT"));
        mode_run_flag = TRUE;
                                     //display %      
        //使用图片代替
        Put_Bmp(100, 330,      tare9060,0);
        if(CHINESE == Get_Language_Type())
          {
           Put_Bmp(210, 330, inputmarkc9060,0);
     	   Put_Bmp(320, 330,samplemarkc9060,0);   
     	  }
     	else
     	  {
     	   Put_Bmp(210, 330, inputmarke9060,0);
     	   Put_Bmp(320, 330,samplemarke9060,0); 
     	  }  
     	Put_Bmp(430, 330,      save9060,0);                                                   
    	/*/Put_Button_Bar(120,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	Put_Button_Bar(280,320,280+140,310+60,3,COL_THINBLUE); //输入基准点
     	Put_Button_Bar(430,320,430+140,310+60,3,COL_THINBLUE); //采集基准点
     	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     	Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
     	Put_Line_Str(Language_Pos(300,320), 335,Language_Str("输入基准点","Input"));
        Put_Line_Str(Language_Pos(440,455), 335,Language_Str("采集基准点","Sample"));
   	    */
   	    /////////////////////////////////////////////////////////
    	Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	Put_Line_Str(120, 250,Language_Str("基准点:","SAMPLE:"));
     	Put_Line_Str(120, 280,Language_Str("重量:",  "  WEIG:"));
     	Set_ASC_Lib(Arial20B_Font);
     	Put_Char(370, 250,'g');
     	Put_Char(370, 280,'g');
    
   	    Set_ASC_Lib(GulimChe48_Font);
    	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
     	Put_Char(95+15, 205,'%'); 
        break;
  case STAT_CHECK:
        Put_Line_Str(20, 115,Language_Str("检重","CHECK"));
        mode_run_flag = TRUE;
        //使用图片代替
        Put_Bmp(100, 330,          tare9060,0);
        Put_Bmp(210, 330, samplecheck129060,0);
     	Put_Bmp(320, 330,   samplecheck9060,0);     
        Put_Bmp(430, 330,          save9060,0);                                                 
    	/*
    	Put_Button_Bar(120,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	Put_Button_Bar(280,320,280+140,310+60,3,COL_THINBLUE); //手动输入限值
     	Put_Button_Bar(420,320,420+140,310+60,3,COL_THINBLUE); //百分比输入限值
     	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     	Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
     	Put_Line_Str(Language_Pos(300,320), 335,Language_Str("输入限值",  "Input"));
        Put_Line_Str(Language_Pos(440,455), 335,Language_Str("输入百分比","Input"));
   	    */
   	    /////////////////////////////////////////////////////////
    	Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	Put_Line_Str(120, 250,Language_Str("下限值:","Limited1:"));
     	Put_Line_Str(120, 280,Language_Str("上限值:","Limited2:"));
     	Set_ASC_Lib(Arial20B_Font);
     	Put_Char(370, 250,'g');
     	Put_Char(370, 280,'g');
        
        Unit_Display();
        break; 
  
  case STAT_DIFFER:
        Put_Line_Str(10, 115,Language_Str("差异化","DIFFER"));
        mode_run_flag = FALSE;
        
        Put_Bmp(100, 330,          tare9060,0);
        //Put_Bmp(210, 330,save9060,0);
        break;
        
  case STAT_TOTAL:
        Put_Line_Str(20, 115,Language_Str("累加","TOTAL"));
        mode_run_flag = TRUE; 
        total_weight  = 0.0;
        total_times   = 0;
        
          //使用图片代替
        Put_Bmp(100, 330,          tare9060,0);
        if(CHINESE == Get_Language_Type())
         {
          Put_Bmp(210, 330,      totaladdc9060,0);  //累加1次
          Put_Bmp(320, 330,    totalclearc9060,0);  //清除
         }
        else
         {
          Put_Bmp(210, 330,      totaladde9060,0);  //累加1次
          Put_Bmp(320, 330,    totalcleare9060,0);  //清除
         }
        Put_Bmp(430, 330,          save9060,0);                                                  
    	/*
    	Put_Button_Bar(120,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	Put_Button_Bar(280,320,280+140,310+60,3,COL_THINBLUE); //手动输入限值
     	Put_Button_Bar(420,320,420+140,310+60,3,COL_THINBLUE); //百分比输入限值
     	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     	Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
     	Put_Line_Str(Language_Pos(300,320), 335,Language_Str("输入限值",  "Input"));
        Put_Line_Str(Language_Pos(440,455), 335,Language_Str("输入百分比","Input"));
   	    */
   	    /////////////////////////////////////////////////////////
    	Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	Put_Line_Str(120, 250,Language_Str("累加重量:","TotalWeight:"));
      	Put_Line_Str(120, 280,Language_Str("累加次数:"," TotalTimes:"));
     	Put_Line_Str(345, 280,Language_Str("次","Times"));
     	Set_ASC_Lib(Arial20B_Font);
     	Put_Char(Language_Pos(370,400), 250,'g');
     	
     	Unit_Display();
        break;
        
  case STAT_ANIMAL:
        Put_Line_Str(Language_Pos(20,10), 115,Language_Str("动物","ANIMAL"));
        mode_run_flag = FALSE;
        animal_start  = FALSE;
        animal_ok     = FALSE;
        
         //使用图片代替
        Put_Bmp(100, 330,            tare9060,0);
        Put_Bmp(210, 330,      animaltime9060,0);  //时间设置
        if(CHINESE == Get_Language_Type())
         {
          Put_Bmp(320, 330,     animalstartc9060,0);  //开始称量
       	  Put_Bmp(430, 330,       animalendc9060,0);  //重置   
     	 }
     	else
     	 {
     	  Put_Bmp(320, 330,     animalstarte9060,0);  //开始称量
       	  Put_Bmp(430, 330,       animalende9060,0);  //重置  
     	 } 
     	//Put_Bmp(540, 330,            save9060,0);                                                 
    	/*
    	Put_Button_Bar(120,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	Put_Button_Bar(280,320,280+140,310+60,3,COL_THINBLUE); //手动输入限值
     	Put_Button_Bar(420,320,420+140,310+60,3,COL_THINBLUE); //百分比输入限值
     	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     	Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
     	Put_Line_Str(Language_Pos(300,320), 335,Language_Str("输入限值",  "Input"));
        Put_Line_Str(Language_Pos(440,455), 335,Language_Str("输入百分比","Input"));
   	    */
   	    /////////////////////////////////////////////////////////
    	Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	Put_Line_Str(120, 250,Language_Str("响应时间:","WorkTime:"));
        Put_Line_Str(120, 280,Language_Str("运行时间:"," RunTime:"));
     	Set_ASC_Lib(Arial20B_Font);
     	Put_Line_Str(290, 250,Language_Str("秒","s"));
     	Put_Line_Str(290, 280,Language_Str("秒","s"));
        Unit_Display();
        break;
        
  case STAT_DENSITY:
        Put_Line_Str(Language_Pos(20,5), 115,Language_Str("密度","DENSITY"));
        mode_run_flag = FALSE; 
        density_start = FALSE;
        
           //使用图片代替
        Put_Bmp(100, 330,          tare9060,0);
        Put_Bmp(210, 330,   densitytemp9060,0);  //水温度
        if(CHINESE == Get_Language_Type())
         {
          Put_Bmp(320, 330,   animalstartc9060,0);  //开始
     	  Put_Bmp(430, 330,   densitynextc9060,0);  //下一步                                                   
    	 }
    	else
    	 {
    	  Put_Bmp(320, 330,   animalstarte9060,0);  //开始
     	  Put_Bmp(430, 330,   densitynexte9060,0);  //下一步                           
    	 } 
    	//Put_Bmp(540, 330,save9060,0);
    	Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	Put_Line_Str(Language_Pos(110,100), 250,Language_Str("空气中重量:","  InAir Weight:"));
        Put_Line_Str(Language_Pos(110,100), 280,Language_Str("液体中重量:","InWater Weight:"));
        Put_Line_Str(Language_Pos(445,430), 250,Language_Str("水温度:",         "WaterTemp:"));
        
        //Set_ASC_Lib(Arial20B_Font);
     	//Put_Char(290, 260,'s');
     	//Put_Char(655, 260,'s');
        Unit_Display(); 
        break;
  case STAT_PEAK:
        Put_Line_Str(20, 115,Language_Str("峰值","PEAK")); 
        mode_run_flag = FALSE; 
        peak_start = FALSE;
        peak_weight = 0;
           //使用图片代替
        Put_Bmp(100, 330,          tare9060,0);
        if(CHINESE == Get_Language_Type())
         {
          Put_Bmp(210, 330,   animalstartc9060,0);  //开始
          Put_Bmp(320, 330,     animalendc9060,0);  //结束
         }
        else
         {
          Put_Bmp(210, 330,   animalstarte9060,0);  //开始
          Put_Bmp(320, 330,     animalende9060,0);  //结束
         } 
        Put_Bmp(430, 330,          save9060,0);
   	    ///////////////////////////////////////////////////显示副窗口信息
        Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     	Put_Line_Str(120, 250,Language_Str("去皮量:", "TARE:"));
     	Put_Line_Str(120, 280,Language_Str("重量:",   "WEIG:"));
     	Set_ASC_Lib(Arial20B_Font);
     	Put_Char(370, 250,'g');
     	Put_Char(370, 280,'g');
    	Unit_Display();
        break;
  case STAT_TARGET:
        Put_Line_Str(10, 115,Language_Str("目标值","TARGET"));
        mode_run_flag = TRUE; 
        //使用图片代替
        Put_Bmp(100, 330,      tare9060,0);
        if(CHINESE == Get_Language_Type())
          {
           Put_Bmp(210, 330, inputmarkc9060,0);     //手动输入目标值
     	   Put_Bmp(320, 330,samplemarkc9060,0);     //采集当前重量为目标值
     	  }
     	else
     	  {
     	   Put_Bmp(210, 330, inputmarke9060,0);     //手动输入目标值
     	   Put_Bmp(320, 330,samplemarke9060,0);     //采集当前重量为目标值
     	  }
     	Put_Bmp(430, 330,      save9060,0);
     	/*
    	Put_Button_Bar(120,320,120+150,310+60,3,COL_THINBLUE); //TARE
     	Put_Button_Bar(280,320,280+140,310+60,3,COL_THINBLUE); //手动输入限值
     	Put_Button_Bar(420,320,420+140,310+60,3,COL_THINBLUE); //百分比输入限值
     	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     	Put_Line_Str(120+50, 335,Language_Str("去皮","TARE"));
     	Put_Line_Str(Language_Pos(300,320), 335,Language_Str("输入限值",  "Input"));
        Put_Line_Str(Language_Pos(440,455), 335,Language_Str("输入百分比","Input"));
   	    */ 
   	     ///////////////////////////////////////////////////显示副窗口信息
   	    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
        Put_Line_Str(120, 250,Language_Str("去皮量:","TARE:"));
   		Put_Line_Str(120, 280,Language_Str("目标值:","TARGET:"));
    	Set_ASC_Lib(Arial20B_Font); 
    	Put_Char(370, 250,'g');
   		Put_Char(370, 280,'g');
     	Unit_Display();
        break;
  default:
        break;
  }
  ////////////////////////////////////
  Stable_Status_Display();
 } 

//////////////////////////////////////////////////////
//24a 常规称重检查 影响报警和 显示颜色 
//////////////////////////////////////////////////////
static uint32  normal_weigh_check(void)
 {
   //
   if(gross_weight > weight_alarm) //123.45678
    {
     Log_Alm(&warn_weigh_out,ALM_LEV_WARN);
     return(1); 
    }  
   else
    {
     Del_Alm(&warn_weigh_out,ALM_LEV_WARN);
     return(0);
    }
   /*  
   if(gross_weight>WEIGHT_ALM_VALUE) //123.45678
    {
     Log_Alm(&warn_weigh_out,ALM_LEV_WARN);
     Set_Chr_Color(COL_DEEPRED,COL_BLACK);
    }
   else
    Del_Alm(&warn_weigh_out,ALM_LEV_WARN); */
  } 


////////////////////////////////////////////////////////
// 22 在指定位置用指定颜色显示 称量数据
///////////////////////////////////////////////////////
static void  New_Data_Display(void)
 {
  uint32 i;
  ///////////////////////////////显示运行状态
  Mode_Run_Display();
 ////////////////////////////////////////////
 //只有在重量模式下显示正负号
     if((STAT_PCS == current_mode)||(STAT_100 == current_mode))  
       {//这两种模式下不显示正负号
       }
      else
      {
        if((TRUE == positive_flag)||(net_weight < display_min_value))
          Clr_Bar(105,155,30,7,COL_BLACK);
         else      
          Clr_Bar(105,155,30,7,COL_WHITE);
       }
  if(net_weight > display_min_value)     
  no_key_time = 0;     //确保有重量的情况下不进入待机模式 
    
  Set_ASC_Lib(GulimChe72_Font);
  
  if((STAT_CALING ==current_mode)||(STAT_CALING_INNER==current_mode))
       i = 0;   //校准过程中不进行 满量程检查
  else 
       i = normal_weigh_check(); 
  
  if(1 == i)
    {
     reflash_flag = 1;
     Set_Chr_Color(COL_DEEPYELLOW,COL_BLACK);
     Put_Str(160,110,"########");       
    }
  else if(2 == i) 
    {
     reflash_flag = 1;
     Set_Chr_Color(COL_THINRED,COL_BLACK);
     Put_Str(160,110,"$$$$$$$$");
    } 
  else
    { 
      if(1==reflash_flag)
        reflash_flag = 2;
      
      Set_Chr_Color(COL_DEEPGREEN,COL_BLACK); 
      
      switch(current_mode)
       {
        case STAT_CALING:       display_caling_data();break;
        case STAT_WEIGH:        display_weight_data();break;
        case STAT_PCS:             display_pcs_data();break;
        case STAT_100:             display_100_data();break;
        ///////////////////////////////////////////////////moremode 
        case STAT_CHECK:         display_check_data();break;
        case STAT_DIFFER:     //display_weight_data();break;
                                display_differ_data();break;
        case STAT_TOTAL:         display_total_data();break;
        case STAT_ANIMAL:       display_animal_data();break;
        case STAT_DENSITY:     display_density_data();break;
        case STAT_PEAK:           display_peak_data();break;
        case STAT_TARGET:       display_target_data();break;
        case STAT_CALING_INNER: 
                          display_caling_inner_data();break;
        default:break;
       }
    }
 }   
//////////////////////////////////////////////////////
//23  显示 主显示窗口的重量数据
//////////////////////////////////////////////////////
static void  display_weight_main_area(float weight_data)
  {
   static uint32 unit_change_tmp=UNIT_NULL;
   
   switch(current_unit)
   {
    case UNIT_G:
        if(QIAN_DIVISION == dot_position)
          Put_Data_Float(180,110,"%7.3f",weight_data);
        else if(WAN_DIVISION == dot_position)
          Put_Data_Float(160,110,"%8.4f",weight_data);
        else
          Put_Data_Float(160,110,"%8.5f",weight_data);
        break;
    case UNIT_MG:
        if(QIAN_DIVISION == dot_position)
          Put_Data(180,110,"%6d",weight_data * G_TO_MG);
        else
          Put_Data_Float(160,110,"%8.1f",weight_data * G_TO_MG);
        break;    
   case UNIT_LB: //首先进行单位转换，然后再显示
        Put_Data_Float(160,110,"%8.6f",weight_data * G_TO_LB);
        break;      
   case UNIT_OZ://首先进行单位转换，然后再显示
        Put_Data_Float(160,110,"%8.6f",weight_data * G_TO_OZ);
        break;
   default:break;     
  } 
   //////////////////////////////////////////在称重或者其他模式下模式下单位显示
  if(unit_change_tmp != current_unit)
  {
   unit_change_tmp = current_unit;
   Unit_Display();
  }
 
  }
//////////////////////////////////////////////////////
//24   称重状态下的 显示 
//////////////////////////////////////////////////////
static void  display_weight_data(void)
 {
   // 小于零位 的情况没有考虑
   //normal_weigh_check();
   //删除其它模式下的报警信息
   Del_Alm(&warn_wps_out,ALM_LEV_WARN);
   Del_Alm(&warn_pcs_out,ALM_LEV_WARN);
   Del_Alm(&warn_100_out,ALM_LEV_WARN);
   Del_Alm(&warn_check_out,ALM_LEV_WARN); 
  
  //////////////////////////////////////// 
   //if(net_weight < display_min_value)
   //display_weight_main_area(0.0);
   //else
   display_weight_main_area(net_weight);
   
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    if(QIAN_DIVISION == dot_position)
    Put_Data_Float(220,265,"%7.3f",tare_weight);
    else if(WAN_DIVISION == dot_position)
    Put_Data_Float(220,265,"%8.4f",tare_weight);
    else 
    Put_Data_Float(220,265,"%8.5f",tare_weight);
    
    if(1 == debug_data_flag)
    {
     Put_Data(390,245,"%8d", debug_disp_data1);
     Put_Data(390,270,"%8d", debug_disp_data2);
     Put_Data(390,295,"%8d", zer0_data);
    }
       
}  

//////////////////////////////////////////////////////
//25   计数状态下的 显示 
//////////////////////////////////////////////////////
static void  display_pcs_data(void)
 {
   //uint32 pcs_number; 
   float x;
   //常规检查
   //normal_weigh_check();
   //额外检查 
   if(weight_per_pcs==0.0)  //
      {
       Log_Alm(&tips_sample_wps,ALM_LEV_TIPS);  
      }
   else if((weight_per_pcs>WPS_MAX)||(weight_per_pcs<WPS_MIN))  //太小
      {
       //Warning_Info_Flag = WPP_TOO_LOW;
       Log_Alm(&warn_wps_out,ALM_LEV_WARN);
       }
   else
      {
       Del_Alm(&warn_wps_out,ALM_LEV_WARN);
       
       pcs_number = net_weight/weight_per_pcs + PCS_ADJUST_COEF; //四舍五入
       if(pcs_number > PCS_MAX)
       {
        //Warning_Info_Flag = PCS_NUM_OVER;
        Log_Alm(&warn_pcs_out,ALM_LEV_WARN);
        Put_Data(200, 110, "%5d",PCS_MAX);  //超范围显示 99999
       }
       else
       {
        //Warning_Info_Flag = NULL; 
        Del_Alm(&warn_pcs_out,ALM_LEV_WARN);       
        Put_Data(200, 110, "%5d",pcs_number);
       }
     } 
       
    /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    if(QIAN_DIVISION == dot_position)
    Put_Data_Float(230,  250, "%7.3f",weight_per_pcs);
    else if(WAN_DIVISION == dot_position)
    Put_Data_Float(230,  250, "%8.4f",weight_per_pcs);
    else
    Put_Data_Float(230,  250, "%8.5f",weight_per_pcs);
    
    Clr_Bar(210,290,14,3,COL_THINPUPLE);
    if(FALSE == positive_flag)
    Clr_Bar(210,290,14,3,COL_WHITE);   //display + -
    if(QIAN_DIVISION == dot_position)
    Put_Data_Float(230,  280, "%7.3f",net_weight);
    else if(WAN_DIVISION == dot_position)
    Put_Data_Float(230,  280, "%8.4f",net_weight);
    else
    Put_Data_Float(230,  280, "%8.5f",net_weight);
} 
 
//////////////////////////////////////////////////////
//26  百分比状态下的 显示 
//////////////////////////////////////////////////////
static void  display_100_data(void)
 {
  //float number_100; 
  //常规检查
  //normal_weigh_check();
  //额外检查 
  
   ///////////////////////////////////////////////在此模式下检测是否有请求
   if((TRUE == do_sam100_flag) && (TRUE == stable_flag)&&(TRUE == positive_flag)&&(net_weight>=MIN_100)&&(net_weight<=MAX_100))
      {
       do_sam100_flag = FALSE;
       weight_compare_100 = net_weight;
      }
   
   if(weight_compare_100==0.0)
     {
      Log_Alm(&tips_sample_bench,ALM_LEV_TIPS);
     }   
   else if((weight_compare_100>MAX_100)||(weight_compare_100 < MIN_100))
      {
       //Warning_Info_Flag = COMP_TOO_LOW;
        Log_Alm(&warn_100_out,ALM_LEV_WARN);
       }
     else
      { 
        //Warning_Info_Flag = NULL;
        Del_Alm(&warn_100_out,ALM_LEV_WARN);
        number_100 = (net_weight/weight_compare_100)*100; //四舍五入 
        if(number_100 > TARGET_100_MAX)
        Log_Alm(&warn_100_data_out,ALM_LEV_WARN);
        else
        {
        Del_Alm(&warn_100_data_out,ALM_LEV_WARN);
        Put_Data_Float(180,  110, "%7.2f",number_100);
        }
      } 
    /////////////////////////////////////////////////display 2WINDOWS
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    Set_ASC_Lib(Arial20B_Font);
    if(QIAN_DIVISION == dot_position)
    Put_Data_Float(230,  250, "%7.3f",weight_compare_100);
    else if(WAN_DIVISION == dot_position)
    Put_Data_Float(230,  250, "%8.4f",weight_compare_100);
    else
    Put_Data_Float(230,  250, "%8.5f",weight_compare_100);
    
    Clr_Bar(210,290,14,3,COL_THINPUPLE);
    if(FALSE == positive_flag)
     Clr_Bar(210,290,14,3,COL_WHITE);            //display + -
    if(QIAN_DIVISION == dot_position)
    Put_Data_Float(230,  280, "%7.3f",net_weight);
    else if(WAN_DIVISION == dot_position)
    Put_Data_Float(230,  280, "%8.4f",net_weight);
    else
    Put_Data_Float(230,  280, "%8.5f",net_weight);      
}  

//////////////////////////////////////////////////////
//27   检重状态下的 显示 
///////////////////////////////////////////////////////
static void  display_check_data(void)
 {
  //常规检查
  //normal_weigh_check();
  //额外检查 
  
   //上下限检查
   if(limit1_weight==limit2_weight)
    {
     Log_Alm(&tips_input_limit,ALM_LEV_TIPS);
    }
   else if((net_weight>limit2_weight)||(net_weight<limit1_weight))
    {
    Set_Chr_Color(COL_DEEPRED,COL_BLACK);
    Log_Alm(&warn_check_out,ALM_LEV_WARN);
    }
   else
    Del_Alm(&warn_check_out,ALM_LEV_WARN);  
   //////////////////////////////////////// 
   display_weight_main_area(net_weight);
   
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    if(QIAN_DIVISION == dot_position)
     {
      Put_Data_Float(230,250,"%7.3f",limit1_weight);
      Put_Data_Float(230,280,"%7.3f",limit2_weight);
     }
    else if(WAN_DIVISION == dot_position) 
     {
      Put_Data_Float(230,250,"%8.4f",limit1_weight);
      Put_Data_Float(230,280,"%8.4f",limit2_weight);
     }
    else 
     {
      Put_Data_Float(230,250,"%8.5f",limit1_weight);
      Put_Data_Float(230,280,"%8.5f",limit2_weight);
     }
}  

//////////////////////////////////////////////////////
//28   差异状态下的 显示 
//////////////////////////////////////////////////////

static void  display_differ_data(void)
 {
   // 小于零位 的情况没有考虑
   //常规检查
   //normal_weigh_check();
   //额外检查 
   //////////////////////////////////////// 
   display_weight_main_area(net_weight);
   
  /////////////////////////////////////////////////display 2WINDOWS
  //  Set_ASC_Lib(Arial20B_Font);
  //  Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
  //  Put_Data_Float(220,265,"%8.4f",tare_weight); 
 }  

//////////////////////////////////////////////////////
//29 累加状态下的 显示
///////////////////////////////////////////////////////
static void  display_total_data(void)
 {
  //常规检查
  //normal_weigh_check();
  //额外检查 
 
  if((TRUE == do_totaladd_flag) && (TRUE == stable_flag)&& (total_times<TOTAL_MAX_TIMES))
     {
      do_totaladd_flag = FALSE;
      total_weight += net_weight;
      total_times++;
     }
 
   //////////////////////////////////////// 
   display_weight_main_area(net_weight);
   
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    if(QIAN_DIVISION == dot_position)
    Put_Data_Float(260,250,"%7.3f",total_weight);
    else if(WAN_DIVISION == dot_position)
    Put_Data_Float(260,250,"%8.4f",total_weight);
    else
    Put_Data_Float(260,250,"%8.5f",total_weight);
    
    Put_Data(270,280,"%3d",total_times);
}  

//////////////////////////////////////////////////////
//30  动物称重状态下的 显示
///////////////////////////////////////////////////////
static void  display_animal_data(void)
 {
  uint32 i,j; 

  //常规检查
  //normal_weigh_check();
  //额外检查 
  
  //////////////////////////////////////// 
   if((0 == animal_real_time)&&(TRUE == stable_flag)&&(FALSE == animal_ok)&&(TRUE == animal_start))
      {
       animal_weight = net_weight;
       animal_ok = TRUE;
      }
  ///////////////////////////////////////    
   if(TRUE == animal_ok)
     display_weight_main_area(animal_weight);   
    else
     display_weight_main_area(net_weight);
   
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    Put_Data(245,250,"%2d",animal_time);
    ///////////////////////////////////////////////////////使用进度条表示
    Clr_Bar(350, 265,150,35,COL_WHITE);
    Clr_Bar(350, 265,150*animal_real_time/animal_time,35,COL_DEEPRED);
   
    Put_Data(245,280,"%2d",animal_real_time); //实时操作时间
    
  }  

//////////////////////////////////////////////////////
//31  密度称重状态下的 显示
///////////////////////////////////////////////////////
static void  display_density_data(void)
 {
   // 小于零位 的情况没有考虑
   //常规检查
   //normal_weigh_check();
   //额外检查 
  
   //////////////////////////////////////// 
   display_weight_main_area(net_weight);
   
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
  
    if(QIAN_DIVISION==dot_position)
     {
      Put_Data_Float(270,250,"%7.3f",density_weight1);
      Put_Data_Float(270,280,"%7.3f",density_weight2);
     }
    else if(WAN_DIVISION==dot_position)
     {
      Put_Data_Float(270,250,"%8.4f",density_weight1);
      Put_Data_Float(270,280,"%8.4f",density_weight2);
     }
    else
     {
      Put_Data_Float(270,250,"%8.5f",density_weight1);
      Put_Data_Float(270,280,"%8.5f",density_weight2);
     } 
     
    Put_Data_Float(460,280,"%4.1f",density_temp);
    //Put_Data_Float(460,280,"%4.1f",temp_float);
}  
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//32  峰值称重状态下的 显示
///////////////////////////////////////////////////////
static void  display_peak_data(void)
 {
  
  //常规检查
  //normal_weigh_check();
  //额外检查 
  
  ////////////////////////////////////////工作期间保持最大值 
   if((TRUE == peak_start) && (peak_weight<net_weight))
     peak_weight = net_weight;
  //////////////////////////////////////// 
   if(TRUE == peak_start)
     display_weight_main_area(peak_weight);
    else
     display_weight_main_area(net_weight);
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
     if(QIAN_DIVISION == dot_position)
     {
      Put_Data_Float(230,250,"%7.3f",tare_weight);
      Put_Data_Float(230,280,"%7.3f",net_weight);
     }
    else if(WAN_DIVISION == dot_position) 
     {
      Put_Data_Float(230,250,"%8.4f",tare_weight);
      Put_Data_Float(230,280,"%8.4f",net_weight);
     }
    else 
     {
      Put_Data_Float(230,250,"%8.5f",tare_weight);
      Put_Data_Float(230,280,"%8.5f",net_weight);
     }
}  
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//33  目标称重状态下的 显示
///////////////////////////////////////////////////////
static void  display_target_data(void)
 {
  // static float peak_weight;
  //常规检查
  //normal_weigh_check();
  //额外检查 
  
  if((TRUE == do_samtarget_flag) && (TRUE == stable_flag)&&(TRUE == positive_flag)&&(net_weight>=TARGET_MIN)&&(net_weight<=TARGET_MAX))
      {
       do_samtarget_flag = FALSE;
       target_weight = net_weight;
      }
      
   if((net_weight < target_weight*(1-TARGET_OK))||(net_weight > target_weight*(1+TARGET_OK)))
   Set_Chr_Color(COL_WHITE,COL_BLACK); //不在目标范围内使用白色显示
    
   ///////////////////////////显示当前重量，并以颜色区分是否接近目标值
   display_weight_main_area(net_weight);
     
  /////////////////////////////////////////////////display 2WINDOWS
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINPUPLE);
    if(QIAN_DIVISION == dot_position)
    {
     Put_Data_Float(220,250,"%7.3f",tare_weight);
     Put_Data_Float(220,280,"%7.3f",target_weight);
    } 
    else if(WAN_DIVISION == dot_position)
    {
     Put_Data_Float(220,250,"%8.4f",tare_weight);
     Put_Data_Float(220,280,"%8.4f",target_weight);
    }
    else
    {
     Put_Data_Float(220,250,"%8.5f",tare_weight);
     Put_Data_Float(220,280,"%8.5f",target_weight);
    }
}  

///////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//34  校准状态下的 显示
//使用线性校准后的数据
///////////////////////////////////////////////////////
void  point2_cal_proc(uint32 weight_tmp)
   {
    static uint32 tmp;
    uint32 i;
    uint8  eeprom_tx_buf[8];
    
    if((USER_CAL_ZER0 == point2_cal_step)&&(TRUE == stable_flag)&&(weight_tmp>USER_ZER0_CODE_MIN)&&(weight_tmp<USER_ZER0_CODE_MAX)) //如果是确认0点阶段
       {
              zer0_data = weight_tmp;
              tare_data = 0;
              //本步骤 不影响线性校准零点 数据  这是用户校准的零点
              eeprom_tx_buf[0] = (uint8)(zer0_data>>24);
  	          eeprom_tx_buf[1] = (uint8)((zer0_data>>16)&0xff);
  	          eeprom_tx_buf[2] = (uint8)((zer0_data>>8)&0xff);
  	          eeprom_tx_buf[3] = (uint8)(zer0_data&0xff); 
  	 
  	          eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	          eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	          eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	          eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	          
  	          if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	          Write_EEPROM(EEP_WEIZERO_U2_ADDR, eeprom_tx_buf, 8);	 //连同备份寄存器写入
              else
              Write_EEPROM(EEP_WEIZERO_U1_ADDR, eeprom_tx_buf, 8);	 //连同备份寄存器写入
              //point2_cal_step = USER_CAL_ZER0_ADD;
              point2_cal_step = USER_CAL_FULL;
              //process_num = 0;
              stable_manual_break();
       }
    if((USER_CAL_ZER0_ADD == point2_cal_step)&&(TRUE == stable_flag)) //增加这步骤的目的 是把当前的零点作为线性校准的起始零点
       {
              zer0_data = weight_tmp;
              tare_data = 0;
               /*本步骤 不影响零点 数据
               eeprom_tx_buf[0] = (uint8)(zer0_data>>24);
  	           eeprom_tx_buf[1] = (uint8)((zer0_data>>16)&0xff);
  	           eeprom_tx_buf[2] = (uint8)((zer0_data>>8)&0xff);
  	           eeprom_tx_buf[3] = (uint8)(zer0_data&0xff); 
  	 
  	           eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	           eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	           eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	           eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	           Write_EEPROM(EEP_WEIZERO_ADDR, eeprom_tx_buf, 8);	 //连同备份寄存器写入
               */
               point2_cal_step = USER_CAL_FULL;
               //process_num = 0;
               stable_manual_break();
       }
       
     if((USER_CAL_FULL==point2_cal_step)&&(TRUE == stable_flag)&&(abs(weight_tmp-zer0_data)>(system_division*inner_code_step/2)))
       {       
               //使用线性校准后的数据作为标准砝码的满量程数据
               tmp = weight_tmp - zer0_data;
               //////////////////////////////////////////////////////////////////
               //////////////////////////////////////////////////////////////////
               weigh_coef1     =  user_cal_para/(tmp+0.5);		
			   full_code       =  full_range / weigh_coef1;               
			   inner_code_step =  full_code/system_division;
              
    	       point2_cal_ok    = TRUE;
    	       eeprom_tx_buf[0] = (uint8)(full_code>>24);
  	           eeprom_tx_buf[1] = (uint8)((full_code>>16)&0xff);
  	           eeprom_tx_buf[2] = (uint8)((full_code>>8)&0xff);
  	           eeprom_tx_buf[3] = (uint8)(full_code&0xff);
  	            
  	           eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	           eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	           eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	           eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	           
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_WEIFULL2_ADDR, eeprom_tx_buf, 8);
  	           else
  	           Write_EEPROM(EEP_WEIFULL1_ADDR, eeprom_tx_buf, 8);
  	           
               point2_cal_step = USER_CAL_OVER;
        }
   }

void  point2_cal_proc_inner(uint32 weight_tmp)
   {
    uint32 tmp;
    uint32 i;
    uint8  eeprom_tx_buf[12];
    
    if((USER_CAL_ZER0 == point2_cal_step)&&(TRUE == stable_flag)) //如果是确认0点阶段
       {
              zer0_data = weight_tmp;
              tare_data = 0;
              //本步骤 不影响线性校准零点 数据  这是用户校准的零点
              //马达 动作 到位后才进入下一步，否则报警退出
              motor_err_flag = motor_run_proc(LOAD_POSITION);
              if(1 == motor_err_flag)
              {
               End_Cal_Pro();
              }
              else
              {
               point2_cal_step = USER_CAL_FULL;
               stable_manual_break();
              }
       }
       // 此处更改为内校准 时候 零点与内校砝码的内码差至少是分辨率的5倍
     if((USER_CAL_FULL==point2_cal_step)&&(TRUE == stable_flag)&&(abs(weight_tmp-zer0_data)>system_division))
       {       
               //使用线性校准后的数据作为标准砝码的满量程数据
               tmp = weight_tmp - zer0_data;  
               
               ///////////////////////yyyyyyyyyyyyyyyy
               ///////////////////////////////////////////////////////////////////   
               weigh_coef1     = inner_cal_real.rec_float/tmp;        //得到新的系数
               
               full_code       =  full_range / weigh_coef1;               
			   inner_code_step =  full_code/system_division;
              
    	       ////以下代码与 外校一致
    	       point2_cal_ok    = TRUE;
    	       eeprom_tx_buf[0] = (uint8)(full_code>>24);
  	           eeprom_tx_buf[1] = (uint8)((full_code>>16)&0xff);
  	           eeprom_tx_buf[2] = (uint8)((full_code>>8)&0xff);
  	           eeprom_tx_buf[3] = (uint8)(full_code&0xff);
  	            
  	           eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	           eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	           eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	           eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	           
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_WEIFULL2_ADDR, eeprom_tx_buf, 8);
  	           else
  	           Write_EEPROM(EEP_WEIFULL1_ADDR, eeprom_tx_buf, 8);
  	           
                
               point2_cal_step = USER_CAL_OVER_PRE;
               stable_manual_break();
               //马达回位
               motor_err_flag = motor_run_proc(UNLOAD_POSITION);
        }
      if((USER_CAL_OVER_PRE == point2_cal_step)&&(TRUE == stable_flag))
       {       
        point2_cal_step = USER_CAL_OVER;
        zer0_data = weight_tmp; 
       } 
   
   }
static void  display_caling_data(void)
 {
     static uint32 process_num = 0;
     ////////////////校准时候的动作确认 移到数据处理中去，不在显示这里处理了 
     process_num = process_num+10;
      if(100 == process_num)
         process_num = 0;
     
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     if((USER_CAL_ZER0==point2_cal_step)||(USER_CAL_ZER0_ADD==point2_cal_step))
       {
        Put_Line_Str(140, 140,Language_Str("正在确认零点","Checking 0_Point"));
        Clr_Bar(120, 190,200,40,COL_WHITE);
        Clr_Bar(120+2*process_num, 192,15,36,COL_DEEPBLUE);
       }
     if(USER_CAL_FULL == point2_cal_step)
       {
        Put_Line_Str(350, 140,Language_Str("请加载砝码","Loading NOW"));
        Clr_Bar(120, 190,200,40,COL_DEEPBLUE);
        Clr_Bar(320, 190,200,40,COL_WHITE);
        Clr_Bar(320+2*process_num, 192,15,36,COL_DEEPBLUE);
       }
     if(USER_CAL_OVER == point2_cal_step)
       {
        process_num = 0;
        current_mode = STAT_WEIGH;
        point2_cal_start = FALSE;
        //////////////////////////////校准完成，清除温度相关信息，并将当前温度保存
        Del_Alm(&warn_temp_c_out,ALM_LEV_WARN);
        temp_cmp_point =  temp_float;
       }  
}  

static void display_caling_inner_data(void)
{
   static uint32 process_num = 0;
     ////////////////校准时候的动作确认 移到数据处理中去，不在显示这里处理了 
     process_num = process_num+10;
      if(100 == process_num)
         process_num = 0;
     
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     if((USER_CAL_ZER0==point2_cal_step)||(USER_CAL_ZER0_ADD==point2_cal_step))
       {
        Put_Line_Str(140, 140,Language_Str("正在确认零点","Checking 0_Point"));
        Clr_Bar(120, 190,200,40,COL_WHITE);
        Clr_Bar(120+2*process_num, 192,15,36,COL_DEEPBLUE);
       }
     if(USER_CAL_FULL == point2_cal_step)
       {
        Put_Line_Str(350, 140,Language_Str("加载砝码确认中","Loading NOW"));
        Clr_Bar(120, 190,200,40,COL_DEEPBLUE);
        Clr_Bar(320, 190,200,40,COL_WHITE);
        Clr_Bar(320+2*process_num, 192,15,36,COL_DEEPBLUE);
       }
     if(USER_CAL_OVER_PRE == point2_cal_step)
       {
        Put_Line_Str(350, 140,Language_Str("卸载砝码确认中","Unloading..."));
        Clr_Bar(120, 190,200,40,COL_DEEPBLUE);
        Clr_Bar(320, 190,200,40,COL_WHITE);
        Clr_Bar(320+2*process_num, 192,15,36,COL_DEEPBLUE);
       }  
  
     if(USER_CAL_OVER == point2_cal_step)
       {
        process_num = 0;
        current_mode = STAT_WEIGH;
        point2_cal_start = FALSE;
        //////////////////////////////校准完成，清除温度相关信息，并将当前温度保存
        Del_Alm(&warn_temp_c_out,ALM_LEV_WARN);
        temp_cmp_point =  temp_float;
       }  


}
//////////////////////////////////////////END OF THE FILE 