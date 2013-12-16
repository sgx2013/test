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
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\GPIO.H"
#include "..\INC\GPIO_IF.H"
/***********************************************************************
*  Name        : GPIO_SetVal
*  Description : Set the GPIO Pin the high level
*  Parameter   : port can select GPIO_PORT_0 ~ GPIO_PORT_7
                 pin can select   GPIO_PIN_0 ~ GPIO_PIN_7
*  Returns     : None
***********************************************************************/
void GPIO_SetVal( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin )
{
    assert_param(IS_RANGE(port,GPIO_PORT_0,GPIO_PORT_8) );
    assert_param(IS_RANGE(pin,GPIO_PIN_0,GPIO_PIN_F) );
    *( ( volatile int * )( GPIO_PDOR_BASE + 0x80 * port + 4 * pin ) ) = 0x01;
}

/***********************************************************************
*  Name        : GPIO_ClrVal
*  Description : make the GPIO Pin the low level
*  Parameter   : port can select GPIO_PORT_0 ~ GPIO_PORT_7
                 pin can select   GPIO_PIN_0 ~ GPIO_PIN_7
*  Returns     : None
***********************************************************************/
void GPIO_ClrVal( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin )
{
    assert_param(IS_RANGE(port,GPIO_PORT_0,GPIO_PORT_8));
    assert_param(IS_RANGE(pin,GPIO_PIN_0,GPIO_PIN_F));
    *( ( volatile int * )( GPIO_PDOR_BASE + 0x80 * port + 4 * pin ) ) = 0x00;
}

/***********************************************************************
*  Name        : GPIO_GetVal
*  Description : get the GPIO Pin  level  value
*  Parameter   : port can select GPIO_PORT_0 ~ GPIO_PORT_7
                 pin can select   GPIO_PIN_0 ~ GPIO_PIN_7
*  Returns     : U32  only the LSB is available
***********************************************************************/
U32 GPIO_GetVal( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin )
{
    assert_param(IS_RANGE(port,GPIO_PORT_0,GPIO_PORT_8) );
    assert_param(IS_RANGE(pin,GPIO_PIN_0,GPIO_PIN_F) );
    return ( *( volatile int * )( GPIO_PDIR_BASE + 0x80 * port + 4 * pin ) ) ;
}


/***********************************************************************
*  Name        : GPIO_SetOutput
*  Description : Set GPIO Pin  as a Output pin
*  Parameter   : port can select GPIO_PORT_0 ~ GPIO_PORT_7
                 pin can select   GPIO_PIN_0 ~ GPIO_PIN_7
*  Returns     : None
***********************************************************************/
void GPIO_SetOutput( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin )
{
    assert_param(IS_RANGE(port,GPIO_PORT_0,GPIO_PORT_8) );
    assert_param(IS_RANGE(pin,GPIO_PIN_0,GPIO_PIN_F) );
    *( ( volatile int * )( GPIO_DDR_BASE + 0x80 * port + 4 * pin ) ) = 0x01;
}

/***********************************************************************
*  Name        : GPIO_SetInput
*  Description : Set GPIO Pin  as a Input pin
*  Parameter   : port can select GPIO_PORT_0 ~ GPIO_PORT_7
                 pin can select   GPIO_PIN_0 ~ GPIO_PIN_7
*  Returns     : None
***********************************************************************/
void GPIO_SetInput( GPIO_PORT_TYPE port, GPIO_PIN_TYPE pin )
{
    assert_param(IS_RANGE(port,GPIO_PORT_0,GPIO_PORT_8) );
    assert_param(IS_RANGE(pin,GPIO_PIN_0,GPIO_PIN_F) );
    *( ( volatile int * )( GPIO_DDR_BASE + 0x80 * port + 4 * pin ) ) = 0x00;
}
/**************** END OF FILE *****************************************/
