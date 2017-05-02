//**********************************************************************
//文件名:normal_opration.h
//**********************************************************************
#ifndef    _NORMAL_OPERATION_H
#define    _NORMAL_OPERATION_H

#include "type_def.h"

#define MENU_NUM                    22        //菜单个数

//MAIN OPER
#define MENU_OP_OPER                  0x00000000    //主工作菜单
#define MENU_SLEEP_OP_OPER            0x00000100    //sleepmode
//date cfg
#define MENU_DATE_CFG_OPER            0x00000200    //时间日期设置
//time cfg 把此菜单更改为 双量程选择
#define MENU_RANGE_SEL_OPER           0x00000300    //时间设置
//mainmenu cfg
#define MENU_UNIT_SEL_OPER            0x00000400    //UNIT
//ASD cfg
#define MENU_ASD_SEL_OPER             0x00000500    //ASD
//LNT cfg  
#define MENU_LNT_SEL_OPER             0x00000600    //LNT
//PRINT
#define MENU_PRT_SEL_OPER             0x00000700    //PRT
//mode_sel
#define MENU_MODE_SEL_OPER            0x00000800    //MODE-SEL
//language_sel
#define MENU_LANG_SEL_OPER            0x00000900    //LANGUAGE-SEL
//
#define MENU_CAL_10POINT_OPER         0x00000a00    //CAL

#define MENU_NUM_INPUT_OPER           0x00000b00    //weigh_per_pcs
#define MENU_PCS_INPUT_OPER           0x00000c00    //pcs numberinput
#define MENU_MORE_CFG_OPER            0x00000d00    //
#define MENU_WIRELESS_CFG_OPER        0x00000e00
#define MENU_SCR_CAL_OPER             0x00000f00
#define MENU_BRIG_CFG_OPER            0x00001000
#define MENU_CALL_DATA_OPER           0x00001100
#define MENU_SPEAK_CFG_OPER           0x00001200
//ADDED
#define MENU_PASSW_CFG_OPER           0x00001300
#define MENU_OPERID_CFG_OPER          0x00001400
#define MENU_FACTORY_CFG_OPER         0x00001500
//
//#define MENU_CAL_10POINT_OPER         0x00000800    //CAL
/////////////////////////////////////////////////

#define   MAIN_BUT_WIDTH        100
#define   MAIN_BUT_HEIGTH       100
#define   DATE_TIME_WIDTH       160
#define   DATE_TIME_HEIGTH       30

#define   DATE_CFG_WIDTH1        80
#define   DATE_CFG_HEIGTH1       40
#define   DATE_CFG_WIDTH2        80
#define   DATE_CFG_HEIGTH2       60

extern   void      Normal_Operation(void);    
extern   void      Set_Menu_Op(sint32 cur_menu_op);    
//extern   uint32    Get_Line_data(uint32 weight2); 
                                                                                 
#endif                                                                                                                                                            