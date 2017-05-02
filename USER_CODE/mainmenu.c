//**********************************************************************
//�ļ�˵��             
//(1)���˵���ʾ�봦��
//(2)���ֹ��ܰ����Ĵ���
//(3)10��У׼��ʾ�봦��
//�����б�
//1    ��ĳ����Ʒ���        DisplayLocation
//2    ASD LNT PRT �����ж�  Action_LNTMenuCfg
//3    ��λѡ������ʼ��    UnitSel_Interface
//4    ASDѡ������ʼ��	 ASDSel_Interface
//5    LNTѡ������ʼ��	 LNTSel_Interface
//6    PRTѡ������ʼ��	 PRTSel_Interface
//7    ��λѡ�񰴼����� 	 UnitSel_Key_Pro
//8    ASDѡ�񰴼����� 		 ASDSel_Key_Pro
//9    LNTѡ�񰴼����� 		 LNTSel_Key_Pro
//10   PRTѡ�񰴼����� 		 PRTSel_Key_Pro 
//11   ��λѡ����ʾ���� 	 UnitSel_Disp_Pro
//12   ASDѡ����ʾ���� 		 ASDSSel_Disp_Pro
//13   LNTѡ����ʾ���� 		 LNTSel_Disp_Pro
//14   PRTѡ����ʾ����    	 PRTSel_Disp_Pro
/////////////////////////////////
//15   10��У׼�İ��� �ж�   Action_CAL_10pointMenuCfg
//16   10��У׼�����ʼ��    Cal_10point_Interface
//17   10��У׼��������      Cal_10point_Key_Pro
//18   10��У׼��ʾ����      Cal_10point_Disp_Pro

//**********************************************************************
#include "Font_Lib.h"

#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "normal_operation.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "type_def.h"

static   uint32 Cal_Point_Count,go_0_flag;
static   uint32 Cal_Buf_Tmp[CAL_POINT_NUM+1];     //��ʱ��

static   uint32 lnt_var_tmp,asd_var_tmp,prt_mod_tmp,unit_var_tmp,range_data_tmp;

/////////////////////////////////////////////////////
//1 ��ĳ����Ʒ���
/////////////////////////////////////////////////////
void  DisplayLocation(uint32 variable)
   {
     uint32 x,y;
     x = (variable>>16)&0xff;
     y = variable&0xff;
     Set_Graphic_Color(COL_DEEPBLUE,COL_WHITE);
   
     Circle(150,140+50*y,9);
     Circle(150,140+50*y,8);
     Circle(150,140+50*y,7);
     Fill_Circle(150,140+50*y,3);
    
     Circle(310,140+50*x,9);
     Circle(310,140+50*x,8);
     Circle(310,140+50*x,7);
     Fill_Circle(310,140+50*x,3);
     
     
   }
   
//////////////////////////////////////////////////
void  DisplayLocation_unit(uint32 variable)
   {
     Set_Graphic_Color(COL_DEEPBLUE,COL_WHITE);
   
     Circle(180,140+50*variable,9);
     Circle(180,140+50*variable,8);
     Circle(180,140+50*variable,7);
     Fill_Circle(180,140+50*variable,3);
   }

/*///////////////////////////////////////////////////
/*2  Action_LNTMenuCfg ���� �ж�
/*ASD LNT PRT ʹ��ͬһ������������
//////////////////////////////////////////////////*/
uint32  Action_LNTMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<6;i++)
	   {
	     if((abs((*p).X_Position-x)< DATE_CFG_WIDTH1/2) && (abs((*p).Y_Position-y)< DATE_CFG_HEIGTH1/2))
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
//3������:UnitSel_Interface
//��������:��λѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void UnitSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
   
     //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("��λѡ��","Unit Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Put_Button_Bar(20, 330, 20+90,  330+40,3,COL_THINBLUE);//esc
    Put_Button_Bar(130,330,130+90,  330+40,3,COL_THINBLUE);//ok
    //fill text
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Put_Line_Str(35, 130,Language_Str("��λ   ��",  "UNIT    g"));
    Put_Line_Str(35, 180,Language_Str("��λ ����",  "UNIT   mg"));
    Put_Line_Str(35, 230,Language_Str("��λ   ��",  "UNIT   Lb"));
    Put_Line_Str(35, 280,Language_Str("��λ ��˾",  "UNIT   Oz"));
   
    Set_Chr_Color(COL_BLACK,COL_THINBLUE);
    Put_Line_Str(Language_Pos( 40, 50), 340,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 340,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20  + 130/2;
    xy[i][1] = 140 + 50*i;
    }
    xy[i][0]   = 20  + 90/2;
    xy[i++][1] = 150 + 50*4;
    xy[i][0]   = 130 + 90/2;
    xy[i++][1] = 150 + 50*4;
    Create_ButtonList(xy,6); 
     
    //////display currentvalue
    unit_var_tmp = current_unit;
    
    DisplayLocation_unit(current_unit-1);              
}

