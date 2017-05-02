#ifndef  _EPM7064S_H
#define  _EPM7064S_H

#include "type_def.h"

/**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/

#define IO_EPM7_SET        FIO0SET
#define IO_EPM7_CLR        FIO0CLR
#define IO_EPM7_PIN        FIO0PIN
#define IO_EPM7_DIR        FIO0DIR

#define EPM7_INT          (1<<12)            /* P2.12  INT2  */
//此中断引脚的初始化在Init_Port函数里由系统统一配置

#define EPM7_DATA_LOW_EN  (1<<5)             /* P0.5    */
#define EPM7_DATA_HIG_EN  (1<<4)             /* P0.4    */

#define EPM7_BUS_DATA0    (1<<10)             /* P1.10    */
#define EPM7_BUS_DATA1    (1<<11)             /* P1.11    */
#define EPM7_BUS_DATA2    (1<<12)             /* P1.12    */
#define EPM7_BUS_DATA3    (1<<13)             /* P1.13    */
#define EPM7_BUS_DATA4    (1<<14)             /* P1.14    */
#define EPM7_BUS_DATA5    (1<<15)             /* P1.15    */
#define EPM7_BUS_DATA6    (1<<16)             /* P1.16    */
#define EPM7_BUS_DATA7    (1<<17)             /* P1.17    */

//////////////////////////////////MOTOR 
//此位无信号

#define EPM7_MOTOR_P1      (1<<15)            /* P0.15    */
#define EPM7_MOTOR_P2      (1<<16)            /* P0.16    */
#define EPM7_MOTOR_POSI    (1<<1)             /* P2.1     */

#define MOTOR_RUN       FIO0CLR = EPM7_MOTOR_P1;FIO0SET = EPM7_MOTOR_P2;          
#define MOTOR_STOP      FIO0CLR = EPM7_MOTOR_P1;FIO0CLR = EPM7_MOTOR_P2;

//#define SET_MOTOR_UNLOAD  FIO0CLR=EPM7_MOTOR_P1;FIO0SET=EPM7_MOTOR_P2          
//#define SET_MOTOR_LOAD    FIO0CLR=EPM7_MOTOR_P2;FIO0SET=EPM7_MOTOR_P1
//#define  STOP_MOTOR       FIO0CLR=EPM7_MOTOR_P1;FIO0CLR=EPM7_MOTOR_P2
           
#define  MOTOR_POSITION   (((FIO2PIN&EPM7_MOTOR_POSI)>>1)&0x01) 
//光耦信号，来自于加卸载状态
#define  LOAD_POSITION                                0 
#define  UNLOAD_POSITION                              1 

void   Epm7064sPIN_Init(void);
uint32 Read_Epm7064_Low(void);
uint32 Read_Epm7064_High(void);
void   Epm7_Data_Pro(uint32 raw_data);  

#endif