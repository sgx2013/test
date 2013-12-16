/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : CAP.C
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
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"

#include "..\INC\CAP.H"
#include "..\INC\CAP_IF.H"
#include "..\INC\mb9bf404r.h"
#include "..\INC\EVENT_IF.H"
#include "..\INC\IRQ_IF.H"
/***********************************************************************
*  Name        : CAP_Timer_Sel
*  Description :
*  Parameter   : cap can select CAP_MFT_0,CAP_MFT_1
                 cap_unit can select CAP_UNIT_0,...,CAP_UNIT_3
*  Returns     : None 0x42400F00
***********************************************************************/
void CAP_Timer_Sel( CAP_MFT_TYPE* cap, CAP_UNIT_TYPE cap_unit, U8 options )
{
    U32 icfs_addr;
    assert_param( IS_CAP_MFT_TYPE(cap) );
    assert_param(IS_RANGE(cap_unit,CAP_UNIT_0,CAP_UNIT_3) );
    icfs_addr = 0x42400C00 + ( ( ( U32 )cap & 0x1000 ) >> 12 ) * 0x20000 + ( U32 )cap_unit * 0x10;
    /*timer selection below*/
    *( ( volatile U32 * )( icfs_addr + 0x0C ) ) = 0x00;
    *( ( volatile U32 * )( icfs_addr + 0x08 ) ) = ( U32 )( options & 0x04 ) >> 2;
    *( ( volatile U32 * )( icfs_addr + 0x04 ) ) = ( U32 )( options & 0x02 ) >> 1;
    *( ( volatile U32 * )( icfs_addr ) ) = ( U32 )( options & 0x01 );

}
/*
void CAP_Init(CAP_MFT_TYPE* cap,CAP_UNIT_TYPE cap_unit,U8 options)
{
	switch(cap_unit)
	{
	case CAP_UNIT_0:
		cap->ICFS10 &= 0xF0;//clear low 4bits
		cap->ICFS10 |= ( U8 )(options&0x0F);
		cap->ICSA10 &=0xEF;
		cap->ICSA10 |=(options&0x10);
		break;
	case CAP_UNIT_1:
		cap->ICFS10 &=0x0F;
		cap->ICFS10 |=(options&0x0F)<<4;
		cap->ICSA10 &=0xDF;
		cap->ICSA10 |=(options&0x10)<<1;
		break;
	case CAP_UNIT_2:
		cap->ICFS32 &= 0xF0;//clear low 4bits
		cap->ICFS32 |= ( U8 )(options&0x0F);
		cap->ICSA32 &=0xEF;
		cap->ICSA32 |=(options&0x10);
		break;
	case  CAP_UNIT_3:
		cap->ICFS32 &=0x0F;
		cap->ICFS32 |=(options&0x0F)<<4;
		cap->ICSA32 &=0xDF;
		cap->ICSA32 |=(options&0x10)<<1;
		break;
	default:
		break;
	}
}
*/
/***********************************************************************
*  Name        : CAP_Cmd
*  Description :
*  Parameter   : cap can select CAP_MFT_0,CAP_MFT_1
                 cap_unit can select CAP_UNIT_0,...,CAP_UNIT_3
                 cap_mode can select CAP_MODE_STOP  CAP_MODE_RISING
                           CAP_MODE_FALLING CAP_MODE_RISING_FALLING
*  Returns     : None
***********************************************************************/