//**********************************************************************
//4������:ASDSel_Interface
//��������:ASDѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void ASDSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
   
     //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("�ȶ���ѡ��","ASD Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Put_Line_Str(35, 130,Language_Str("�ȶ���0","   ASD-0"));
    Put_Line_Str(35, 180,Language_Str("�ȶ���1","   ASD-1"));
    Put_Line_Str(35, 230,Language_Str("�ȶ���2","   ASD-2"));
    Put_Line_Str(35, 280,Language_Str("�ȶ���3","   ASD-3"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6); 
     
    //////display currentvalue
    DisplayLocation(asd_var);
}
//**********************************************************************
//5������:LNTSel_Interface
//��������:LNT ѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void LNTSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
    
    //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("���Զ�ѡ��","LNT Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font); 
    Put_Line_Str(35, 130,Language_Str("����-0","LNT-0"));
    Put_Line_Str(35, 180,Language_Str("����-1","LNT-1"));
    Put_Line_Str(35, 230,Language_Str("����-2","LNT-2"));
    Put_Line_Str(35, 280,Language_Str("����-3","LNT-3"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6);
    //////display currentvalue 
    DisplayLocation(lnt_var);
  
}
///////////////////////////////////////////////////////////////////////
//**********************************************************************
//6������:PRTSel_Interface
//��������:PRT ѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void PRTSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
    
    //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("��ӡ��ѡ��","Printer Select"));
     
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font); 
    Put_Line_Str(35,    130,Language_Str("��ӡģʽ0","PRINT-0"));
    Put_Line_Str(35,    180,Language_Str("��ӡģʽ1","PRINT-1"));
    Put_Line_Str(35,    230,Language_Str("��ӡģʽ2","PRINT-2"));
    Put_Line_Str(35,    280,Language_Str("��ӡģʽ3","PRINT-3"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6); 
    //////display currentvalue 
    DisplayLocation(prt_mod); 
}

//**********************************************************************
//7������:UnitSel_Key_Pro
//��������:��λѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 UnitSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://ASD-0
  	           unit_var_tmp = UNIT_G;
  	           break;
    	case 2://ASD-1
    	       unit_var_tmp = UNIT_MG;
    	       break;
       	case 3://ASD-2
       	       unit_var_tmp = UNIT_LB;
       	       break;
   		case 4://ASD-3
   		       unit_var_tmp = UNIT_OZ;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;       
   		case 6://ok
   		      current_unit = unit_var_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//8������:ASDSel_Key_Pro
//��������:ASD ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 ASDSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://ASD-0
  	           asd_var_tmp = 0;
  	           break;
    	case 2://ASD-1
    	       asd_var_tmp = 1;
    	       break;
       	case 3://ASD-2
       	       asd_var_tmp = 2;
       	       break;
   		case 4://ASD-3
   		       asd_var_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;       
   		case 6://ok
   		      asd_var = asd_var_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//9������:LNTSel_Key_Pro
//��������:LNT ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 LNTSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://lnt-0
  	           lnt_var_tmp = 0;
  	           break;
    	case 2://lnt-1
    	       lnt_var_tmp = 1;
    	       break;
       	case 3://lnt-2
       	       lnt_var_tmp = 2;
       	       break;
   		case 4://lnt-3
   		       lnt_var_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;       
   		case 6://ok
   		      lnt_var = lnt_var_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
