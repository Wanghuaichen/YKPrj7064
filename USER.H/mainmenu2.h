#ifndef _MAINMENU2_H
#define _MAINMENU2_H

#include  "type_def.h"

//machine cfg
extern void    MoreCfgInput_Interface(sint32 cur_menu_op);
extern sint32  MoreCfg_Key_Pro(uint32 key_c,sint32 op);
extern void    MoreCfg_Disp_Pro(void);
//bright
extern void    Speak_Adjust_Interface(sint32 cur_menu_op);
extern sint32  Speak_Adjust_Key_Pro(uint32 key_c,sint32 op);
extern void    Speak_Adjust_Disp_Pro(void);
//bright
extern void    Brig_Adjust_Interface(sint32 cur_menu_op);
extern sint32  Brig_Adjust_Key_Pro(uint32 key_c,sint32 op);
extern void    Brig_Adjust_Disp_Pro(void);
//数据回显示
extern void    Data_Call_Interface(sint32 cur_menu_op);
extern sint32  Data_Call_Key_Pro(uint32 key_c,sint32 op);
extern void    Data_Call_Disp_Pro(void);

//wireless config
extern void    WirelessCfgInput_Interface(sint32 cur_menu_op);
extern sint32  WirelessCfg_Key_Pro(uint32 key_c,sint32 op);
extern void    WirelessCfg_Disp_Pro(void);
//SCREEN CAL
extern void    ScrCalInput_Interface(sint32 cur_menu_op);
extern sint32  ScrCal_Key_Pro(uint32 key_c,sint32 op);
extern void    ScrCal_Disp_Pro(void);
//mode-select
extern void    ModeSel_Interface(sint32 cur_menu_op);
extern sint32  ModeSel_Key_Pro(uint32 key_c,sint32 op);
extern void    ModeSel_Disp_Pro(void);
//language
extern void    LangSel_Interface(sint32 cur_menu_op);
extern sint32  LangSel_Key_Pro(uint32 key_c,sint32 op);
extern void    LangSel_Disp_Pro(void);
//numinput
extern void    NumberInput_Interface(sint32 cur_menu_op);
extern sint32  NumInput_Key_Pro(uint32 key_c,sint32 op);
extern void    NumberInput_Disp_Pro(void);
//PCSinput
extern void    PCSInput_Interface(sint32 cur_menu_op);
extern sint32  PCSInput_Key_Pro(uint32 key_c,sint32 op);
extern void    PCSInput_Disp_Pro(void);

#endif