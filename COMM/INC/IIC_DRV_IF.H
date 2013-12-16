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
#ifndef _IIC_DRV_IF_H_
#define _IIC_DRV_IF_H_ 

/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void iic_drv_create(U8 channel, U32 baudrate, U8 addr);
extern BOOL iic_drv_send(U8 slave_address, U8 *iic_data, U8 length, U8 channel, U8 options);
extern BOOL iic_drv_read(U8 slave_address, U8 *iic_data, U8 length, U8 channel, U8 options);
extern void iic_drv_down(U8 channel);




#endif /*_IIC_DRV_IF_H_*/

/****************************** END OF FILE ***************************/