////////////////////////////////////////////////////////////////////////
//**********************************************************************
//10������:PRTSel_Key_Pro
//��������:PRT ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 PRTSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
  	           prt_mod_tmp = 0;
  	           break;
    	case 2://ASD-1
    	       prt_mod_tmp = 1;
    	       break;
       	case 3://ASD-2
       	       prt_mod_tmp = 2;
       	       break;
   		case 4://ASD-3
   		       prt_mod_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;
   		case 6://ok
   		      prt_mod = prt_mod_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//11������:UnitSel_Disp_Pro               
//��������:��λѡ���µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void UnitSel_Disp_Pro(void)
{
  static uint32 unit_var_display;
  if(unit_var_display != unit_var_tmp)
  {
   unit_var_display = unit_var_tmp;
   //erase
   Clr_Bar(170,120,20,180,COL_DEEPGRAY);
   
   DisplayLocation_unit(unit_var_display-1);
  }   
 }

//**********************************************************************
//12������:ASDSel_Disp_Pro
//��������:asd�����µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void ASDSel_Disp_Pro(void)
{
  static uint32 asd_var_display;
  if(asd_var_display != asd_var_tmp)
  {
   asd_var_display = asd_var_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(asd_var_display);
  }
 } 
//**********************************************************************
//13������:LNTSel_Disp_Pro               
//��������:LNT CFG �µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void LNTSel_Disp_Pro(void)
{
  static uint32 lnt_var_display;
 
  if(lnt_var_display != lnt_var_tmp)
 {
   lnt_var_display = lnt_var_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(lnt_var_display);
  }
}
//**********************************************************************
//14������:PRTSel_Disp_Pro               
//��������:PRT CFG �µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void PRTSel_Disp_Pro(void)
{
  static uint32 prt_var_display;
 
  if(prt_var_display != prt_mod_tmp)
 {
   prt_var_display = prt_mod_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(prt_var_display);
  }
}

