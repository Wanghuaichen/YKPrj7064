#ifndef _MAINMENU_H
#define _MAINMENU_H

#include  "type_def.h"

extern void     UnitSel_Interface(sint32 cur_menu_op);
extern sint32   UnitSel_Key_Pro(uint32 key_c,sint32 op);
extern void     UnitSel_Disp_Pro(void);
//ASD
extern void     ASDSel_Interface(sint32 cur_menu_op);
extern sint32   ASDSel_Key_Pro(uint32 key_c,sint32 op);
extern void     ASDSel_Disp_Pro(void);
//PRT
extern void     PRTSel_Interface(sint32 cur_menu_op);
extern sint32   PRTSel_Key_Pro(uint32 key_c,sint32 op);
extern void     PRTSel_Disp_Pro(void);

//LNT
extern void     LNTSel_Interface(sint32 cur_menu_op);
extern sint32   LNTSel_Key_Pro(uint32 key_c,sint32 op);
extern void     LNTSel_Disp_Pro(void);
/////////////////////////////////////////////////////

extern void     RangeSel_Interface(sint32 cur_menu_op);
extern sint32   RangeSel_Key_Pro(uint32 key_c,sint32 op);
extern void     RangeSel_Disp_Pro(void);
//
extern void     Password_Adjust_Interface(sint32 cur_menu_op);
extern sint32   PasswordCfg_Key_Pro(uint32 key_c,sint32 op);
extern void     PasswordCfg_Disp_Pro(void);
//
extern void     OperID_Adjust_Interface(sint32 cur_menu_op);
extern sint32   OperIDCfg_Key_Pro(uint32 key_c,sint32 op);
extern void     OperIDCfg_Disp_Pro(void);
//
extern void     Factory_Restore_Interface(sint32 cur_menu_op);
extern sint32   FactoryCfg_Key_Pro(uint32 key_c,sint32 op);
extern void     FactoryCfg_Disp_Pro(void);




//CAL-10POINT
extern void     Cal_10point_Interface(sint32 cur_menu_op);
extern sint32   Cal_10point_Key_Pro(uint32 key_c,sint32 op);
extern void     Cal_10point_Disp_Pro(void);
extern void     point10_cal_proc(uint32 weight2);
extern void     point4_cal_proc(uint32 weight2);
#endif