/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : FRT.H
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
#ifndef _FRT_IF_H_
#define _FRT_IF_H_
#include "..\..\CONFIG\INC\TYPES.H"
#include "mb9bf404r.h"

/**** Definition of constants ****/


/**** Definition of types ****/
typedef enum
{
    FRT_UNIT_0 = 0x00,
    FRT_UNIT_1 = 0x01,
    FRT_UNIT_2 = 0x02
} FRT_UNIT_TYPE;

typedef enum
{
    FRT_STATE_START = 0x00,
    FRT_STATE_STOP = 0x01
} FRT_STATE_TYPE;

/**** Definition of macros ****/
#define FRT_MFT_TYPE    FM3_MFT_FRT_TypeDef
#define FRT_MFT_0       FM3_MFT0_FRT
#define FRT_MFT_1       FM3_MFT1_FRT
#define FRT_PCLK_DIV1    0x0000
#define FRT_PCLK_DIV2    0x0001
#define FRT_PCLK_DIV4    0x0002
#define FRT_PCLK_DIV8    0x0003
#define FRT_PCLK_DIV16   0x0004
#define FRT_PCLK_DIV32   0x0005
#define FRT_PCLK_DIV64   0x0006
#define FRT_PCLK_DIV128  0x0007
#define FRT_PCLK_DIV256  0x0008
#define IS_FRT_MFT_TYPE(ftr)        ((frt==FRT_MFT_0)||(frt==FRT_MFT_1))
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


/********* end extern micro/type define ***************/
extern void FRT_Init( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, U16 options );
extern void FRT_SetPeriod( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, U16 period );
extern void FRT_Cmd( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, FRT_STATE_TYPE state );
extern void FRT_Isr_Cmd( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, FUNCTION_STATE state );
void FRT_Clr_Flag( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit ); //JUST FOR CLR IRQ FLAG
void FRT_Handler( void ); //for IRQ only

#endif /*_FRT_IF_H_*/

/****************************** END OF FILE ***************************/