//////////////////////////////////////////////////
//���� �˵� ����ѡ��
//////////////////////////////////////////////////
void Display_SelRange(void)
{
  
   Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
   switch(range_data_tmp)
   {case WAN_DIVISION:
                 Circle(430,150,9);
                 Circle(430,150,8);
                 Circle(430,150,7);
                 Fill_Circle(430,150,4);
                 Clr_Bar(430-10,200-10,20,20,COL_DEEPGRAY);
                 break;
    case WAN10_DIVISION:
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
//  
//////////////////////////////////////////////////*/
uint32  Action_RangeMenuCfg(uint16 x,uint16 y)
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
//**********************************************************************	
void RangeSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
    
    //Create window
    Create_Window_Frame(256,80,200,220,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(270,85,Language_Str("����ѡ��","Range Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
    Bar(260+10,130,260+10+130,130+40);    //chinese
    Bar(260+10,180,260+10+130,180+40);    //english
    
    Put_Button_Bar(270, 250,270+80, 250+40,3,COL_BLACK);//esc
    Put_Button_Bar(360, 250,360+80, 250+40,3,COL_BLACK);//ok
  
    Set_Chr_Color(COL_THINGRAY,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270+20,  130+10,Language_Str("0.0001",   "0.0001"));
    Put_Line_Str(270+20,  190+ 0,Language_Str("0.00001",  "0.00001"));
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(290,280),  250+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(380,390),  250+10,Language_Str("ȷ��",   "OK"));
    //create button list
    xy[0][0] = 270+130/2;    xy[0][1] = 130+40/2;
    xy[1][0] = 270+130/2;    xy[1][1] = 180+40/2;
    xy[2][0] = 270+80/2;     xy[2][1] = 250+40/2;
    xy[3][0] = 360+80/2;     xy[3][1] = 250+40/2;
                  
    Create_ButtonList(xy,4); 
    ///////////////////////////////
    if(WAN10_DIVISION == dot_position)
    range_data_tmp = WAN10_DIVISION;
    else
    range_data_tmp = WAN_DIVISION;
    Display_SelRange();   
}
//**********************************************************************
//24 ����  ��:LangSel_Key_Pro

//**********************************************************************
sint32 RangeSel_Key_Pro(uint32 key_c,sint32 op)
{
    uint8  eeprom_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_RangeMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://���֮һ
  	           range_data_tmp = WAN_DIVISION;
  	           break;
    	case 2://ʮ���֮һ
    	       range_data_tmp = WAN10_DIVISION;
    	       break;
       	case 3://Esc
       	       op_id =  MENU_OP_OPER;  // return
       	       break;
       	case 4://�жϴ˴β�����û�б仯
       	       //���� ֻ�ܸı� С�����λ��
        	   /*    if(dot_position != range_data_tmp)
       	        {
       	         model_id = (model_id&0xffff) + (range_data_tmp<<16); 
       	         eeprom_tx_buf[0] = REC_CHECK_DATA1;
  	             eeprom_tx_buf[1] = model_id&0xff;       //������
  	             eeprom_tx_buf[2] = (model_id>>16)&0xff; //С��λ
  	             eeprom_tx_buf[3] = REC_CHECK_DATA2; 
  	             Write_EEPROM(EEP_MODEL_ID_ADDR, eeprom_tx_buf, 4);
  	             //������ɺ� ���鿪������
  	             Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
  	             Set_ASC_Lib(Arial16B_Font);
  	             Set_Graphic_Color(COL_WHITE,COL_WHITE);
                 Rectangle(230,200,440,290); 
  	             Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
				 Put_Line_Str(250,  220,Language_Str("�л��ֶȳɹ�","Screen Cal Successed!"));
				 Put_Line_Str(250,  250,Language_Str("��������������","Please Restart Machine!"));
				 
				 ///�������Ź����ȴ�ϵͳ�Լ���λ
				 //Init_WDT(500);                     //���Ź���ʼ��,ι�����=1s
				 while(1){;}
  	            }  
  	           else
  	           op_id =  MENU_OP_OPER;*/
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************

//**********************************************************************
void RangeSel_Disp_Pro(void)
{
   Display_SelRange();
  }



///////////////////////////////////////////////////////���������˵���Ŀ
///xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//**********************************************************************
//4������:ASDSel_Interface
//��������:ASDѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void Password_Adjust_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    which_para = DATA_PASSWORD;
    //op_id_tmp = MENU_PCS_INPUT_OPER; 

}
//**********************************************************************
//5������:LNTSel_Interface
//��������:LNT ѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void OperID_Adjust_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    which_para = DATA_USER_ID;
    //op_id_tmp = MENU_PCS_INPUT_OPER;   
}
///////////////////////////////////////////////////////////////////////
//**********************************************************************
//6������:PRTSel_Interface
//��������:PRT ѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void Factory_Restore_Interface(sint32 cur_menu_op)
{
  
}
//**********************************************************************
//8������:ASDSel_Key_Pro
//��������:ASD ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 PasswordCfg_Key_Pro(uint32 key_c,sint32 op)
{
   sint32 op_id;
   op_id = MENU_PCS_INPUT_OPER; //ǿ����ת����������Ի���
   return(op_id);
}
//**********************************************************************
//9������:LNTSel_Key_Pro
//��������:LNT ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 OperIDCfg_Key_Pro(uint32 key_c,sint32 op)
{
   sint32 op_id;
   op_id = MENU_PCS_INPUT_OPER; //ǿ����ת����������Ի���
   return(op_id);
}
////////////////////////////////////////////////////////////////////////
//**********************************************************************
//10������:PRTSel_Key_Pro
//��������:PRT ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 FactoryCfg_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
  	           prt_mod_tmp = 0;
  	           break;
    	case 2://ASD-1
    	       prt_mod_tmp = 1;
    	       break;
       	case 3://ASD-2
       	       prt_mod_tmp = 2;
       	       break;
   		case 4://ASD-3
   		       prt_mod_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;
   		case 6://ok
   		      prt_mod = prt_mod_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//11������:UnitSel_Disp_Pro               
//��������:��λѡ���µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void PasswordCfg_Disp_Pro(void)
 {
  uint32 i;
  i++;
  i++;
 }

//**********************************************************************
//12������:ASDSel_Disp_Pro
//��������:asd�����µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void OperIDCfg_Disp_Pro(void)
{
 uint32 i;
  i++;
  i++;
 } 
//**********************************************************************
//13������:LNTSel_Disp_Pro               
//��������:LNT CFG �µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void FactoryCfg_Disp_Pro(void)
{
  static uint32 lnt_var_display;
 
  if(lnt_var_display != lnt_var_tmp)
 {
   lnt_var_display = lnt_var_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(lnt_var_display);
  }
}


//yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
/*///////////////////////////////////////////////////
/*15
/*       10��У׼���� �ж�
//////////////////////////////////////////////////*/
uint32  Action_CAL_10pointMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     Key_Status = KEY_DO_OVER; 
	     
	   return(Key_Code);
	}
//**********************************************************************
//16������:Cal_10point_Interface
//��������:10��У׼�µĽ����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void Cal_10point_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
    
    //Create window
    Clr_Bar(0,70,SCR_XSIZE,410,COL_THINBLUE);   //TITLE
    //draw XY axia
    Set_Graphic_Color(COL_BLACK,COL_THINBLUE);
    ////////////////////////////////////////
    Ver_Line(70,475,38);
    Line(30, 90,38,70);       //�������ͷ
    Line(38, 70,46,90);
    /////////////////////////////////////// 
    Hor_Line(5,630,435);
    Line(620,435-8,630,  435);  //�������ͷ
    Line(630,  435,620,435+8);
    
    for(i=0;i<5;i++)
    Clr_Bar(540, 80+70*i,90, 60,COL_BLACK);
    
    for(i=0;i<5;i++)
    {xy[i][0] = 540+50;  xy[i][1] = 115+70*i;}
    //create button list
    Create_ButtonList(xy,5); 
    //fill text
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Set_ASC_Lib(Arial16B_Font); 
    Put_Line_Str(Language_Pos(560,550),  80+20,Language_Str("��ʼ",   " START"));
    Put_Line_Str(Language_Pos(550,540), 150+20,Language_Str("��һ��", "  LAST"));
    Put_Line_Str(Language_Pos(550,540), 220+20,Language_Str("��һ��", "  NEXT"));
    Put_Line_Str(Language_Pos(560,540), 290+20,Language_Str("����",   "  SAVE"));
    Put_Line_Str(Language_Pos(560,540), 360+20,Language_Str("�˳�",   "  EXIT"));
    
    //////    
    Cal_Point_Count = 0;           
}
//**********************************************************************
//17������:Cal_10point_Key_Pro
//��������:10��У׼�µ� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 Cal_10point_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    static uint8  Line_CalData[16];
	       uint32 CheckSum;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_CAL_10pointMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:                   //start
  	           Cal_Point_Count = 1;
  	           point10_cal_start = TRUE;
  	           stable_manual_break();
  	           break;
    	case 2://last
    	       /*if(Cal_Point_Count>1)  
    	       {Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPBLUE);
                Fill_Circle(38 + (Cal_Point_Count-1)*(550-38)/10,435-(Cal_Point_Count-1)*(435-100)/10,3); 
    	        Cal_Point_Count--;
    	        }*/
    	       break;
       	case 3://next
       	       /*
       	       if((Cal_Point_Num<11) && (Cal_Point_Num>0))
       	       {//record current data
       	        Set_Graphic_Color(COL_THINGREEN,COL_DEEPBLUE);
                Fill_Circle(38 + (Cal_Point_Num-1)*(550-38)/10,435-(Cal_Point_Num-1)*(435-100)/10,3); 
       	        // Cal_Buf_Tmp[Cal_Point_Num-1] = Get_Filter_Data();
       	        Cal_Buf_Tmp[Cal_Point_Num-1] = Cal_Point_Num*13 ;
       	       
       	        Cal_Point_Num++;
       	       }*/
       	       break;
       	case 4://save
       	       
       	       for(i=0;i<Cal_Point_Count-1;i++)
       	        Point10_Cal_Buf[i] = Cal_Buf_Tmp[i];
       	        //////////////////////////////////save to EEPROM
       	        CheckSum = 0;
       	       for(i=0;i<Cal_Point_Count-1;i++)  //����11�����У��ֵ
       	        CheckSum += Cal_Buf_Tmp[i];
       	        Cal_Buf_Tmp[Cal_Point_Count-1] = CheckSum;
       	       
       	       for(i=0;i<3;i++)  //ÿ��д16���ֽڣ�����4��������ݣ���Ҫ3��д��
       	       {
       	        for(j=0;j<4;j++)
       	        {
       	         Line_CalData[4*j+0] = Cal_Buf_Tmp[i*4+j]>>24;
       	         Line_CalData[4*j+1] = Cal_Buf_Tmp[i*4+j]>>16;
       	         Line_CalData[4*j+2] = Cal_Buf_Tmp[i*4+j]>>8;
       	         Line_CalData[4*j+3] = Cal_Buf_Tmp[i*4+j]&0xff;
       	        }
       	        
       	        Write_EEPROM(EEP_LINCAL1_ADDR01+i*16, Line_CalData, 16); /* ����ʼ��ַ��ʼд��16������*/
			    for(j=0;j<800000;j++)	{;}	                 //delay
       	        
       	        } 
       	        /////////////////////////////////////////CHECK
       	        //////////////////////////////////////////////use current data
       	        point10_cal_start = FALSE;
       	        point10_cal_ok    = TRUE;
       	        Get_Linecal10_para(Point10_Cal_Buf[10]);
       	        
       	        /*
       	       for(i=0;i<5;i++)
       	        Point10_Cal_Buf[i] = Cal_Buf_Tmp[i];
       	        //////////////////////////////////save to EEPROM
       	        CheckSum = 0;
       	       for(i=0;i<5;i++)  //����5�����У��ֵ
       	        CheckSum += Cal_Buf_Tmp[i];
       	        Cal_Buf_Tmp[5] = CheckSum;
       	       
       	       for(i=0;i<2;i++)  //ÿ��д16���ֽڣ�����4��������ݣ���Ҫ3��д��
       	       {
       	        for(j=0;j<4;j++)
       	        {
       	         Line_CalData[4*j+0] = Cal_Buf_Tmp[i*4+j]>>24;
       	         Line_CalData[4*j+1] = Cal_Buf_Tmp[i*4+j]>>16;
       	         Line_CalData[4*j+2] = Cal_Buf_Tmp[i*4+j]>>8;
       	         Line_CalData[4*j+3] = Cal_Buf_Tmp[i*4+j]&0xff;
       	        }
       	        
       	        if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
       	        Write_EEPROM(EEP_LINCAL2_ADDR01+i*16, Line_CalData, 16); // ����ʼ��ַ��ʼд��16������
       	        else
			    Write_EEPROM(EEP_LINCAL1_ADDR01+i*16, Line_CalData, 16); // ����ʼ��ַ��ʼд��16������
			   
			    for(j=0;j<800000;j++)	{;}	                 //delay
       	        
       	        } 
       	        /////////////////////////////////////////CHECK
       	        //////////////////////////////////////////////use current data
       	        point10_cal_start = FALSE;
       	        point10_cal_ok = TRUE;
       	        Get_Linecal_Var();*/
       	        
       	        //////////////////////////////////////////////////////////////
       	        op_id =  MENU_OP_OPER; //�ص�������        
   		        break;
   		case 5://end
   		        op_id =  MENU_OP_OPER; //�ص�������
   		        break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//18������:Cal_10point_Disp_Pro               
