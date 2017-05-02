/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: target.c
**创   建   人: 陈明计
**最后修改日期: 2004年2月2日
**描        述: lpc21xx（飞利浦的ARM）目标板特殊的代码，包括异常处理程序和目标板初始化程序
**              每个工程应当具有这个文件的拷贝，用户根据程序的需要修改本文件。
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: v1.0
** 日　期: 2004年2月2日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人:
** 版  本:
** 日　期:
** 描　述:
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#define IN_TARGET
#include "config.h"

/*********************************************************************************************************
** 函数名称: IRQ_Exception
** 功能描述: 中断异常处理程序，用户根据需要自己改变程序
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void __irq IRQ_Exception(void)
{
    while(1);                   // 这一句替换为自己的代码
}

/*********************************************************************************************************
** 函数名称: FIQ_Exception
** 功能描述: 快速中断异常处理程序，用户根据需要自己改变程序
**           
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void FIQ_Exception(void)
{
    while(1);                   // 这一句替换为自己的代码
}

/*********************************************************************************************************
** 函数名称: TargetInit
** 功能描述: 目标板初始化代码，在需要的地方调用，根据需要改变
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
       void TargetInit(void)
{
    /* 添加自己的代码 */
}

/*********************************************************************************************************
** 函数名称: TargetResetInit
** 功能描述: 调用main函数前目标板初始化代码，根据需要改变，不能删除
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void TargetResetInit(void)
{
#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;                   //remap
#endif

#ifdef __DEBUG_FLASH    
    MEMMAP = 0x1;                   //remap
#endif

#ifdef __IN_CHIP    
    MEMMAP = 0x1;                   //remap
#endif

    /* 设置系统各部分时钟 */
    PLLCON = 1;
#if (Fpclk / (Fcclk / 4)) == 1
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    VPBDIV = 1;
#endif

#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0);
    PLLCON = 3;
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    
    /* 设置存储器加速模块 */
    MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
#if Fcclk < 40000000
    MAMTIM = 2;
#else
    MAMTIM = 3;
#endif
#endif
    MAMCR = 2;
    
    /* 初始化VIC */
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* 添加自己的代码 */

}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
