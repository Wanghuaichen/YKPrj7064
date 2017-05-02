//**********************************************************************
//文件说明              
//功能
//（1）维修调试模式下的功能实现
//函数列表
//1   主界面显示 初始化			Init_Main_Sel
//2   称重传感器显示 初始化		Init_Service_Sensor
//3   温度传感器显示 初始化		Init_Service_Temperature
//4   EEPROM 显示 初始化		Init_Service_EEprom
//5   触摸屏显示 初始化			Init_Service_AD7843
//6   马达显示 初始化 			Init_Service_Motor
//7   接口显示 初始化			Init_Service_Interface
//8   声音亮度显示 初始化		Init_Service_BriANDSpeaker
//9   屏幕保护显示 初始化		Init_Service_Screen
/************************************************************************/
/*10  Start_Service_process  主程序及其调度                             */
/************************************************************************/
//11  主界面 按键判断			Action_MainMenuCfg
//12  子界面 按键判断			Action_SubMainCfg
//13  EEPROM界面 按键判断 		Action_EEPromMainCfg
//14  接口界面 按键判断			Action_InterfaceMainCfg
//15  声音亮度界面 按键判断     Action_BrightSpeakerMainCfg
//16  屏幕测试按键判断          Action_ScreenTestMainCfg
//17  主界面按键处理            Menu_Serv_Main_Key(uint32 key_c,sint32 op);
//18  称重传感器按键处理        Menu_Serv_Sensor_Key(uint32 key_c,sint32 op);
//19  温度传感器按键处理        Menu_Serv_Temperature_Key(uint32 key_c,sint32 op);
//20  EEPROM 按键处理           Menu_Serv_EEprom_Key(uint32 key_c,sint32 op);
//21  触摸屏按键处理            Menu_Serv_AD7843_Key(uint32 key_c,sint32 op);
//22  马达按键处理              Menu_Serv_Motor_Key(uint32 key_c,sint32 op);
//23  接口按键处理              Menu_Serv_Interface_Key(uint32 key_c,sint32 op);
//24  声音亮度按键处理          Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op);
//25  屏幕测试界面按键处理      Menu_Serv_Screen_Key
//26  主界面显示                Serv_Main_Menu_Disp
//27  称重传感器界面显示        Serv_Sensor_Menu_Disp
//28  温度传感器界面显示        Serv_Temperature_Menu_Disp
//29  EEPROM  传感器界面显示    Serv_EEprom_Menu_Disp
//30  触摸屏界面显示            Serv_AD7843_Menu_Disp
//31  马达界面显示              Serv_Motor_Menu_Disp
//32  接口界面显示              Serv_Interface_Menu_Disp
//33  声音亮度界面显示          Serv_BriANDSpk_Menu_Disp
//34  屏幕测试界面显示          Serv_Screen_Menu_Disp

//**********************************************************************/
#include "service.h"
#include "screencal.h"
#include "menu.h"
#include "Font_Lib.H"
#include "Font_Lib2.H"
#include "disp.h"
#include "lcd2478.h"
#include "global.h"
#include "eeprom.h"
#include "i2c_com.h"
#include "type_def.h"
#include  "timer.h"
#include  "ad7843.h"
#include  "epm7064s.h"
////////////////////////////////////////////////////////////////////////
//
//本文件使用的静态变量
////////////////////////////////////////////////////////////////////////
static  uint32 model_id_full_tmp,model_id_posi_tmp,position_tmp;
static  uint32 eeprom_data,eeprom_zero_flash_flag,eeprom_full_flash_flag,eeprom_test_flash_flag;
static  uint32 eeprom_linear_data[10],eeprom_linear_flash_flag;

static  uint32 flash_scr_flag;
static  uint32 interface_data;
static  uint32 Motor_test_flag;
///////////////////////////////////////////////////////////////////////
//函数声明
//////////////////////////////////////////////////////////////////////
static  sint32 Menu_Serv_Main_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Sensor_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Temperature_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_EEprom_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Model_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Motor_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Interface_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Screen_Key(uint32 key_c,sint32 op);

void Serv_Main_Menu_Disp(void);
void Serv_Sensor_Menu_Disp(void);
void Serv_Temperature_Menu_Disp(void);
void Serv_EEprom_Menu_Disp(void);
void Serv_Model_Menu_Disp(void);
void Serv_Motor_Menu_Disp(void);
void Serv_Interface_Menu_Disp(void);
void Serv_BriANDSpk_Menu_Disp(void);
void Serv_Screen_Menu_Disp(void);
/////////////////////////////////////////////////////////////////////////

//**********************************************************************
//1函数  名:Init_Main_Sel
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void Init_Main_Sel(sint32 op)
{
    uint32 xy[8][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
	Ver_Line(80,SCR_YSIZE-5,SCR_XSIZE/2);	
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 210,40,Language_Str("请选择检测内容","Please choose one item")); 
    
    for(i=0;i<4;i++)
     Clr_Bar( 30,90+90*i,250,80,COL_DEEPGRAY);
    for(i=0;i<4;i++)
     Clr_Bar(360,90+90*i,250,80,COL_DEEPGRAY);
   	
   	Set_Chr_Color(COL_DEEPGREEN,COL_DEEPGRAY);
    Put_Line_Str( 50+60,120,Language_Str("SENSOR",      "SENSOR"));
	Put_Line_Str( 50+30,210,Language_Str("TEMPERATURE", "TEMPERATURE"));
	Put_Line_Str( 50+60,300,Language_Str("EEPROM",      "EEPROM"));
	Put_Line_Str( 50+80,390,Language_Str("MODEL",       "MODEL"));
	
	Put_Line_Str( 390+60,120,Language_Str("MOTOR",          "MOTOR"));
	Put_Line_Str( 390+30,210,Language_Str("INTERFACE",      "INTERFACE"));
	Put_Line_Str( 390+20,300,Language_Str("BRIGHT&SPEAKER", "BRIGHT&SPEAKER"));
	Put_Line_Str( 390+60,390,Language_Str("SCREEN",         "SCREEN"));

    ////////////////////////////////////////create keylist 
    for(i=0;i<4;i++)
    {
     xy[i][0] = 20+250/2;
     xy[i][1] = 120+90*i;
    }
     for(;i<8;i++)
     {
      xy[i][0] = 360+250/2;
      xy[i][1] = 120+90*(i-4);
     }
    
    Create_ButtonList(xy,8);            
 }