//��������:10��У׼�µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************

void Cal_10point_Disp_Pro(void)
{
   uint16  i;
   static uint8 tmp = 0xff;
   
   Set_Graphic_Color(COL_DEEPRED,COL_THINBLUE);//x0,y0 {38,435} ; xMAX yMAX {550,100}
   Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);   //X length:660-38  Y length:435-110  
   Set_ASC_Lib(Arial16B_Font);
   if(tmp != Cal_Point_Count)
   {
     tmp = Cal_Point_Count;
     
     if(Cal_Point_Count > 1)
     i = line_cal_para * (Cal_Point_Count-1) / 10;  
    
    switch(Cal_Point_Count)
     {
      case 0: Put_Line_Str(170, 450,Language_Str("У׼δ��ʼ",    "NOT START"));     break;
      case 1: Put_Line_Str(170, 450,Language_Str("����ȷ�����",  "CHECKING ZERO")); break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
              Put_Line_Str(                 170,          450,Language_Str("�����     ������",  "PLS LOAD XXXg "));
             
              Put_Data(Language_Pos(170+24*3+10, 170+40), 450, "%03d", i);
              break;
      case 12:
              Put_Line_Str(170, 450,Language_Str("�밴�������������У׼",  "PRESS SAVE KEY AND STOP LINE_CALING")); break;   
      default:break; 
     }
   }
 }
 
 
