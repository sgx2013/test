/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
#ifndef _EXTI_IF_H_
#define _EXTI_IF_H_
#include "..\..\CONFIG\INC\TYPES.H"

/**** Definition of constants ****/


/**** Definition of types ****/
typedef enum
{
    EXTI_00 = 0x00,
    EXTI_01 = 0x01,
    EXTI_02 = 0x02,
    EXTI_03 = 0x03,
    EXTI_04 = 0x04,
    EXTI_05 = 0x05,
    EXTI_06 = 0x06,
    EXTI_07 = 0x07,
    EXTI_08 = 0x08,
    EXTI_09 = 0x09,
    EXTI_10 = 0x0A,
    EXTI_11 = 0x0B,
    EXTI_12 = 0x0C,
    EXTI_13 = 0x0D,
    EXTI_14 = 0x0E,
    EXTI_15 = 0x0F
} EXTI_TYPE;

typedef enum
{
    EXTI_TRIGGER_LOW = 0x00,
    EXTI_TRIGGER_HIGH = 0x01,
    EXTI_TRIGGER_RISING = 0x02,
    EXTI_TRIGGER_FALLING = 0x03
} EXTI_TRIGGER_TYPE;

/**** Definition of macros ****/
#define IS_EXTI_TYPE(exti_x)        ((exti_x==EXTI_00)||(exti_x==EXTI_01)||(exti_x==EXTI_02)||(exti_x==EXTI_03)||\
                                     (exti_x==EXTI_04)||(exti_x==EXTI_05)||(exti_x==EXTI_06)||(exti_x==EXTI_07)||\
                                     (exti_x==EXTI_08)||(exti_x==EXTI_09)||(exti_x==EXTI_10)||(exti_x==EXTI_11)||\
                                     (exti_x==EXTI_12)||(exti_x==EXTI_13)||(exti_x==EXTI_14)||(exti_x==EXTI_15))

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/



extern void EXTI_Config( EXTI_TYPE exti_x, EXTI_TRIGGER_TYPE exti_trigger );
extern void EXTI_Cmd( EXTI_TYPE exti_x, FUNCTION_STATE state );
/*below just for IRQ only*/
void EXTI_Handler4( void );
void EXTI_Handler5( void );
#endif /* _EXTI_IF_H_ */
/****************************** END OF FILE ***************************/

