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
#ifndef _GPIO_IF_H_
#define _GPIO_IF_H_
#include "..\..\CONFIG\INC\TYPES.H"

/**** Definition of constants ****/


/**** Definition of types ****/
typedef enum
{
    GPIO_PIN_0 = 0x00,
    GPIO_PIN_1 = 0x01,
    GPIO_PIN_2 = 0x02,
    GPIO_PIN_3 = 0x03,
    GPIO_PIN_4 = 0x04,
    GPIO_PIN_5 = 0x05,
    GPIO_PIN_6 = 0x06,
    GPIO_PIN_7 = 0x07,
    GPIO_PIN_8 = 0x08,
    GPIO_PIN_9 = 0x09,
    GPIO_PIN_A = 0x0A,
    GPIO_PIN_B = 0x0B,
    GPIO_PIN_C = 0x0C,
    GPIO_PIN_D = 0x0D,
    GPIO_PIN_E = 0x0E,
    GPIO_PIN_F = 0x0F
} GPIO_PIN_TYPE;

typedef enum
{
    GPIO_PORT_0 = 0x00,
    GPIO_PORT_1 = 0x01,
    GPIO_PORT_2 = 0x02,
    GPIO_PORT_3 = 0x03,
    GPIO_PORT_4 = 0x04,
    GPIO_PORT_5 = 0x05,
    GPIO_PORT_6 = 0x06,
    GPIO_PORT_7 = 0x07,
    GPIO_PORT_8 = 0x08
} GPIO_PORT_TYPE;

/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void GPIO_SetVal( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin );
extern void GPIO_ClrVal( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin );
extern U32  GPIO_GetVal( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin );
extern void GPIO_SetOutput( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin );
extern void GPIO_SetInput( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin );
#endif/*_GPIO_IF_H_*/
/****************************** END OF FILE ***************************/
