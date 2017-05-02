#ifndef  _GP21_H
#define  _GP21_H

#include  "type_def.h"
/**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/
#define IO_GP21_SET        FIO1SET
#define IO_GP21_CLR        FIO1CLR
#define IO_GP21_PIN        FIO1PIN
#define IO_GP21_DIR        FIO1DIR

#define GP21_INT         (1<<12)            /* P2.12    */

#define GP21_SSN         (1<<9)             /* P1.9  pin188  */
#define GP21_SCLK        (1<<8)             /* P1.8  pin190  */
#define GP21_RST         (1<<4)             /* P1.4  pin192  */
#define GP21_DOUT        (1<<1)             /* P1.1  pin194  */
#define GP21_DIN         (1<<0)             /* P1.0  pin196  */

#endif