void Cal_10point_Disp_Pro_old(void)
{
  uint32 i,j;
   
  static uint32 flash_time,flash_flag;
  static uint32 Cal_Point_Tmp;
 
  flash_time++;
  if(flash_time%7 == 0)
  flash_flag++;
  if(flash_flag & 0x01)                  //flash 
  {
   Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
   Set_Chr_Color(COL_THINBLUE,COL_THINBLUE);
   }
  else
   {
    Set_Graphic_Color(COL_DEEPRED,COL_THINBLUE);//x0,y0 {38,435} ; xMAX yMAX {550,100}
    Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);   //X length:660-38  Y length:435-110
   }
    
  Set_ASC_Lib(Arial16B_Font);
  if(Cal_Point_Count>0)
   {
     Fill_Circle(38 + (Cal_Point_Count-1)*(530-38)/10,435-(Cal_Point_Count-1)*(435-100)/10,3);
     if(1 == Cal_Point_Count)
       Put_Line_Str(170, 450,Language_Str("����ȷ�����","Checking 0_Point"));
     else
      {Clr_Bar(170,450,400,25,COL_THINBLUE);
       ////////////////////////////////////////////////////
       if((TRUE == go_0_flag)&&(FALSE == auto_zer0_valid))
        {
         
         Put_Line_Str(170,     450,Language_Str("��ȴ�����","Please Waiting for 0_Point"));
         } 
       else if(TRUE == auto_zer0_valid)
               {
                go_0_flag = FALSE; 
                Put_Line_Str(170,     450,Language_Str("�����","Please Loading"));
                Put_Data(170+72,  450,"%3d",(Cal_Point_Count-1)*10);
                Put_Line_Str(170+105, 450,Language_Str("% ����������","% Full-Load"));
                }
          
       }
   }
 
  //////////////////////////////////////////////////////////
  if((Cal_Point_Tmp != Cal_Point_Count) && (Cal_Point_Count>0))
   { 
    Cal_Point_Tmp = Cal_Point_Count;  
    Set_Graphic_Color(COL_BLACK,COL_THINBLUE);
    Line(38,435,530,100);        //������
                                 //������������������
    for(i=0;i<Cal_Point_Count;i++)  
    Clr_Bar(40 + i * (530-38)/10,440- i * (435-100)/10,100,20,COL_THINBLUE);
   
    
    Set_Chr_Color(COL_BLACK,COL_THINBLUE);
    Set_ASC_Lib(Arial12B_Font);  //����˸������·���ʾ�õ�ı궨��ֵ
    for(i=0;i<Cal_Point_Count;i++)  
     {
      Set_Graphic_Color(COL_DEEPRED,COL_THINBLUE);
      // Fill_Circle(38 + (Cal_Point_Count-1)*(550-38)/10,435-(Cal_Point_Count-1)*(435-100)/10,3);
      Fill_Circle(38 + i * (530-38)/10,435-i*(435-100)/10,3);
      Put_Data(  40 + i * (530-38)/10,440- i * (435-100)/10,"%d",Cal_Buf_Tmp[i]);//�����Ѵ�����
     }
   }
 }
