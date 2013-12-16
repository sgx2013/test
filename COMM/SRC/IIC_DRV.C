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
#include <RTL.H>
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"   
#include "..\..\OS\INC\TASK_IF.H"   

/*---Public include files---------------------------------------------*/
#include "..\..\HAL\INC\I2C_HAL.H"
/*---Private include files--------------------------------------------*/
#include "..\INC\IIC_DRV.H"
#include "..\INC\IIC_DRV_IF.H"


/*===VARIABLES========================================================*/

/*---Global-----------------------------------------------------------*/

/*---Private----------------------------------------------------------*/

/*===FUNCTIONS========================================================*/

/*---Global-----------------------------------------------------------*/
/*******************************************************************************
**  FUNCTION      : iic_drv_create                                         
**  DESCRIPTION   : This function init iic
**  PARAMETERS    : channel
**                  baudrate
**                  addr  
**  RETURN        : void                                                          
*******************************************************************************/
void iic_drv_create(U8 channel, U32 baudrate, U8 addr)
{
    I2C_HAL_Create(channel, baudrate, addr);    
}


BOOL iic_f[8] = {0};
/*******************************************************************************
**  FUNCTION      : iic_drv_send                                         
**  DESCRIPTION   : This function send iic message
**  PARAMETERS    : slave_address
**                  iic_data
**                  length
**                  channel
**                  options
**  RETURN        : BOOL                                                          
*******************************************************************************/
BOOL iic_drv_send(U8 slave_address, U8 *iic_data, U8 length, U8 channel, U8 options)
{
    U8 iic_wait_counter = 0x00;
    U8 iic_retry_counter = 0x00;

    do
    {
        if(!I2c_HAL_Is_Busy(channel))
        {
            os_sem_init(i2c_sem_wait[channel],0);
            I2C_HAL_Send(slave_address, iic_data, length, channel, options);
            do
            {   
               os_sem_wait(i2c_sem_wait[channel],2 OS_MSECOND);//os_dly_wait(1); //2ms 
               tm_delay_us(10);//just fix i2c stop timing here
                if(iic_wait_counter++ >= 50) //50*2 = 100ms
                {
                    return FALSE;
                }
            }while(I2c_HAL_Is_Busy(channel));
			if(I2c_HAL_Is_Nack(channel))
            {
               return FALSE;
            }

		}
        else
        {
            os_dly_wait(4 OS_MSECOND);//os_dly_wait(1); //2ms
            
            if(iic_retry_counter++ >= 50)    //50*2 = 100ms
            {
                return FALSE;
            }
        }
    }while(I2c_HAL_Is_Busy(channel));

    return TRUE;
}

/*******************************************************************************
**  FUNCTION      : iic_drv_read                                         
**  DESCRIPTION   : This function read iic message
**  PARAMETERS    : slave_address
**                  iic_data
**                  length
**                  channel
**                  options
**  RETURN        : BOOL                                                          
*******************************************************************************/
BOOL iic_drv_read(U8 slave_address, U8 *iic_data, U8 length, U8 channel, U8 options)
{
    U8 iic_wait_counter = 0x00;
    U8 iic_retry_counter = 0x00;
    do
    {
        if(!I2c_HAL_Is_Busy(channel))
        {
            os_sem_init(i2c_sem_wait[channel],0);
            I2C_HAL_Read(slave_address, iic_data, length, channel, options);

            do
            {
                os_sem_wait(i2c_sem_wait[channel],2 OS_MSECOND);//os_dly_wait(1); //2ms
                tm_delay_us(10);//just fix i2c stop timing here
                if(iic_wait_counter++ >= 50)   //50*2 = 100ms
                {
                    return FALSE;
                }
            }while(I2c_HAL_Is_Busy(channel));
        }
        else
        {
            os_dly_wait(4 OS_MSECOND);// //2ms
            
            if(iic_retry_counter++ >= 50)     //50*2 = 100ms
            {
                return FALSE;
            }
        }
    }while(I2c_HAL_Is_Busy(channel));

    return TRUE;
}

/*******************************************************************************
**  FUNCTION      : iic_drv_down                                         
**  DESCRIPTION   : This function shut down the iic channel
**  PARAMETERS    : channel
**  RETURN        : void                                                          
*******************************************************************************/
void iic_drv_down(U8 channel)
{
    I2C_HAL_Down(channel);
}
/*---Private----------------------------------------------------------*/

