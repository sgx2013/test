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
#ifndef _I2C_H_
#define _I2C_H_
#include "..\..\CONFIG\INC\TYPES.H"
/**** Definition of constants ****/


/**** Definition of types ****/
typedef struct
{
    int I2C_IBCR_INT;
    int I2C_IBCR_BER;
    int I2C_IBCR_INTE;
    int I2C_IBCR_CNDE;
    int I2C_IBCR_WSEL;
    int I2C_IBCR_ACKE;
    int I2C_IBCR_SCC;
    int I2C_IBCR_MSS;
} I2C_IBCR_TYPE;

typedef struct
{
    U8 * message;
    U8 size;
    U8 options;
    U8 userflag;
} I2C_FRAME_TYPE;

/**** Definition of macros ****/
#define I2C_ISMK_I2C_EN             0x80
#define I2C_ISBA_SAEN               0x80
#define I2C_IBCR_BITBAND_BASE       0x42700020UL
#define I2C_BUS_BUSY_BIT            0x01
#define I2C_RX_NOFIRST_CHAR		    0x02
#define I2C_BUS_NACK_BIT            0x04
#define I2C_BUS_NOADDR_CHAR         0x08
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/




#endif /*_I2C_H_*/
/****************************** END OF FILE ***************************/
