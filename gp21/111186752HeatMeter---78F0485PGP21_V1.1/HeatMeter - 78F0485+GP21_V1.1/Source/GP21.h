//-----------------------------------------------------------------------------
// Copyright SECOM Telecom Co., LTD
//
// Target:         uPD78F0485
// Tool chain:     NEC PM+ v6.30
// Author:		   Gan zhiyong
// Date:		   2011-5-8
// Vervion:	       V1.1
//-----------------------------------------------------------------------------

#ifndef	_GP21_H
#define _GP21_H


//-----------------------------------------------------------------------------
// Function prototyps
//-----------------------------------------------------------------------------
extern void GP21_Init(void);
extern signed long GP21_GetDeltaT(unsigned char *pValidFlag);
extern signed long GP21_GetDeltaT_StartTOFRestart(unsigned char *pValidFlag);




#endif	//_GP21_H
