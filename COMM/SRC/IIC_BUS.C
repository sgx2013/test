/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :  Zhang Jiacheng           										   *
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

/**** System include files ****/
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"   

#include "..\..\SYSTEM\INC\TIMER_IF.H"


/*---Public include files---------------------------------------------*/
#include "..\INC\IIC_DRV_IF.H"
/*---Private include files--------------------------------------------*/
#include "..\INC\IIC_BUS.H"
#include "..\INC\IIC_BUS_IF.H"  


/*===VARIABLES========================================================*/

/*---Global-----------------------------------------------------------*/

/*---Private----------------------------------------------------------*/

/*===FUNCTIONS========================================================*/

/*---Global-----------------------------------------------------------*/
/*******************************************************************************
**  FUNCTION      : iic_bus_create_eeprom                                         
**  DESCRIPTION   : This function reset the eeprom channel
**  PARAMETERS    : void
**  RETURN        : void                                                          
*******************************************************************************/
void iic_bus_create_eeprom(void)
{
    iic_drv_down(IIC_CHANNEL_EEPROM);
    tm_delay_100us();
    iic_drv_create(IIC_CHANNEL_EEPROM,IIC_BAUDRATE_EEPROM,IIC_ADDRESS_EEPPROM);    
}

/*******************************************************************************
**  FUNCTION      : iic_bus_create_tuner                                         
**  DESCRIPTION   : This function make the iic channels enable
**  PARAMETERS    : void
**  RETURN        : void                                                          
*******************************************************************************/
void iic_bus_create_tuner(void)
{
    iic_drv_create(IIC_CHANNEL_TUNER,IIC_BAUDRATE_TUNER,IIC_ADDRESS_TUNER);    
}

/*******************************************************************************
**  FUNCTION      : iic_bus_create_dsp                                         
**  DESCRIPTION   : This function make the iic channels enable
**  PARAMETERS    : void
**  RETURN        : void                                                          
*******************************************************************************/

void iic_bus_create_dsp(void)
{
    iic_drv_create(IIC_CHANNEL_DSP,IIC_BAUDRATE_DSP,IIC_ADDRESS_DSP);      
}

/*******************************************************************************
**  FUNCTION      : iic_bus_create_pa                                         
**  DESCRIPTION   : This function make the iic channels enable
**  PARAMETERS    : void
**  RETURN        : void                                                          
*******************************************************************************/

void iic_bus_create_pa(void)
{
    iic_drv_create(IIC_CHANNEL_PA,IIC_BAUDRATE_PA,IIC_ADDRESS_PA);      
}

/*******************************************************************************
**  FUNCTION      : iic_bus_send                                         
**  DESCRIPTION   : This function send iic message
**  PARAMETERS    : slave_address
**                  iic_data
**                  length
**                  channel
**                  options
**  RETURN        : BOOL                                                          
*******************************************************************************/
BOOL iic_bus_send(U8 slave_address, U8 *iic_data, U8 length,  U8 channel, U8 options)
{
    return iic_drv_send(slave_address, iic_data, length, channel, options);
}


/*******************************************************************************
**  FUNCTION      : iic_bus_read                                         
**  DESCRIPTION   : This function send iic message
**  PARAMETERS    : slave_address
**                  iic_data
**                  length
**                  channel
**                  options
**  RETURN        : BOOL                                                          
*******************************************************************************/
BOOL iic_bus_read(U8 slave_address, U8 *iic_data, U8 length,  U8 channel, U8 options)
{
    return iic_drv_read(slave_address, iic_data, length, channel, options); 
}

/*******************************************************************************
**  FUNCTION      : iic_bus_create                                         
**  DESCRIPTION   : This function make the iic channels enable
**  PARAMETERS    : void
**  RETURN        : void                                                          
*******************************************************************************/
void iic_bus_down(void)
{
    iic_drv_down(IIC_CHANNEL_TUNER);
    iic_drv_down(IIC_CHANNEL_DSP);
}


/*---Private----------------------------------------------------------*/
