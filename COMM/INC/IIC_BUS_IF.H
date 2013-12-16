/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Zhang Jiacheng										   *
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
#ifndef _IIC_BUS_IF_H_
#define _IIC_BUS_IF_H_ 
#include "..\..\HAL\INC\I2C_HAL.H"
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define IIC_CHANNEL_EEPROM  I2C_HAL_EEPROM
#define IIC_CHANNEL_TUNER   I2C_HAL_TUNER
#define IIC_CHANNEL_DSP     I2C_HAL_DSP
#define IIC_CHANNEL_PA		I2C_HAL_PA

#define IIC_BAUDRATE_NAVI    200000
#define IIC_BAUDRATE_EEPROM  300000
#define IIC_BAUDRATE_TUNER   200000
#define IIC_BAUDRATE_DSP     300000
#define IIC_BAUDRATE_PA      300000

#define IIC_ADDRESS_NAVI     0xC0
#define IIC_ADDRESS_EEPPROM  0xAE
#define IIC_ADDRESS_TUNER    0xC8
#define IIC_ADDRESS_DSP      0x30
#define IIC_ADDRESS_PA     	 0xD8


#define IIC_NO_STOPC              I2C_NO_STOPC
#define IIC_NO_STARTC             I2C_NO_STARTC



/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void iic_bus_create_eeprom(void);
extern void iic_bus_create(void);
extern void iic_bus_create_tuner(void);
extern void iic_bus_create_dsp(void);
extern void iic_bus_create_pa(void);
extern BOOL iic_bus_send(U8 slave_address, U8 *iic_data, U8 length, U8 channel, U8 options);
extern BOOL iic_bus_read(U8 slave_address, U8 *iic_data, U8 length, U8 channel, U8 options);
extern void iic_bus_down(void);




#endif /*_IIC_BUS_IF_H_*/

/****************************** END OF FILE ***************************/

