//**********************************************************************
//文件名:eeprom.h
//**********************************************************************
#ifndef _EEPROM_H
#define _EEPROM_H
#include "type_def.h"

#define Macro_En_EEPROM()             (FIO3CLR = P3_WP)
#define Macro_Dis_EEPROM()            (FIO3SET = P3_WP)

//保存零点 和满量程数值 万分之一保存地址
//clr_data 线性校准零点
#define    EEP_WEIZERO_L1_ADDR            0x00    //线性零点
#define    EEP_WEIZERO_L1_ADDR_BAK        0x04    //备份
//zer0_data 用户校准零点
#define    EEP_WEIZERO_U1_ADDR            0x08    //用户零点
#define    EEP_WEIZERO_U1_ADDR_BAK        0x0C    //备份

#define    EEP_WEIFULL1_ADDR              0x10    //满量程内码数
#define    EEP_WEIFULL1_ADDR_BAK          0x14    //备份

#define    EEP_USERINNCAL1_ADDR           0x18    //存放用户内校内码数


//保存 线性校准数值
#define    EEP_LINCAL1_ADDR01           0x20    //零点
#define    EEP_LINCAL1_ADDR02           0x24    //1/10
#define    EEP_LINCAL1_ADDR03           0x28    //2/10
#define    EEP_LINCAL1_ADDR04           0x2C    //3/10
#define    EEP_LINCAL1_ADDR05           0x30    //4/10
#define    EEP_LINCAL1_ADDR06           0x34    //5/10
#define    EEP_LINCAL1_ADDR07           0x38    //6/10
#define    EEP_LINCAL1_ADDR08           0x3C    //7/10
#define    EEP_LINCAL1_ADDR09           0x40    //8/10
#define    EEP_LINCAL1_ADDR10           0x44    //9/10
#define    EEP_LINCAL1_ADDR11           0x48    //满量程
#define    EEP_LINCAL1_CHECK            0x4C    //
//////////////////////////////////////////////////////////////////////////////////
////保存零点 和满量程数值 十万分之一保存地址
//////////////////////////////////////////////////////////////////////////////////
//clr_data 线性校准零点
#define    EEP_WEIZERO_L2_ADDR            0xA0    //线性零点
#define    EEP_WEIZERO_L2_ADDR_BAK        0xA4    //备份
//zer0_data 用户校准零点
#define    EEP_WEIZERO_U2_ADDR            0xA8    //用户零点
#define    EEP_WEIZERO_U2_ADDR_BAK        0xAC    //备份

#define    EEP_WEIFULL2_ADDR              0xB0    //满量程内码数
#define    EEP_WEIFULL2_ADDR_BAK          0xB4    //备份
#define    EEP_USERINNCAL2_ADDR           0xB8    //存放用户内校内码数

//保存 线性校准数值
#define    EEP_LINCAL2_ADDR01             0xC0    //零点
#define    EEP_LINCAL2_ADDR02             0xC4    //1/10
#define    EEP_LINCAL2_ADDR03             0xC8    //2/10
#define    EEP_LINCAL2_ADDR04             0xCC    //3/10
#define    EEP_LINCAL2_ADDR05             0xD0    //4/10
#define    EEP_LINCAL2_ADDR06             0xD4    //5/10
#define    EEP_LINCAL2_ADDR07             0xD8    //6/10
#define    EEP_LINCAL2_ADDR08             0xDC    //7/10
#define    EEP_LINCAL2_ADDR09             0xE0    //8/10
#define    EEP_LINCAL2_ADDR10             0xE4    //9/10
#define    EEP_LINCAL2_ADDR11             0xE8    //满量程
#define    EEP_LINCAL2_CHECK              0xEC    //


//保存 系统参数
//保存 屏幕校准数值
#define   EEP_SCREEN_ADDR              0x60 //三个坐标点数据各占4个字节+校验     = 13个字节
//1:语言设置 2亮度 3无线配置
#define   EEP_LANGUAGE_ADDR            0x70
#define   EEP_BRIGHT_ADDR              0x74
#define   EEP_WIRELESS_ADDR            0x78
#define   EEP_SPEAK_ADDR               0x7c
#define   EEP_PASSWORD_ADDR            0x80
#define   EEP_USER_ID_ADDR             0x84
#define   EEP_MODEL_ID_ADDR            0x88
//////////////////////////////////////////////////////
#define   EEP_INNER_CAL1_ADDR          0x50        //内校准保存真实的砝码重量
#define   EEP_INNER_CAL2_ADDR          0x58        //暂时没有使用 2016.06.12


//定义测试地址
#define   EEP_TEST_ADDR                0xF0        //EEPROM读写测试字节
#define   EEP_FACT_PARA_ADDR           0xF8        //该字节后续存放了工厂设置参数开关选项
//以下信息用于存储用户测量数据
//AT24C01 128Bytes   AT24C08 1024Bytes   AT24C64   8192Bytes   
//AT24C02 256Bytes   AT24C16 2048Bytes   AT24C128 16384Bytes
//AT24C04 512Bytes   AT24C32 4096Bytes   AT24C256 32768Bytes
//计算公式 100*16=1600BYTES 至少使用24C16 页写缓冲最大为16BYTE
//如果使用24C32或以上 页写缓存变大，可以考虑修改程序

#define    EEP_BYTE_PER_REC           0x10     //用户每条记录占用字节数
#define    REC_MAX_NUM                 100     //最大存储记录条数
#define    RECNUM_PER_PAGE              10     //每页有10条记录

#define    EEP_REC_TOTAL_ADDR        0x100     //存放记录总数
#define    EEP_REC_TOTAL_ADDR_BAK    0x104     //记录总数备份   
#define    EEP_REC_START_ADDR        0x110     //存放用户记录开始地址








///////////////////////////////////////////////////////////////////
 uint8 Read_EEPROM(uint16 addr,  uint8 *rptr, uint16 rlen);
 uint8 Read_EEPROM_1B(uint16 addr,  uint8 *r1ptr);
 uint8 Read_EEPROM_2B(uint16 addr, uint16 *r2ptr);
 uint8 Read_EEPROM_4B(uint16 addr, uint32 *r4ptr);
 uint8 Write_EEPROM(uint16 addr,  uint8 *wptr, uint16 wlen);
 uint8 Write_EEPROM_1B(uint16 addr,  uint8 w1b);
 uint8 Write_EEPROM_2B(uint16 addr, uint16 w2b);
 uint8 Write_EEPROM_4B(uint16 addr, uint32 w4b);
 uint8 Check_EEP_Wr_ERR(void);
 uint8 EEPROM_RW_Test(void);
#endif