/////////////////////////////////////////////////////////
//
//10��У׼�����ݴ���
/////////////////////////////////////////////////////////
void  point10_cal_proc(uint32 weight2)
  {
           uint8  eeprom_tx_buf[8];
   static uint32  last_cal_raw_data;
   ///////////////////////////
  if((Cal_Point_Count>0)&&(TRUE == stable_flag))
   {
     if(1 == Cal_Point_Count)
      {
       last_cal_raw_data = weight2;            //ȷ��0�� 
       Cal_Buf_Tmp[Cal_Point_Count-1] = 0;
       clr_data  = weight2;
       zer0_data = weight2;  //��ǰ������Ϊ���
       
       eeprom_tx_buf[0] = (uint8)(zer0_data>>24);
  	   eeprom_tx_buf[1] = (uint8)((zer0_data>>16)&0xff);
  	   eeprom_tx_buf[2] = (uint8)((zer0_data>>8)&0xff);
  	   eeprom_tx_buf[3] = (uint8)(zer0_data&0xff); 
  	 
  	   eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	   eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	   eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	   eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	   
  	   if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	   Write_EEPROM(EEP_WEIZERO_L2_ADDR, eeprom_tx_buf, 8);	 //��ͬ���ݼĴ���д��
  	   else
  	   Write_EEPROM(EEP_WEIZERO_L1_ADDR, eeprom_tx_buf, 8);	 //��ͬ���ݼĴ���д��
       
       Cal_Point_Count++;
       //go_0_flag = FALSE;
       stable_manual_break();
      }                                                //40000000/15/200000 = 13
     else if(weight2>(last_cal_raw_data + 1000000))    //��Լÿ���ֶ�����13������
      {
       last_cal_raw_data = weight2;    //net weigh code
       Cal_Buf_Tmp[Cal_Point_Count-1] = last_cal_raw_data - zer0_data;
       Cal_Point_Count++;
       //go_0_flag = TRUE;
       stable_manual_break();
      }
   }
  }
  /////////////////////////////////////////////////////////
