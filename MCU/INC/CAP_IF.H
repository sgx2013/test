/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : CAP.H
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:  0.1
* Date: 2011-11-24
* Author: Shao Guangxian
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
#ifndef _CAP_IF_H_
#define _CAP_IF_H_
#include "..\..\CONFIG\INC\TYPES.H"
#include "mb9bf404r.h"


/**** Definition of constants ****/


/**** Definition of types ****/
typedef enum
{
    CAP_UNIT_0 = 0x00,
    CAP_UNIT_1 = 0x01,
    CAP_UNIT_2 = 0x02,
    CAP_UNIT_3 = 0x3
} CAP_UNIT_TYPE;

typedef enum
{
    CAP_MODE_STOP = 0x00,
    CAP_MODE_RISING = 0x01,
    CAP_MODE_FALLING = 0x02,
    CAP_MODE_RISING_FALLING = 0x03
} CAP_MODE_TYPE;

/**** Definition of macros ****/
#define CAP_IT_ENABLE          0x10//for options
#define CAP_IN_FRT_UNIT0       0x00
#define CAP_IN_FRT_UNIT1       0x01
#define CAP_IN_FRT_UNIT2       0x02
#define CAP_EX_FRT_UNIT0       0x03
#define CAP_EX_FRT_UNIT1       0x04
#define CAP_MFT_TYPE           FM3_MFT_ICU_TypeDef
#define CAP_MFT_0              FM3_MFT0_ICU
#define CAP_MFT_1              FM3_MFT1_ICU

#define IS_CAP_MFT_TYPE(cap_mft)    ((cap_mft==CAP_MFT_0)||(cap_mft==CAP_MFT_1))

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void CAP_Timer_Sel( CAP_MFT_TYPE* cap, CAP_UNIT_TYPE cap_unit, U8 options );
extern void CAP_Config( CAP_MFT_TYPE* cap, CAP_UNIT_TYPE cap_unit, CAP_MODE_TYPE cap_mode );
extern U16  CAP_GetValue( CAP_MFT_TYPE* cap, CAP_UNIT_TYPE cap_unit );
extern void CAP_Handle_Process( void );
#endif /*_CAP_IF_H_*/
/**************** END OF FILE *****************************************/
