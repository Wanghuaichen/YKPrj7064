/****************************************************************************
* �ļ�����i2c_com.h
* ���ܣ�Ӳ��I2C��������(ͷ�ļ�)
* ˵����������Ҫ���ú�I2C���߽ӿ�(I2C���Ź��ܣ�����ʹ��I2C��ģʽ)
****************************************************************************/

#ifndef  I2C_COM_H
#define  I2C_COM_H

#include "type_def.h"

//SYS24C08     DEBUG USE
//USER24C08   ALWAYE USE							                                             
#define   SYS24C08   0xA0     //1010XP1P0X                        
#define   USER24C08  0xA8     //64PAGE *16BYTE = 1kb  

#define	ONE_BYTE_SUBA	1
#define TWO_BYTE_SUBA	2
#define X_ADD_8_SUBA	3

extern void Init_I2C(uint32 fi2c);
extern  uint8 I2C_Send_Byte( uint8 sla,  uint8 dat);
extern  uint8 I2C_Send_Str( uint8 sla,  uint8 suba,  uint8 *s,  uint8 no);
extern  uint8 I2C_Rcv_Byte( uint8 sla,  uint8 *dat);
extern  uint8 I2C_Rcv_Str( uint8 sla,  uint8 suba,  uint8 *s,  uint8 no);

#endif