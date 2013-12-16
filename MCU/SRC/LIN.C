/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN1 R&D SW     									   *
*   AUTHOR	   : CYH        										   *
************************************************************************
* Object        : LIN.C
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:  0.1
* Date: 2013-03-14
* Author: Chen Yuehao
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
/****  include files ****/
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\LIN.H"
#include "..\INC\LIN_IF.H"
#include "..\INC\IRQ_IF.H"
#include "..\INC\CortexM3.h"
#include "..\INC\mb9bf404r.h"
#include "..\INC\ERR_DEF.H"
#include "..\INC\EVENT_IF.H"

volatile static U8 lin_read_buffer[8];

/***********************************************************************
*  Name        : LIN_Init
*  Description :
*  Parameter   : lin-> LIN_0,LIN_1...,LIN_7
                 baudrate -> 115200,19200,9600...
                 options-> LIN_RX_IT|LIN_TX_IT|LIN_RX_EN|LIN_TX_EN...
*  Returns     : None
***********************************************************************/
void LIN_Init( LIN_TYPE* lin, LIN_BAUDRATE_TYPE baudrate, U16 options )
{
    assert_param(IS_LIN_TYPE(lin));
    assert_param(IS_LIN_BAUDRATE_TYPE(baudrate));
    lin->SCR = LIN_SETUP_ENABLE ;
    lin->SMR = LIN_SMR_DEFAULT ;
    lin->SSR = LIN_RECEIVE_ERR_CLR ;
    lin->ESCR = ( options & 0xFF00 ) >> 8 ;
    lin->BGR = baudrate;
    lin->SCR = options & 0xFF ;
    IRQ_Enable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
    IRQ_Enable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
}


/***********************************************************************
*  Name        : LIN_IrqEnable
*  Description :
*  Parameter   : lin->LIN_0,LIN_1,...LIN_7
*  Returns     : None
***********************************************************************/
void LIN_IrqEnable( LIN_TYPE* lin )
{
    IRQ_Enable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
    IRQ_Enable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
}

/***********************************************************************
*  Name        : LIN_IrqEnable
*  Description :
*  Parameter   : lin->LIN_0,LIN_1,...LIN_7
*  Returns     : None
***********************************************************************/
void LIN_IrqDisable( LIN_TYPE* lin )
{
    IRQ_Disable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt disable
    IRQ_Disable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt disable
}

/***********************************************************************
*  Name        : LIN_SendBreak
*  Description :
*  Parameter   : lin->LIN_0,LIN_1,...LIN_7
*  Returns     : None
***********************************************************************/
void LIN_SendBreak( LIN_TYPE* lin )
{
    assert_param(IS_LIN_TYPE(lin));
    lin->SSR &= ~LIN_SSR_LBD;
    lin->SCR |= LIN_LBR_GEN;
    lin->ESCR |= LIN_LB_IT; //enable LIN Break field detected interrupt
}

/***********************************************************************
*  Name        : LIN_SendChar
*  Description :
*  Parameter   : lin->LIN_0,LIN_1,...LIN_7
                 chr->byte
*  Returns     : U8
***********************************************************************/
U8 LIN_SendChar( LIN_TYPE* lin, U8 chr )
{
    assert_param(IS_LIN_TYPE(lin));
    if( !( lin->SSR & LIN_SSR_TDRE ) )
        return ERR_OVERRUN;
    lin->TDR = chr;
    lin->SCR |= LIN_TX_IT; //enable TX  interrupt
    return ERR_OK;
}

/***********************************************************************
*  Name        : LIN_RecvChar
*  Description :
*  Parameter   : lin->  LIN_0,LIN_1,...LIN_7
                 recv_char-> byte Address
*  Returns     : U8
***********************************************************************/
U8 LIN_RecvChar( LIN_TYPE* lin, U8 *recv_char )
{
    assert_param(IS_LIN_TYPE(lin));
    *recv_char = lin_read_buffer[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )];
    return ERR_OK;
}

/***********************************************************************
*  Name        : LIN_Tx_Handle
*  Description :
*  Parameter   : lin->  LIN_0,LIN_1,...LIN_7
*  Returns     : None
***********************************************************************/
void LIN_Tx_Handler( LIN_TYPE* lin )
{
    assert_param(IS_LIN_TYPE(lin));
    lin->SCR &= ( U8 )~LIN_TX_IT; //disable TX  interrupt
    /*invoke user function here*/
    if( NULL != mcu_lin_tx_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )] )
        mcu_lin_tx_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )]();
}

/***********************************************************************
*  Name        : LIN_Rx_Handle
*  Description :
*  Parameter   : lin->  LIN_0,LIN_1,...LIN_7
*  Returns     : None
***********************************************************************/
void LIN_Rx_Handler( LIN_TYPE* lin )
{
    assert_param(IS_LIN_TYPE(lin)); 
    if( lin->SSR & LIN_SSR_ERR )
    {
        lin->SSR |= LIN_RECEIVE_ERR_CLR;
        lin->RDR;//Flush the receive data

        /*invoke user function here*/
        if( NULL != mcu_lin_err_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )] )
            mcu_lin_err_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )]();
    }
    else 
    {
        if( lin->SSR & LIN_SSR_LBD )
        {
            lin->SSR &= ~LIN_SSR_LBD;
            lin->ESCR &= ~LIN_LB_IT; //disable LIN Break field detected interrupt
            
            /*invoke user function here*/
            if( NULL != mcu_lin_lb_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )] )
                mcu_lin_lb_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )]();
        }
        if( lin->SSR & LIN_SSR_RDRF )
        {
            lin_read_buffer[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )] = lin->RDR;

            /*invoke user function here*/
            if( NULL != mcu_lin_rx_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )] )
                mcu_lin_rx_callback[( U8 )( ( ( U32 )lin & 0xF00 ) >> 8 )]();
        }
    }
}

/***********************************************************************
*  Name        : LIN_Disable
*  Description :
*  Parameter   : lin->LIN_0,LIN_1,...LIN_7
*  Returns     : None
***********************************************************************/
void LIN_Disable( LIN_TYPE* lin )
{
    assert_param(IS_LIN_TYPE(lin));
	IRQ_Disable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
    IRQ_Disable( ( ( ( U32 )lin & 0xF00 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
    lin->SCR = 0x80;//CPCL=1,NO tx,rx,and no interrupt enable
    lin->SMR &= 0xFE;//Disables a serial data output.
}

/**************** END OF FILE *****************************************/