//**********************************************************************
//2   称重传感器界面初始化
//**********************************************************************
void Init_Service_Sensor(sint32 op)
{
    uint32 xy[3][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(  5,SCR_XSIZE-5,100);
	Hor_Line(  5,SCR_XSIZE-5,150);
    Hor_Line(  5,SCR_XSIZE-5,200);
    Hor_Line(  5,SCR_XSIZE-5,250);
    Hor_Line(  5,SCR_XSIZE-5,300);
    Hor_Line(  5,SCR_XSIZE-5,350);
    Ver_Line(100,350,SCR_XSIZE/2);
    
	
	Clr_Bar(200,400,100,50,COL_DEEPBLUE);
    Set_Chr_Color(COL_BLACK,COL_DEEPBLUE);
	Put_Line_Str( 230,415,Language_Str("置0","ZERO"));
	
	Clr_Bar(350,400,100,50,COL_DEEPBLUE);
    Set_Chr_Color(COL_BLACK,COL_DEEPBLUE);
	Put_Line_Str( 380,415,Language_Str("去皮","TARE"));
	
		
	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("电磁传感器数据测试","SENSOR DATA ANALYZE")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Put_Line_Str( 50,120,Language_Str("传感器原始数据",    "DATA1"));
	Put_Line_Str( 50,170,Language_Str("原始加工后的数据",  "DATA2"));
    Put_Line_Str( 50,220,Language_Str("第一级滤波数据",    "DATA3"));
	Put_Line_Str( 50,270,Language_Str("第二级滤波数据",    "DATA4"));
	Put_Line_Str( 50,320,Language_Str("第三级滤波数据",    "DATA5"));
	
   
   
     xy[0][0] = 200+100/2;
     xy[0][1] = 400+50/2;
     xy[1][0] = 350+100/2;
     xy[1][1] = 400+50/2;
     
     xy[2][0] = 500+100/2;
     xy[2][1] = 400+50/2;
   
     Create_ButtonList(xy,3);
     
 }
//**********************************************************************
//3   温度传感器界面初始化
//**********************************************************************
void Init_Service_Temperature(sint32 op)
{
    uint32 xy[1][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Hor_Line(5,SCR_XSIZE-5,200);
	Hor_Line(5,SCR_XSIZE-5,250);
	
	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("温度传感器数据测试","TEMP SENSOR ANALYZE")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Put_Line_Str( 50,120,Language_Str("温度传感器原始数据","DATA1"));
	Put_Line_Str( 50,170,Language_Str("温度值",            "DATA2"));
  
   
    for(i=0;i<1;i++)
    {
     xy[i][0] = 500+100/2;;
     xy[i][1] = 425;
    }
     Create_ButtonList(xy,1);          
 }
//**********************************************************************
//4   EEPROM 界面初始化
//**********************************************************************
void Init_Service_EEprom(sint32 op)
{
    uint32 xy[5][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);  //零点数据
    Hor_Line(5,SCR_XSIZE-5,200);  //满量程数据
    Hor_Line(5,SCR_XSIZE-5,250);  //读写测试
    Hor_Line(5,SCR_XSIZE-5,300);  //线性校数据
	Ver_Line(100,300,SCR_YSIZE/2);
		
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("EEPROM  数据测试","EEPROM DATA ANALYZE")); 
	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	Put_Line_Str(  50,115,Language_Str("零点数据",       "ZER0 DATA")); 
	Put_Line_Str(  50,165,Language_Str("满量程数据",     "FULL RANGE DATA")); 
	Put_Line_Str(  50,215,Language_Str("测试读写结果",   "TEST EEPROM")); 
	Put_Line_Str(  50,265,Language_Str("线性校准数据",   "LINEAR CAL DATA"));
	
	Clr_Bar( 20,380,100,50,COL_THINBLUE);
	Clr_Bar(140,380,100,50,COL_THINBLUE);
	Clr_Bar(260,380,100,50,COL_THINBLUE);
	Clr_Bar(380,380,100,50,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str(  5+30,  395,Language_Str("读零点",     "ZER0-R")); 
	Put_Line_Str(  Language_Pos(140,150),  395,Language_Str("读满量程",   "FULL-R")); 
	Put_Line_Str(15+270,  395,Language_Str("测试",       "TEST")); 
	Put_Line_Str(  Language_Pos(380,385),  395,Language_Str("读线性校",   "LINEAR-R")); 
	
	
	Clr_Bar(520,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 550,415,Language_Str("退出","EXIT"));
		  	
	
    //4个测试按钮
    for(i=0;i<4;i++)
    {
     xy[i][0] = 70 + 120*i;
     xy[i][1] = 405;
    }
    //1个退出按钮
    for(;i<5;i++)
    {
     xy[i][0] = 570;
     xy[i][1] = 430;
    }
    
    Create_ButtonList(xy,5);                   
 }
//**********************************************************************
//5 此项功能改为 修改机器型号
//**********************************************************************
/*
void Init_Service_Model_old(sint32 op)
{
   uint32 i,j;
   uint32 xy[11][2];
   
     //Create window
    Create_Window_Frame(10,75,330,400,3,COL_THINCYAN);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(30,80,Language_Str("型号选择","Model Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<6;i++)
    Bar(30,120+50*i,  30+90,160+50*i);
    
    for(i=0;i<3;i++)
    Bar(190,120+50*i,190+90,160+50*i);
    
    Bar(    70,120+50*6, 70+90,160+50*6);
    Bar(   180,120+50*6,180+90,160+50*6);
    //fill text
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Put_Line_Str(60, 130,"100");
    Put_Line_Str(60, 180,"120");
    Put_Line_Str(60, 230,"200");
    Put_Line_Str(60, 280,"220");
    Put_Line_Str(60, 330,"300");
    Put_Line_Str(60, 380,"500");
    
    Put_Line_Str(205, 130,"0.000");
    Put_Line_Str(200, 180,"0.0000");
    Put_Line_Str(195, 230,"0.00000");
    
    Put_Line_Str(Language_Pos(90, 100),  430,Language_Str("取消","ESC"));
    Put_Line_Str(Language_Pos(205,210),  430,Language_Str("确定","OK"));
   
    //create button list
    for(i=0;i<6;i++)
    {
    xy[i][0] = 30  + 90/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    for(;i<9;i++)
    {
    xy[i][0] = 190 + 90/2;
    xy[i][1] = 120 + 50*(i-6) + 20;
    }
    
    xy[i][0]   = 70  + 90/2;
    xy[i++][1] = 120 + 50*6 + 20;
    xy[i][0]   = 180 + 90/2;
    xy[i++][1] = 120 + 50*6 + 20;
    Create_ButtonList(xy,11); 
    //////display currentvalue
    model_id_tmp = model_id;
    
    DisplayLocation(model_id_tmp);            
 }
 */

void Init_Service_Model(sint32 op)
{
   uint32 i,j;
   uint32 xy[8][2];
   
     //Create window
    Create_Window_Frame(10,75,330,300,3,COL_THINCYAN);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(30,80,Language_Str("型号选择","Model Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<3;i++)
    Bar(30,120+50*i,  30+90,160+50*i);
    
    for(i=0;i<3;i++)
    Bar(190,120+50*i,190+90,160+50*i);
    
    Bar(    70,120+50*4, 70+90,160+50*4);
    Bar(   180,120+50*4,180+90,160+50*4);
    //fill text
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Put_Line_Str(60, 130,"100");
    Put_Line_Str(60, 180,"ADD");
    Put_Line_Str(60, 230,"SUB");
    
    Put_Line_Str(205, 130,"0.000");
    Put_Line_Str(200, 180,"0.0000");
    Put_Line_Str(195, 230,"0.00000");
    
    Put_Line_Str(Language_Pos(90, 100),  330,Language_Str("取消","ESC"));
    Put_Line_Str(Language_Pos(205,210),  330,Language_Str("确定","OK"));
   
    //create button list
    for(i=0;i<3;i++)
    {
    xy[i][0] = 30  + 90/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    for(;i<6;i++)
    {
    xy[i][0] = 190 + 90/2;
    xy[i][1] = 120 + 50*(i-3) + 20;
    }
    
    xy[i][0]   = 70  + 90/2;
    xy[i++][1] = 120 + 50*4 + 20;
    xy[i][0]   = 180 + 90/2;
    xy[i++][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,8); 
    //////display currentvalue
    
    model_id_full_tmp = model_id&0xffff;
    model_id_posi_tmp = dot_position;           
 }
 
 
//**********************************************************************
//6    马达 界面初始化
//**********************************************************************
void Init_Service_Motor(sint32 op)
{
    uint32 xy[11][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    Ver_Line(100,SCR_XSIZE-5,SCR_XSIZE/2);

	Set_Color(COL_DEEPGREEN);
	for(i=0;i<6;i++)
	Hor_Line(5,SCR_XSIZE-5,100+50*i);
	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str(220,40,Language_Str("电机电路测试","MOTOR TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
				
	for(i=0;i<5;i++)
	Clr_Bar( 50,105+50*i,200,40,COL_THINBLUE);
	
    for(i=0;i<5;i++)
	Clr_Bar(400,105+50*i,200,40,COL_THINBLUE);
	
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( Language_Pos(130,130),115,Language_Str("转动",  "START"));
	Put_Line_Str( Language_Pos(130,130),165,Language_Str("停止",  "STOP"));
	Put_Line_Str( Language_Pos(130,130),215,Language_Str("卸载",  "UNLOAD"));
    Put_Line_Str( Language_Pos(130,130),265,Language_Str("加载",  "LOAD"));
    Put_Line_Str( Language_Pos(130,130),315,Language_Str("保存",  "SAVE"));	

	Put_Line_Str( Language_Pos(450,450),115,Language_Str("连续测试",  "RUN TEST"));
	Put_Line_Str( Language_Pos(450,450),165,Language_Str("停止测试",  "ABORT"));
	Put_Line_Str( Language_Pos(450,450),215,Language_Str("保存系数",  "SAVE COEF"));
    Put_Line_Str( Language_Pos(450,450),265,Language_Str("系数调大",  "COEF+"));
    Put_Line_Str( Language_Pos(450,450),315,Language_Str("系数调小",  "COEF-"));	
    
    for(i=0;i<5;i++)
    {
     xy[i][0] = 150;
     xy[i][1] = 125+50*i;
    }
   for(;i<10;i++)
    {
     xy[i][0] = 500;
     xy[i][1] = 125+50*(i-5);
    }
    
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,11);   
 }
//**********************************************************************
//7   接口电路 界面初始化
//**********************************************************************
void Init_Service_Interface(sint32 op)
 {
    uint32 xy[4][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    Ver_Line(100,SCR_XSIZE-5,SCR_XSIZE/2);

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150); //title
    Hor_Line(5,SCR_XSIZE-5,200); //led1
	Hor_Line(5,SCR_XSIZE-5,250); //led2
	Hor_Line(5,SCR_XSIZE-5,300); //led3
	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("外围接口电路测试","CPU IO TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str(  50,120,Language_Str("输出接口","OUTPUT SIGNAL"));
	Put_Line_Str( 370,120,Language_Str("输入接口","INPUT  SIGNAL"));
	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	Put_Line_Str(  50,170,Language_Str("运行指示灯",  "SYSTEM RUN"));
	Put_Line_Str(  50,220,Language_Str("无线指示灯1", "WIRELESS LED1"));
	Put_Line_Str(  50,270,Language_Str("无线指示灯2", "WIRELESS LED2"));
	
	Clr_Bar(200,155,80,40,COL_THINBLUE);
	Clr_Bar(200,205,80,40,COL_THINBLUE);
	Clr_Bar(200,255,80,40,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( Language_Pos(220,210),165,Language_Str("开关",  "ON-OFF"));
	Put_Line_Str( Language_Pos(220,210),215,Language_Str("开关",  "ON-OFF"));
	Put_Line_Str( Language_Pos(220,210),265,Language_Str("开关",  "ON-OFF"));
	
    for(i=0;i<3;i++)
    {
     xy[i][0] = 240;
     xy[i][1] = 175+50*i;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,4);                     
 }

//**********************************************************************
//8   声音亮度 界面初始化
//**********************************************************************
void Init_Service_BriANDSpeaker(sint32 op)
 {
    uint32 xy[11][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    
    Set_Color(COL_DEEPGREEN);
    Ver_Line(5,475,SCR_XSIZE/2);
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(  100, 40,Language_Str("亮度调整","Brightness-Adjust"));
	Put_Line_Str(  350, 40,Language_Str("选项调整","Factory-Setup")); 
	
	Clr_Bar(50,100,200,50,COL_THINBLUE);  //亮度增加
	Clr_Bar(50,170,200,50,COL_THINBLUE);  //亮度减小
	Clr_Bar(50,240,200,50,COL_THINBLUE);  //亮度减小
	
    for(i=0;i<7;i++)
	Clr_Bar(370,100+50*i,150,40,COL_THINBLUE);  //

	Clr_Bar(100,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	/////////////////////////////////////////////////
	Put_Line_Str(130,415,Language_Str("退出","EXIT"));
	
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 100,120,Language_Str("亮度增加",  "BRIGHT ADD"));
	Put_Line_Str( 100,190,Language_Str("亮度减小",  "BRIGHT SUB"));
	Put_Line_Str( 100,260,Language_Str("调试开关",  "FACTORY DATA"));
		
	Put_Line_Str( Language_Pos(400,400-15),110 + 50*0,Language_Str("LOGO开关",     "LOGO display"));
	Put_Line_Str( Language_Pos(400,400-20),110 + 50*1,Language_Str("型号开关",     "MODEL display"));
	Put_Line_Str( Language_Pos(400,400-15),110 + 50*2,Language_Str("整克设置",     "0000 adjust "));
	Put_Line_Str( Language_Pos(400,400-15),110 + 50*3,Language_Str("载荷设置",     "LOAD  track"));
	Put_Line_Str( Language_Pos(400,400-15),110 + 50*4,Language_Str("重复性设置",   "REPEAT check"));
	Put_Line_Str( Language_Pos(400,400-15),110 + 50*5,Language_Str("通讯设置",     "COMM FUNC"));
	
	Put_Line_Str( Language_Pos(400,400-20),110 + 50*6,Language_Str("保存",                "  SAVE"));	
    
    for(i=0;i<7;i++)
    {
     xy[i][0] = 370 + 150/2;
     xy[i][1] = 120 + 50*i;
    }
    
     xy[i][0]   = 50  + 200/2;
     xy[i++][1] = 100 +  50/2;
     xy[i][0]   = 50  + 200/2;
     xy[i++][1] = 170 +  50/2;   //亮度调整按键
     
     xy[i][0]   = 50  + 200/2;
     xy[i++][1] = 240 +  50/2;   //调试窗口
     
  
     xy[i][0]   = 100 + 100/2; 
     xy[i++][1] = 400 +  50/2;   //增加退出按键
   
     Create_ButtonList(xy,11);                      
 }
//**********************************************************************
//9  屏幕测试 界面初始化
//**********************************************************************
void Init_Service_Screen(sint32 op)
{
   uint32 xy[3][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
   	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 250,40,Language_Str("屏幕颜色测试","SCREEN TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
	
	
	Clr_Bar(100,400,150,50,COL_THINBLUE);
	Clr_Bar(300,400,150,50,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 150,415,Language_Str("开始",  "START"));
	Put_Line_Str( 350,415,Language_Str("停止",  "STOP"));
	
    for(i=0;i<2;i++)
    {
     xy[i][0] = 175+200*i;
     xy[i][1] = 425;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,3);                
  
           
 }
 
 static void Server_Assi_Null(void)
{
 /////////////////// 
}
//******************************************************************************************************
//10函数 名 Start_Service_process
//函数功能:调试模式主菜单项目 及其调度
//入口参数:无
//出口参数:无 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//******************************************************************************************************
void  Start_Service_process(void)
{
 struct menu_str menu[9] = {
    {MENU_SERV_MAIN_SEL,        Init_Main_Sel,              Menu_Serv_Main_Key,           Serv_Main_Menu_Disp,      Server_Assi_Null},//主界面
    {MENU_SERV_SEL_SENSOR_OP,   Init_Service_Sensor,        Menu_Serv_Sensor_Key,       Serv_Sensor_Menu_Disp,      Server_Assi_Null},//电磁传感器
    {MENU_SERV_SEL_TEMP_OP,     Init_Service_Temperature,   Menu_Serv_Sensor_Key,  Serv_Temperature_Menu_Disp,      Server_Assi_Null},//温度传感器
    {MENU_SERV_SEL_EEPROM_OP,   Init_Service_EEprom,        Menu_Serv_EEprom_Key,       Serv_EEprom_Menu_Disp,      Server_Assi_Null},//EEPROM
    {MENU_SERV_SEL_MODEL_OP,    Init_Service_Model,         Menu_Serv_Model_Key,         Serv_Model_Menu_Disp,      Server_Assi_Null},//AD7843
    {MENU_SERV_SEL_MOTOR_OP,    Init_Service_Motor,         Menu_Serv_Motor_Key,         Serv_Motor_Menu_Disp,      Server_Assi_Null},//MOTOR
    {MENU_SERV_SEL_INTERFACE_OP,Init_Service_Interface,     Menu_Serv_Interface_Key, Serv_Interface_Menu_Disp,      Server_Assi_Null},//INTERFACE
    {MENU_SERV_SEL_BRISPK_OP,   Init_Service_BriANDSpeaker, Menu_Serv_BriANDSpk_Key, Serv_BriANDSpk_Menu_Disp,      Server_Assi_Null},//更改为亮度 工厂选项
    {MENU_SERV_SEL_SCREEN_OP,   Init_Service_Screen,        Menu_Serv_Screen_Key,       Serv_Screen_Menu_Disp,      Server_Assi_Null} //屏幕
    };//扩展口
    sint32 cur_op,next_op;    //当前选项和所切换到的选项(bits[7:0]:选项号;bits[15:8]:界面号;bits[23:16]:选项的显示状态)
    uint8 key_c;            //按键号
        
    cur_op  = 0xffffffff ;  
    next_op = MENU_SERV_MAIN_SEL;
    //next_op = MENU_SERV_SEL_MODEL_OP;
    
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//当前所处的界面和准备切换到的界面不一样
        {//初始化界面
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
          
        }
        cur_op = next_op;        //"界面/菜单/选项"之间的切换
        
        if(TRUE == flag_new_data) 
        {
            flag_new_data = FALSE;
            filter();
            update_new_data();
        }
        ////////////////////////////////////////////////////////10ms
        if (flag_10ms_ok)
        {
            flag_10ms_ok = FALSE;
            key_value = Get_PositionXY();     //键盘扫描
            next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_value,cur_op);    //选项处理
        }    
        ////////////////////////////////////////////////////////300ms
        if (flag_300ms_ok)
        {
            flag_300ms_ok = FALSE;    
            (*(menu[(cur_op>>8)&0xff].disp_pro))();    //显示处理函数
                      
        }
        ////////////////////////////////////////////////////////500ms
        if (flag_500ms_ok)
        {//500ms时间到
            flag_500ms_ok = FALSE;            
          
        }                
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*11  主界面 键盘响应函数MAIN
//////////////////////////////////////////////////*/
uint32  Action_MainMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<8;i++)
	   {
	     if((abs((*p).X_Position-x)< 300/2) && (abs((*p).Y_Position-y)< 80/2))
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
/*12  子项目键盘响应函数 
//////////////////////////////////////////////////*/
uint32  Action_SubMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<3;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*13  EEPROM键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_EEPromMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*14  interface 键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_InterfaceMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 200/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*14  interface 键盘响应函数
//////////////////////////////////////////////////*/
/*
uint32  Action_MotorfaceMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   
	   Key_Code = 0xAA55;
	   
	   for(i=0;i<11;i++)
	   {
	     if((abs((*p).X_Position-x)< 200/2) && (abs((*p).Y_Position-y)< 50/2))
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
*/	
/*///////////////////////////////////////////////////
/*15  brightness and speaker  键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_BrightSpeakerMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<11;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 60/2))
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
/*16  SCREEN TEST  键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_ScreenTestMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<3;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 50/2))
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
//**********************************************************************
//17函数名:Menu_Serv_Main_Key
//函数功能:主界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static  sint32 Menu_Serv_Main_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MainMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://sensor
  	           op_id = MENU_SERV_SEL_SENSOR_OP;
  	           break;
    	case 2://temperature
    	       //op_id = MENU_SERV_SEL_TEMP_OP;
    	       break;
       	case 3://eeprom
       	       op_id = MENU_SERV_SEL_EEPROM_OP;
       	       break;
       	case 4://ad7843
       	       op_id = MENU_SERV_SEL_MODEL_OP;
               break;
        case 5://内校准版本才可以 进入这个菜单
               if(INNER_CAL_VERSION)
  	           op_id = MENU_SERV_SEL_MOTOR_OP;
  	           break;
    	case 6://interface
    	       op_id = MENU_SERV_SEL_INTERFACE_OP;
    	       break;
       	case 7://brightness and speaker
       	       op_id = MENU_SERV_SEL_BRISPK_OP;
       	       break;
       	case 8://screen
       	       op_id = MENU_SERV_SEL_SCREEN_OP;
       	        break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//18   称重传感器界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Sensor_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_SubMainCfg((key_c>>16)&0xffff,key_c&0xffff);
    
    if(i != 0xaa55)
     {
      if(1 == i)
        do_zer0_flag = TRUE;
        
      else if(2 == i)
        //tare
        do_tare_flag = TRUE;
         
      else if(3 == i)
        op_id =  MENU_SERV_MAIN_SEL;
      else
       {
        ;
       }
     }
    return(op_id);
  }
//**********************************************************************
//19   温度传感器界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Temperature_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MainMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://CHINESE
  	           //language_var_tmp = CHINESE ; //Set_Language_Type(CHINESE);
  	           break;
    	case 2://ENGLISH
    	       //language_var_tmp = ENGLISH;  //Set_Language_Type(ENGLISH);
    	       break;
       	case 3://Esc
       	       //op_id =  MENU_OP_OPER;  // return
       	       break;
       	case 4://OK
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//20   EEPROM 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_EEprom_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_EEPromMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://read 0
  	           if(OK == Init_Zer0_Weigh1_Varible())
  	           eeprom_zero_flash_flag = TRUE;
  	           else
  	           eeprom_zero_flash_flag = FALSE;
  	           break;
    	case 2://read full range data
    	       if(OK == Init_Cal_Varible())
  	           eeprom_full_flash_flag = TRUE;
  	           else
  	           eeprom_full_flash_flag = FALSE;
    	       break;
       	case 3://test eeprom 
       	       if(OK == EEPROM_RW_Test())
  	           eeprom_test_flash_flag = TRUE;
  	           else
  	           eeprom_test_flash_flag = FALSE;
    	       break;
       	case 4://read linear cal data
       	     if(OK == Init_LineCal_Varible())
  	           eeprom_linear_flash_flag = TRUE;
  	           else
  	           eeprom_linear_flash_flag = FALSE;
    	       break;
       	case 5://exit
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
  
//**********************************************************************
//21a   
//**********************************************************************
uint32  Action_ModelCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<11;i++)
	   {
	     if((abs((*p).X_Position-x)< 90/2) && (abs((*p).Y_Position-y)< 50/2))
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
  
//**********************************************************************
//21   
//**********************************************************************
static  sint32 Menu_Serv_Model_Key(uint32 key_c,sint32 op)
  {
    sint32 op_id;
    uint8  tmp,eeprom_tx_buf[8];  
    uint32 i,j;
    op_id = op;         //first
    i = Action_ModelCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1:  break;
  	    case 2:
  	             model_id_full_tmp+=10;
  	             if(model_id_full_tmp>500)
  	                model_id_full_tmp = 100;
  	                break;
  	    case 3:
  	             model_id_full_tmp-=10;
  	             if(model_id_full_tmp < 100)
  	                model_id_full_tmp = 500;
  	                break;
  	    
  	    case 4:  model_id_posi_tmp = 0;  break;
  	    case 5:  model_id_posi_tmp = 1;  break;
  	    case 6:  break;
  	    
  	    case 7://return
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;
       	case 8://ok
       	       tmp = model_id_posi_tmp + (model_id_full_tmp&0xff) + (model_id_full_tmp>>8)&0xff;
       	       model_id = (tmp<<24) + (model_id_posi_tmp<<16) + model_id_full_tmp;
       	       eeprom_tx_buf[3] = (model_id>>24) &0xff;
       	       eeprom_tx_buf[2] = (model_id>>16) &0xff;
       	       eeprom_tx_buf[1] = (model_id>>8)  &0xff;
       	       eeprom_tx_buf[0] =  model_id      &0xff;
  	           Write_EEPROM(EEP_MODEL_ID_ADDR, eeprom_tx_buf, 4);
  	           
  	           for(i=0;i<2000000;i++)
  	           {;}
  	           eeprom_tx_buf[0] = 0xee; eeprom_tx_buf[1] = 0xee;
  	           eeprom_tx_buf[2] = 0xee; eeprom_tx_buf[3] = 0xee;
  	            
  	           eeprom_tx_buf[4] = 0xcc; eeprom_tx_buf[5] = 0xcc;
  	           eeprom_tx_buf[6] = 0xcc; eeprom_tx_buf[7] = 0xcc; 
  	           
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_WEIFULL2_ADDR, eeprom_tx_buf, 8);
  	           else
  	           Write_EEPROM(EEP_WEIFULL1_ADDR, eeprom_tx_buf, 8);
  	          
  	           for(i=0;i<2000000;i++)
  	           {;}
  	           eeprom_tx_buf[0] = 0xee; eeprom_tx_buf[1] = 0xee;
  	           eeprom_tx_buf[2] = 0xee; eeprom_tx_buf[3] = 0xee;
  	            
  	           eeprom_tx_buf[4] = 0xcc; eeprom_tx_buf[5] = 0xcc;
  	           eeprom_tx_buf[6] = 0xcc; eeprom_tx_buf[7] = 0xcc; 
  	           
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_INNER_CAL2_ADDR, eeprom_tx_buf, 8);
  	           else
  	           Write_EEPROM(EEP_INNER_CAL1_ADDR, eeprom_tx_buf, 8);
  	          
  	       
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  
  }
//**********************************************************************
//22  马达界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Motor_Key(uint32 key_c,sint32 op)
{
    static uint32 inner_cal_factory=0,tmp;
    static uint32 motor_tmp;
    static uint32 motor_stop_flag,motor_stop_cnt;
    sint32 op_id;
    uint32 i;
    uint8  eeprom_tx_buf[8];  
 
    op_id = op;         //first
  
  ////////////////////////////////////////////////////////////////
 //马达连续测试代码
 if(TRUE == Motor_test_flag)
    {
      
       if(FALSE == motor_stop_flag)
         {
           if(((~motor_tmp)&0x01) != MOTOR_POSITION)
              {
                FIO0CLR = EPM7_MOTOR_P1;FIO0SET = EPM7_MOTOR_P2;
              }
           else
              {
               FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;
               motor_stop_flag = TRUE;
               motor_stop_cnt  = 300;
              }
         }
       else
         {
           FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;
          motor_stop_cnt--;
          if(0==motor_stop_cnt)
              {
               motor_stop_flag = FALSE;
               motor_tmp = MOTOR_POSITION;
              }         
         }
    }
  else
    {
     motor_tmp       = MOTOR_POSITION;
     motor_stop_flag = FALSE;
     motor_stop_cnt  = 0;
    } 
  /////////////////////////////////////////////////////////////////////
  
    //i = Action_MotorfaceMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    i = Action_BrightSpeakerMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://马达转动 不管正反转
  	           position_tmp++;
  	           inner_cal_factory = 0;
  	           if(FALSE == Motor_test_flag)
  	           motor_err_flag = motor_run_proc(position_tmp&0x01);
  	           break;
    	case 2://马达停止
    	       Motor_test_flag = FALSE;
    	        FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;
    	       inner_cal_factory = 0;
  	           break;
       	case 3://卸载
       	       if(FALSE == Motor_test_flag)
       	         {
       	          motor_err_flag = motor_run_proc(UNLOAD_POSITION);
       	          Set_Zer0_Flag();
       	          
       	          inner_cal_factory = 1;
       	         }
       	       break;
       	case 4://加载
       	       if((1==inner_cal_factory)&&(TRUE==stable_flag)&&(0==motor_err_flag)&&(FALSE==Motor_test_flag))
       	         {
       	          tmp = new_data_big;
       	          /////////////////
       	          motor_err_flag = motor_run_proc(LOAD_POSITION);
       	          inner_cal_factory++;
       	         }
       	         break;  
       	case 5://保存(加载-卸载)的数据
       	       if((2==inner_cal_factory)&&(TRUE==stable_flag)&&(0==motor_err_flag)&&(FALSE==Motor_test_flag))
       	         {
       	          tmp = new_data_big - tmp;
       	          
       	          eeprom_tx_buf[0] = (uint8)(tmp>>24);
  	              eeprom_tx_buf[1] = (uint8)((tmp>>16)&0xff);
  	              eeprom_tx_buf[2] = (uint8)((tmp>>8)&0xff);
  	              eeprom_tx_buf[3] = (uint8)(tmp&0xff);
  	            
  	              eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	              eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	              eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	              eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	           
  	              /*  此数据不再保存  2016.06.12
  	              if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	                 Write_EEPROM(EEP_INNER_CAL2_ADDR, eeprom_tx_buf, 8);
  	              else
  	                 Write_EEPROM(EEP_INNER_CAL1_ADDR, eeprom_tx_buf, 8);
  	              */
  	              Set_ASC_Lib(Arial16B_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
                  Put_Str(270,315,"SAVE OK");
                  inner_cal_factory = 0;
        	     }
       	         break;
       
       	case 6://
       	       Motor_test_flag = TRUE;
       	       break;      
       	case 7://
       	       Motor_test_flag = FALSE;
       	       FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;
       	       break;      
       	case 8://
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
       	       ////////////////////////////////
       	       Set_ASC_Lib(Arial16B_Font);
               Set_Chr_Color(COL_WHITE,COL_BLACK);
               Put_Str(270,315,"SAVE OK");
       	       break;      
       	case 9://
       	       full_code+=10;
       	       break;      
       	case 10://
       	       full_code-=5;
       	       break;
       	case 11:
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
      
    return(op_id);
  }  
//**********************************************************************
//23  接口 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Interface_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
   // return(op_id);
    i = Action_InterfaceMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://系统运行灯取反
  	           LED_CPU_RUN_Cpl();
  	           break;
    	case 2://WIRELESS LED2
    	       Wireless_Run_Led2_Cpl();
    	       break;
       	case 3://WIRELESS LED1
       	       Wireless_Run_Led1_Cpl();
       	       break;
       	case 4://EXIT
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//24  声音亮度界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op)
{
    uint8  buf[8];
    uint32 i,j;
    sint32 op_id;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_BrightSpeakerMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
        case 1:
                 logo_display_flag =   (logo_display_flag+1)&0x01;
                 break;
        case 2:
                 model_display_flag = (model_display_flag+1)&0x01;
                 break;
        case 3:
                 adjust_0000_flag =     (adjust_0000_flag+1)&0x01;
                 break;
        case 4:
                 load_track_flag =       (load_track_flag+1)&0x01;
                 break;
        case 5:
                 repeat_check_flag =   (repeat_check_flag+1)&0x01;
                 break;
        case 6:
                 Uart_Printer_Flag =   (Uart_Printer_Flag+1)&0x01;
                 break;
                 
        case 7:
                buf[0] = logo_display_flag;
                buf[1] = model_display_flag;
                buf[2] = adjust_0000_flag;
                buf[3] = load_track_flag;
                buf[4] = repeat_check_flag;
                buf[5] = Uart_Printer_Flag;
                buf[6] = debug_data_flag;
                buf[7] = buf[0]+buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6]+REC_CHECK_DATA1;
                I2C_Send_Str(SYS24C08, EEP_FACT_PARA_ADDR, buf, 8);
	            for(i=0;i<80000;i++)	{;}	         //delay
	            
	            Set_Chr_Color(COL_WHITE,COL_BLACK);
                Put_Line_Str(570,110 + 50*6,"OK");
               
                break;
  	    case 8://亮度增加
  	           lcd_brightness++;
  	           if(lcd_brightness > BRIGHT_CLASS_TOTAL)
  	              lcd_brightness = BRIGHT_CLASS_TOTAL;
  	           Set_LCD_Brightness(lcd_brightness);
  	           
  	           buf[0] = (uint8)(lcd_brightness);
  	           buf[1] = 0;
  	           buf[2] = 0;
  	           buf[3] = buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,buf, 4);
  	           break;
    	case 9://亮度减小
    	       if(lcd_brightness>1)
    	       lcd_brightness--;
    	       Set_LCD_Brightness(lcd_brightness);
    	       
    	       buf[0] = (uint8)(lcd_brightness);
  	           buf[1] = 0;
  	           buf[2] = 0;
  	           buf[3] = buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,buf, 4);
    	       break;
        case 10:
               
               debug_data_flag =  (debug_data_flag+1)&0x01;
               break;       
   		       
        case 11:
               //exit
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//25   屏幕测试 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Screen_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_ScreenTestMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://start
  	           flash_scr_flag = TRUE;
  	           break;
    	case 2://stop
    	       flash_scr_flag = FALSE;
    	       break;
        case 3://exit
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }

//**********************************************************************
//26函数名:Serv_Main_Menu_Disp               
//函数功能:主界面下的显示处理
//入口参数:无
//出口参数:无
//**********************************************************************
void Serv_Main_Menu_Disp(void)
{

}

//**********************************************************************
//27 称重传感器界面显示
//**********************************************************************
void Serv_Sensor_Menu_Disp(void)
 {
   uint32 i;
  
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(440,115,"%8d",Get_Raw());
    Put_Data(440,165,"%8d",Get_Raw1_Data());
    Put_Data(440,215,"%8d",Get_Ave1_Data());
    Put_Data(440,265,"%8d",Get_Ave2_Data());   
    Put_Data(440,315,"%8d",Get_Filter_Data());
    //
    
    if(FALSE == positive_flag)
    Put_Char(50,  400, '-');
    else
    Put_Char(50,  400, ' ');
  
    Put_Data_Float(80,400,  "%8.4f",net_weight);
    
  }
//**********************************************************************
//28  温度传感器界面显示
//**********************************************************************
void Serv_Temperature_Menu_Disp(void)
{
}
//**********************************************************************
//29  EEPROM  传感器界面显示
//**********************************************************************
void Serv_EEprom_Menu_Disp(void)
 {
  static uint32 flash1_time=0;
  static uint32 flash2_time=0;
  static uint32 flash3_time=0;
  static uint32 flash4_time=0;
  uint32 i;
  /////////////////////////////////////1
  if(TRUE == eeprom_zero_flash_flag)
    {
     eeprom_zero_flash_flag = FALSE;
     flash1_time = 50;   //300ms*50 = 15s
    } 
    else if(flash1_time>0)
    {
    flash1_time--;
    if(0 == (flash1_time%3))
       Clr_Bar(330,105,200,40,COL_BLACK);
    else   
      {
       Set_Chr_Color(COL_WHITE,COL_BLACK);
       Put_Data(330,110,"%8d",zer0_data);   
      }
    }
   /////////////////////////////////////2
  if(TRUE == eeprom_full_flash_flag)
    {
     eeprom_full_flash_flag = FALSE;
     flash2_time = 50;   //300ms*50 = 15s
    } 
    else if(flash2_time>0)
    {
    flash2_time--;
    if(0 == (flash2_time%3))
        Clr_Bar(330,155,200,40,COL_BLACK);
    else   
      {
       Set_Chr_Color(COL_WHITE,COL_BLACK);
       i = full_range / weigh_coef1;
       Put_Data(330,160,"%8d",i);  
      }
    }  
   /////////////////////////////////////3
  if(TRUE == eeprom_test_flash_flag)
    {
     eeprom_test_flash_flag = FALSE;
     flash3_time = 50;   //300ms*50 = 15s
    } 
    else if(flash3_time>0)
    {
    flash3_time--;
    if(0 == (flash3_time%3))
        Clr_Bar(330,205,200,40,COL_BLACK);
    else   
      {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      Put_Line_Str(330,210,Language_Str("测试通过","TEST OK"));   
      }
    }  
   /////////////////////////////////////4
  if(TRUE == eeprom_linear_flash_flag)
    {
     eeprom_linear_flash_flag = FALSE;
     flash4_time = 150;   //300ms*150 = 50s
    } 
    else if(flash4_time>0)
    {
    flash4_time--;
    if(0 == (flash4_time%5))
        Clr_Bar(10,305,620,70,COL_BLACK);
    else   
      {
       Set_Chr_Color(COL_WHITE,COL_BLACK);
       for(i=0;i<5;i++)
       Put_Data(10+100*i,310,"%8d",Point10_Cal_Buf[i]+1); 
       for(i=0;i<5;i++)
       Put_Data(10+100*i,340,"%8d",Point10_Cal_Buf[i+5]+1);   
      }
    }  
  }
//**********************************************************************
//30  
//**********************************************************************
void Serv_Model_Menu_Disp(void)
 {
  static uint16 full_tmp = 0xff;
 
  
  if(full_tmp != model_id_full_tmp)
   {
     full_tmp = model_id_full_tmp;
   
     Set_ASC_Lib(Arial16B_Font); 
     Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
     Put_Data(60, 130, "%3d",full_tmp);
    
   }
  
     ;
       
    if(0 == model_id_posi_tmp)
      {
       Set_Graphic_Color(COL_DEEPBLUE,COL_WHITE);
       Circle(310,140+0,9);
       Circle(310,140+0,8);
       Circle(310,140+0,7);
       Fill_Circle(310,140,3);
     
       Set_Graphic_Color(COL_THINCYAN,COL_THINCYAN);
       Circle(310,140+50,9);
       Circle(310,140+50,8);
       Circle(310,140+50,7);
       Fill_Circle(310,140+50,3); 
      }
      
     else
      {
      
       Set_Graphic_Color(COL_DEEPBLUE,COL_WHITE);
       Circle(310,140+50,9);
       Circle(310,140+50,8);
       Circle(310,140+50,7);
       Fill_Circle(310,140+50,3);
      
       Set_Graphic_Color(COL_THINCYAN,COL_THINCYAN);
       Circle(310,140,9);
       Circle(310,140,8);
       Circle(310,140,7);
       Fill_Circle(310,140,3);
      } 
   }   
//**********************************************************************
//31  马达界面显示
//**********************************************************************
void Serv_Motor_Menu_Disp(void)
{
  
 Set_ASC_Lib(Arial16B_Font);
 Set_Chr_Color(COL_WHITE,COL_BLACK);
 Put_Data(30,370,"%3d",position_tmp);
 
 if(MOTOR_POSITION&0x01)
    Put_Data(30,400,"%8d",new_data_big); 
 else
    Put_Data(30,430,"%8d",new_data_big); 
 
 
 Put_Data_Float(210,370, "%8.4f",net_weight);                  //内校砝码重量
 
 
 if(OK == inner_cal_flag)   
   {
    //显示已存储的数据
    Put_Data_Float(210,400, "%8.4f",inner_cal_real.rec_float);;   //系统校内码
   }
  else 
   {
    Put_Data_Float(210,400, "%8.4f",765.4321);   //系统校内码 
   }  
  
}
//**********************************************************************
//32  接口界面显示
//**********************************************************************
void Serv_Interface_Menu_Disp(void)
{
}
//**********************************************************************
//33  声音亮度界面显示
//**********************************************************************
void Serv_BriANDSpk_Menu_Disp(void)
{
  Set_Chr_Color(COL_WHITE,COL_BLACK);
  Put_Data(570,110 + 50*0,"%01d",logo_display_flag);
  Put_Data(570,110 + 50*1,"%01d",model_display_flag);
  Put_Data(570,110 + 50*2,"%01d",adjust_0000_flag);
  Put_Data(570,110 + 50*3,"%01d",load_track_flag);
  Put_Data(570,110 + 50*4,"%01d",repeat_check_flag);
  Put_Data(570,110 + 50*5,"%01d",Uart_Printer_Flag);

  Put_Data(265,255,       "%01d",debug_data_flag);
  
  
}
//**********************************************************************
//34  屏幕测试界面显示
//**********************************************************************
void Serv_Screen_Menu_Disp(void)
{
 uint32 i;
 //800/16 = 50
 static  uint32 j;
 static  uint32 flash_var = 0;
 static  uint32 flash_col_var = 0;
 
 flash_var++;
 if(flash_var > 16)
  {
   flash_var = 0;
   j++;
   if(16 == j)
   j=0;
  }
  
 if(TRUE == flash_scr_flag)
 {
  Clr_Bar(40*flash_var,100,40,260,(flash_col_var+j)%16); //实现偏移显示
  flash_col_var++;
  if(16 == flash_col_var)
   flash_col_var = 0;
  
 }
 
}