void CAP_Config( CAP_MFT_TYPE* cap, CAP_UNIT_TYPE cap_unit, CAP_MODE_TYPE cap_mode )
{
    U32 icsa_addr;
    assert_param( IS_CAP_MFT_TYPE(cap) );
    assert_param( IS_RANGE(cap_unit,CAP_UNIT_0,CAP_UNIT_3 ) );
    assert_param( IS_RANGE( cap_mode,CAP_MODE_STOP,CAP_MODE_RISING_FALLING ) );
    icsa_addr = 0x42400F00 + ( ( ( U32 )cap & 0x1000 ) >> 12 ) * 0x20000;
    if( cap_unit & 0x2 )
    {
        icsa_addr += 0x80; //cap unit2/3
    }
    if( cap_unit & 0x1 ) //cap1/3
    {
        *( ( volatile U32 * )( icsa_addr + 0x1C ) ) = 0x00; //ICP=0x00
        *( ( volatile U32 * )( icsa_addr + 0x08 ) ) = cap_mode & 0x01; //cap mode
        *( ( volatile U32 * )( icsa_addr + 0x0C ) ) = ( cap_mode & 0x02 ) >> 1;
        *( ( volatile U32 * )( icsa_addr + 0x14 ) ) = 0x01; //enable interrupt
    }
    else
    {
        *( ( volatile U32 * )( icsa_addr + 0x18 ) ) = 0x00; //ICP=0x00
        *( ( volatile U32 * )( icsa_addr + 0x00 ) ) = cap_mode & 0x01; //cap mode
        *( ( volatile U32 * )( icsa_addr + 0x04 ) ) = ( cap_mode & 0x02 ) >> 1;
        *( ( volatile U32 * )( icsa_addr + 0x10 ) ) = 0x01; //enable interrupt
    }
    IRQ_Enable( 29 );
}
/*
void CAP_Cmd(CAP_MFT_TYPE* cap,CAP_UNIT_TYPE cap_unit,CAP_MODE_TYPE cap_mode)
{
	switch(cap_unit)
	{
	case CAP_UNIT_0:
		cap->ICSA10 &=0xFC;
		cap->ICSA10 |=cap_mode;
		break;
	case CAP_UNIT_1:
		cap->ICSA10 &=0xF3;
		cap->ICSA10 |=cap_mode<<2;
		break;
	case CAP_UNIT_2:
		cap->ICSA32 &=0xFC;
		cap->ICSA32 |=cap_mode;
		break;
	case CAP_UNIT_3:
		cap->ICSA32 &=0xF3;
		cap->ICSA32 |=cap_mode<<2;
		break;
	default:
		break;

	}
}*/
/***********************************************************************
*  Name        : CAP_GetValue
*  Description :
*  Parameter   : cap can select CAP_MFT_0,CAP_MFT_1
                 cap_unit can select CAP_UNIT_0,...,CAP_UNIT_3
*  Returns     : U16
***********************************************************************/
U16 CAP_GetValue( CAP_MFT_TYPE* cap, CAP_UNIT_TYPE cap_unit )
{
    U16 cap_value = 0;
    assert_param( IS_CAP_MFT_TYPE(cap) );
    assert_param( IS_RANGE(cap_unit,CAP_UNIT_0,CAP_UNIT_3 ) );   
    switch( cap_unit )
    {
    case CAP_UNIT_0:
        cap_value = cap->ICCP0; //default unit0
        break;
    case CAP_UNIT_1:
        cap_value = cap->ICCP1;
        break;
    case  CAP_UNIT_2:
        cap_value = cap->ICCP2;
        break;
    case CAP_UNIT_3:
        cap_value = cap->ICCP3;
        break;
    default:
        break;

    }
    return cap_value;
}


/***********************************************************************
*  Name        : CAP_Handle_Process
*  Description : this function just for IRQ only!
*  Parameter   : cap can select CAP_MFT_0,CAP_MFT_1
                 cap_unit can select CAP_UNIT_0,...,CAP_UNIT_3
*  Returns     : None
***********************************************************************/
void CAP_Handle_Process( void )
{
    U16 flag = FM3_INTREQ->IRQ29MON;
    if( flag & 0x01 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[0] != NULL )
            mcu_cap_callback[0]();
        bFM3_MFT0_ICU_ICSA10_IPC0 = 0x00; //clear irq flag
    }
    if( flag & 0x02 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[1] != NULL )
            mcu_cap_callback[1]();
        bFM3_MFT0_ICU_ICSA10_IPC1 = 0x00; //clear irq flag
    }
    if( flag & 0x04 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[2] != NULL )
            mcu_cap_callback[2]();
        bFM3_MFT0_ICU_ICSA32_IPC2 = 0x00; //clear irq flag
    }
    if( flag & 0x08 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[3] != NULL )
            mcu_cap_callback[3]();
        bFM3_MFT0_ICU_ICSA32_IPC3 = 0x00; //clear irq flag
    }

    if( flag & 0x10 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[4] != NULL )
            mcu_cap_callback[4]();
        bFM3_MFT1_ICU_ICSA10_IPC0 = 0x00; //clear irq flag
    }
    if( flag & 0x20 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[5] != NULL )
            mcu_cap_callback[5]();
        bFM3_MFT1_ICU_ICSA10_IPC1 = 0x00; //clear irq flag
    }
    if( flag & 0x40 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[6] != NULL )
            mcu_cap_callback[6]();
        bFM3_MFT1_ICU_ICSA32_IPC2 = 0x00; //clear irq flag
    }
    if( flag & 0x80 )
    {
        /*callbace user function here*/
        if( mcu_cap_callback[7] != NULL )
            mcu_cap_callback[7]();
        bFM3_MFT1_ICU_ICSA32_IPC3 = 0x00; //clear irq flag
    }
}
/**************** END OF FILE *****************************************/
