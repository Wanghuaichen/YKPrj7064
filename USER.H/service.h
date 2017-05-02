//**********************************************************************
//文件名:service.h
//**********************************************************************
#ifndef _SERVICE_H
#define _SERVICE_H

#include "type_def.h"

#define MENU_SERV_MAIN_SEL                   0x00       //主菜单
#define MENU_SERV_MAIN_OP_NUM                   8       //选项个数

#define MENU_SERV_SEL_SENSOR_OP        0x00000100       //AD数据 读取原始数据与滤波后的数值
#define MENU_SERV_SEL_TEMP_OP          0x00000200       //温度   读取温度数
#define MENU_SERV_SEL_EEPROM_OP        0x00000300       //EEPROM 读写测试
#define MENU_SERV_SEL_MODEL_OP         0x00000400       // 
#define MENU_SERV_SEL_MOTOR_OP         0x00000500       //马达   测试正反转
#define MENU_SERV_SEL_INTERFACE_OP     0x00000600       //测试   接口信号。如指示灯等
                      //程序运行指示灯
                      //无线信号指示灯
                      //无线输入信号
#define MENU_SERV_SEL_BRISPK_OP        0x00000700       //测试   输出连续的亮度变化
#define MENU_SERV_SEL_SCREEN_OP        0x00000800       //屏幕   颜色输出

extern void  Start_Service_process(void);
#endif