//**********************************************************************
//ÎÄ¼þÃû:ad_acquire.h
//**********************************************************************
#ifndef _AD_ACQUIRE_H_
#define _AD_ACQUIRE_H_

#include "type_def.h"

uint32 Get_Raw(void);

void Init_Timer1(void);
void Init_EINT2(void);
void FIQ_Exception(void);
#endif