//
//10��У׼�����ݴ���
/////////////////////////////////////////////////////////
void  point4_cal_proc(uint32 weight1)
  {
   uint8  eeprom_tx_buf[8];
   static uint32 last_cal_raw_data;
   uint32 x;
   x = abs(weight1-zer0_data);
   ///////////////////////////
   if(TRUE == stable_flag)
     {
      switch(Cal_Point_Count)
        {
         case 1: //ȷ��0��
                 Cal_Buf_Tmp[Cal_Point_Count-1] = 0;
                 last_cal_raw_data = 0;
                 clr_data = weight1;
                 zer0_data = clr_data;
                 eeprom_tx_buf[0] = (uint8)(clr_data>>24);
  	   			 eeprom_tx_buf[1] = (uint8)((clr_data>>16)&0xff);
  	   			 eeprom_tx_buf[2] = (uint8)((clr_data>>8)&0xff);
  	   			 eeprom_tx_buf[3] = (uint8)(clr_data&0xff); 
  	             eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	  			 eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	  			 eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	   			 eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	   			
  	   			 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
                 Write_EEPROM(EEP_WEIZERO_L2_ADDR, eeprom_tx_buf, 8);	 //��ͬ���ݼĴ���д��
  	   			 else
  	   			 Write_EEPROM(EEP_WEIZERO_L1_ADDR, eeprom_tx_buf, 8);	 //��ͬ���ݼĴ���д��
       			 
       			 Cal_Point_Count++;
      			 go_0_flag = FALSE;
      			 stable_manual_break();
      			 break;
         case 2://ȷ��50��
         case 3://ȷ��100��
         case 4://ȷ��150��
         case 5://ȷ��200��
                 if((x>last_cal_raw_data)&&((x-last_cal_raw_data) > 2000000*15/8))  
                  {
                   last_cal_raw_data = x;
                   Cal_Buf_Tmp[Cal_Point_Count-1] = x;
                   Cal_Point_Count++;
                   go_0_flag = TRUE;
                   stable_manual_break();
                   auto_zer0_valid = FALSE;
                  }
                 break;
         default:break;
        }
     }
  }