/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : I2C.H
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:  0.1
* Date: 2011-12-20
* Author: Shao Guangxian
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
#ifndef _I2C_IF_H_
#define _I2C_IF_H_
#include "..\..\CONFIG\INC\TYPES.H"
#include "mb9bf404r.h"

/**** Definition of constants ****/


/**** Definition of types ****/


/**** Definition of macros ****/
#define  I2C_TYPE       FM3_MFS47_I2C_TypeDef
#define  I2C_0          ( I2C_TYPE * )FM3_MFS0_I2C
#define  I2C_1          ( I2C_TYPE * )FM3_MFS1_I2C
#define  I2C_2          ( I2C_TYPE * )FM3_MFS2_I2C
#define  I2C_3          ( I2C_TYPE * )FM3_MFS3_I2C
#define  I2C_4          ( I2C_TYPE * )FM3_MFS4_I2C
#define  I2C_5          ( I2C_TYPE * )FM3_MFS5_I2C
#define  I2C_6          ( I2C_TYPE * )FM3_MFS6_I2C
#define  I2C_7          ( I2C_TYPE * )FM3_MFS7_I2C
#define  I2C_STOP       0x0
#define  I2C_NOSTOP     0x10
#define  I2C_NOSTART    0x20

#define  I2C_BUS_BUSY_FLAG  0x00
#define  I2C_BUS_NACK_FLAG  0x01

#define  IS_I2C_TYPE(i2c)       ((i2c==I2C_0)||(i2c==I2C_1)||(i2c==I2C_2)||(i2c==I2C_3)||\
                                 (i2c==I2C_4)||(i2c==I2C_5)||(i2c==I2C_6)||(i2c==I2C_7))
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


/***********end extern micro/type define ****************/
extern void I2C_Init( I2C_TYPE* i2c, U32 baudrate, U8 Addr );
extern void I2C_WriteFrame( I2C_TYPE* i2c, U8 Addr, U8* pstr, U8 size, U8 options );
extern void I2C_ReadFrame( I2C_TYPE* i2c, U8 Addr, U8* pstr, U8 size, U8 options );
extern void I2C_Slave_SendChar( I2C_TYPE* i2c, U8 chr );
extern void I2C_Slave_RecvChar( I2C_TYPE* i2c, U8* chr );
extern void I2C_Disable( I2C_TYPE* i2c );
//extern BOOL I2C_Is_Busy( I2C_TYPE* i2c );
extern BOOL I2C_Get_Flag( I2C_TYPE* i2c,U8 flag );
extern void I2C_AlterRxSize( I2C_TYPE* i2c, U8 size );
extern void I2C_Slave_SendChar( I2C_TYPE* i2c, U8 chr );
extern void I2C_Slave_RecvChar( I2C_TYPE* i2c, U8* chr );
void I2C_Handler( I2C_TYPE* i2c );
#endif /*_I2C_IF_H_*/
/****************************** END OF FILE ***************************/
