/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : UART.c
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version: 0.1
* Date: 2011-11-24
* Author: Shao Guangxian
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
/****  include files ****/
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\UART.H"
#include "..\INC\UART_IF.H"
#include "..\INC\IRQ_IF.H"
#include "..\INC\CortexM3.h"
#include "..\INC\mb9bf404r.h"
#include "..\INC\ERR_DEF.H"
#include "..\INC\EVENT_IF.H"

volatile static U8 uart_read_buffer[8];

/***********************************************************************
*  Name        : UART_Init
*  Description :
*  Parameter   : uart-> UART_0,UART_1...,UART_7
                 baudrate -> 115200,19200,9600...
                 options-> UART_RX_IT|UART_TX_IT|UART_RX_EN|UART_TX_EN...
*  Returns     : None
***********************************************************************/
void UART_Init( UART_TYPE* uart, UART_BAUDRATE_TYPE baudrate, U16 options )
{
    assert_param(IS_UART_TYPE(uart));
    assert_param(IS_UART_BAUDRATE_TYPE(baudrate));
    uart->SCR = UART_SETUP_ENABLE ;
    uart->SMR = UART_SMR_DEFAULT ;
    uart->SSR = UART_RECEIVE_ERR_CLR ;
    uart->ESCR = ( options & 0xFF00 ) >> 8 ;
    uart->BGR = baudrate;
    uart->SCR = options & 0xFF ;
    IRQ_Enable( ( ( ( U32 )uart & 0xF00 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
    IRQ_Enable( ( ( ( U32 )uart & 0xF00 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
}

/***********************************************************************
*  Name        : UART_SendChar
*  Description :
*  Parameter   : uart->UART_0,UART_1,...UART_7
                 chr->byte
*  Returns     : U8
***********************************************************************/
U8 UART_SendChar( UART_TYPE* uart, U8 chr )
{
    assert_param(IS_UART_TYPE(uart));
    if( !( uart->SSR & UART_SSR_TDRE ) )
        return ERR_OVERRUN;
    uart->TDR = chr;
    uart->SCR |= UART_TX_IT; //enable TX  interrupt
    return ERR_OK;
}


/***********************************************************************
*  Name        : UART_TX_Isr_Enable
*  Description :
*  Parameter   : uart->UART_0,UART_1,...UART_7
                 chr->byte
*  Returns     : U8
***********************************************************************/
void UART_TX_Isr_Enable(UART_TYPE* uart)
{
    uart->SCR |= UART_TX_IT; //enable TX  interrupt
}



/***********************************************************************
*  Name        : UART_RecvChar
*  Description :
*  Parameter   : uart->  UART_0,UART_1,...UART_7
                 recv_char-> byte Address
*  Returns     : U8
***********************************************************************/
U8 UART_RecvChar( UART_TYPE* uart, U8 *recv_char )
{
    assert_param(IS_UART_TYPE(uart));
    *recv_char = uart_read_buffer[( U8 )( ( ( U32 )uart & 0xF00 ) >> 8 )];
    return ERR_OK;
}



/***********************************************************************
*  Name        : UART_Tx_Handle
*  Description :
*  Parameter   : uart->  UART_0,UART_1,...UART_7
*  Returns     : None
***********************************************************************/
void UART_Tx_Handler( UART_TYPE* uart )
{
    assert_param(IS_UART_TYPE(uart));
    uart->SCR &= ( U8 )~UART_TX_IT; //disable TX  interrupt
    /*invoke user function here*/
    if( NULL != mcu_uart_tx_callback[( U8 )( ( ( U32 )uart & 0xF00 ) >> 8 )] )
        mcu_uart_tx_callback[( U8 )( ( ( U32 )uart & 0xF00 ) >> 8 )]();

}

/***********************************************************************
*  Name        : UART_Rx_Handle
*  Description :
*  Parameter   : uart->  UART_0,UART_1,...UART_7
*  Returns     : None
***********************************************************************/
void UART_Rx_Handler( UART_TYPE* uart )
{
    assert_param(IS_UART_TYPE(uart));
    if( uart->SSR & UART_SSR_ERR )
    {
        uart->SSR |= UART_RECEIVE_ERR_CLR;
        uart->RDR;//Flush the receive data
    }
    else if( uart->SSR & UART_SSR_RDRF )
    {
        uart_read_buffer[( U8 )( ( ( U32 )uart & 0xF00 ) >> 8 )] = uart->RDR;

        /*invoke user function here*/
        if( NULL != mcu_uart_rx_callback[( U8 )( ( ( U32 )uart & 0xF00 ) >> 8 )] )
            mcu_uart_rx_callback[( U8 )( ( ( U32 )uart & 0xF00 ) >> 8 )]();
    }
}

/***********************************************************************
*  Name        : UART_Disable
*  Description :
*  Parameter   : uart->UART_0,UART_1,...UART_7
*  Returns     : None
***********************************************************************/
void UART_Disable( UART_TYPE* uart )
{
    assert_param(IS_UART_TYPE(uart));
		IRQ_Disable( ( ( ( U32 )uart & 0xF00 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
    IRQ_Disable( ( ( ( U32 )uart & 0xF00 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
    uart->SCR = 0x80;//CPCL=1,NO tx,rx,and no interrupt enable
    uart->SMR &= 0xFE;//Disables a serial data output.
}

/**************** END OF FILE *****************************************/
