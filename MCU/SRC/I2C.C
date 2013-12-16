/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : I2C.C
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
#include "..\INC\I2C.H"
#include "..\INC\I2C_IF.H"
#include "..\INC\IRQ_IF.H"
#include "..\INC\EVENT_IF.H"
#include "..\INC\mb9bf404r.h"

static volatile I2C_FRAME_TYPE i2c_message_array[8];
static U8 i2c_slave_tx_char_buffer[8];
static U8 i2c_slave_rx_char_buffer[8];
/***********************************************************************
*  Name        : I2C_Init
*  Description : this function is to Init the I2C before use
*  Parameter   : i2c can select I2C_0,I2C_1,...,I2C_7
                 baudrate is for the master mode only
                 Addr is for the slave only
*  Returns     : None
***********************************************************************/
void I2C_Init( I2C_TYPE* i2c, U32 baudrate, U8 Addr )
{
    U8 i;
    assert_param( IS_I2C_TYPE(i2c) );
    assert_param(IS_RANGE(baudrate,1000,400000) );//baudrate should between 1k,400k
    i = ( ( U32 )i2c >> 8 ) & 0x7;
    i2c->ISMK = 0x00; //i2c disable
    i2c->BGR = APB2_BUS_CLK / baudrate - 1;
    //((UART_TYPEDEF *)i2c)->SCR |=0x80;
    i2c->SMR = 0x80; //i2c mode select
    if(Addr!=0x00)
    {
        i2c->ISBA = I2C_ISBA_SAEN | ( Addr >> 1 );
        i2c->IBCR = 0x3C; //inte  start&stop interrupt enable
    }
    else
    {
         i2c->ISBA = (U8)(~I2C_ISBA_SAEN);//no slave enable
         i2c->IBCR = 0x34; //inte

    }
    i2c->SSR = 0x80;
    i2c->ISMK = 0xFF; //enable i2c
    IRQ_Enable( ( ( ( int )i2c & 0xF00 ) >> 8 ) * 2 + 8 );
    i2c_message_array[i].userflag = 0x00;
}

/***********************************************************************
*  Name        : I2C_AlterRxSize
*  Description : this function is use to Alter the receive size
*  Parameter   : i2c is the channel of I2Cs,size is the new size value

*  Returns     : None
***********************************************************************/
void I2C_AlterRxSize( I2C_TYPE* i2c, U8 size )
{
    U8 i;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x07;
    i2c_message_array[i].size = size;
}


/***********************************************************************
*  Name        : I2C_Handler
*  Description : this function is use for the I2C  IRQ
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void I2C_Handler( I2C_TYPE* i2c )
{
    U8 i;
    I2C_IBCR_TYPE *ibcr;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x07;
    ibcr = ( I2C_IBCR_TYPE * )( I2C_IBCR_BITBAND_BASE + 0x2000 * i );
    if( i2c->IBCR & 0x01 ) //INT
    {
        if( i2c->IBSR & 0x40 ) //RACK=1 means not ack
        {
           if(!(i2c_message_array[i].userflag & I2C_BUS_NOADDR_CHAR))
            {
                i2c_message_array[i].userflag |= ( U8 )I2C_BUS_NACK_BIT;
                i2c_message_array[i].userflag &= ( U8 )~I2C_BUS_BUSY_BIT;
            }
            /*stop*/
            ibcr->I2C_IBCR_MSS = 0x00;
            ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
            return;
        }
        if( i2c->IBCR & 0x02 ) //bus err
        {
            ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
            return ;
        }
        if( ibcr->I2C_IBCR_MSS == 0x01 ) //master
        {
            if( i2c->IBSR & 0x10 ) //TRX bit means TX
            {

                if( i2c_message_array[i].size == 0x00 )
                {
                    if( i2c_message_array[i].options & I2C_NOSTOP ) //Nostop
                    {
                        ibcr->I2C_IBCR_INTE = 0x00;		 //NO make a stop
                    }
                    else
                    {
                        ibcr->I2C_IBCR_MSS = 0x00;	//MSS=0	make a stop
                        ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                    }
                    i2c_message_array[i].userflag &= ( U8 )~I2C_BUS_BUSY_BIT;
                    mcu_i2c_tx_finish_callback(i);
                    /*transmit frame complete on user here*/
                    if( NULL != mcu_i2c_tx_frame_callback[( U8 )( ( ( int )i2c & 0xF00 ) >> 8 )] )
                        mcu_i2c_tx_frame_callback[( U8 )( ( ( int )i2c & 0xF00 ) >> 8 )]();
                    return;
                }
                i2c->TDR = *( i2c_message_array[i].message++ ) ;
                i2c_message_array[i].size--;
                ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag

            }
            else if( ( i2c->IBSR & 0x10 ) == 0x00 )
            {
                if( i2c->IBSR & 0x80 ) //first byte
                {
                    //i2c->RDR;
                    i2c_message_array[i].userflag |= I2C_BUS_NOADDR_CHAR;
                    ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                    return;
                }
                else
                {
                    if( i2c_message_array[i].size == 0x01 )
                    {
                        ibcr->I2C_IBCR_ACKE = 0x00;     //disable ack
                    }

                    *i2c_message_array[i].message++ = i2c->RDR;
                    if( !( i2c_message_array[i].userflag & I2C_RX_NOFIRST_CHAR ) ) //first char
                    {
                        // first char on user here
                        if( NULL != mcu_i2c_rx_char_callback[( U8 )( ( ( int )i2c & 0xF00 ) >> 8 )] )
                            mcu_i2c_rx_char_callback[( U8 )( ( ( int )i2c & 0xF00 ) >> 8 )]();
                        i2c_message_array[i].userflag |= I2C_RX_NOFIRST_CHAR;
                    }
                    i2c_message_array[i].size--;
                    if( i2c_message_array[i].size == 0x00 ) //the last one byte has received
                    {

                        ibcr->I2C_IBCR_MSS = 0x00;		 //mss make a stop
                        ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                        i2c_message_array[i].userflag &= ( U8 )~I2C_BUS_BUSY_BIT;
                        /*receive frame complete on user here*/
                        mcu_i2c_rx_finish_callback(i);
                        if( NULL != mcu_i2c_rx_frame_callback[( U8 )( ( ( int )i2c & 0xF00 ) >> 8 )] )
                            mcu_i2c_rx_frame_callback[( U8 )( ( ( int )i2c & 0xF00 ) >> 8 )]();
                        return;
                    }
                    ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                    return ;
                }

            }
        }
        else //slave   mode
        {
            if( i2c->IBSR & 0x10 ) //TX mode
            {
                if( i2c->IBSR & 0x80 ) //first byte
                {
                    if( NULL != mcu_i2c_slave_tx_firstchar_callback[i] )
                        mcu_i2c_slave_tx_firstchar_callback[i]();
                }
                if( NULL != mcu_i2c_slave_tx_char_callback[i] )
                    mcu_i2c_slave_tx_char_callback[i]();
                
                /*callback here*/
                i2c->TDR = i2c_slave_tx_char_buffer[i];
                ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                ibcr->I2C_IBCR_WSEL = 0x00; //bit9 interrupt
            }
            else	 //Rx mode
            {
                if( i2c->IBSR & 0x80 ) //first byte
                {
                    if( NULL != mcu_i2c_slave_rx_firstchar_callback[i] )
                        mcu_i2c_slave_rx_firstchar_callback[i]();
                    ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                    ibcr->I2C_IBCR_WSEL = 0x01; //bit8 interrupt
                    return;
                }
                else
                {
                    i2c_slave_rx_char_buffer[i] = i2c->RDR;
                    if( NULL != mcu_i2c_slave_rx_char_callback[i] )
                        mcu_i2c_slave_rx_char_callback[i]();
                    ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
                    ibcr->I2C_IBCR_WSEL = 0x01; //bit8 interrupt
                    return;
                }
                

            }

        }
        ibcr->I2C_IBCR_INT = 0x00;	//INT=0	clear int flag
    }
    else if(i2c->IBSR&0x06)
    {
        if(i2c->IBSR&0x02)//stop
        {
            if( NULL != mcu_i2c_stop_callback[i] )
                        mcu_i2c_stop_callback[i]();
            i2c->IBSR &=(U8)~0x02;              
        }
        else//start
        {
            if( NULL != mcu_i2c_start_callback[i] )
                        mcu_i2c_start_callback[i]();
            i2c->IBSR &=(U8)~0x04;            
        }
        
        
    }
   

}

/***********************************************************************
*  Name        : I2C_Slave_SendChar
*  Description : i2c can select I2C_0,I2C_1,...,I2C_7
*  Parameter   :
*  Returns     : None
***********************************************************************/
void I2C_Slave_SendChar( I2C_TYPE* i2c, U8 chr )
{
    U8 i;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x7;
    i2c_slave_tx_char_buffer[i] = chr;
}


/***********************************************************************
*  Name        : I2C_Slave_RecvChar
*  Description : i2c can select I2C_0,I2C_1,...,I2C_7
*  Parameter   :
*  Returns     : None
***********************************************************************/
void I2C_Slave_RecvChar( I2C_TYPE* i2c, U8* chr )
{
    U8 i;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x7;
    *chr = i2c_slave_rx_char_buffer[i];
}


/***********************************************************************
*  Name        : I2C_WriteFrame
*  Description : i2c can select I2C_0,I2C_1,...,I2C_7
                 Addr is the slave to Write
                 pstr is the buffer address to send
                 size is the count to read
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void I2C_WriteFrame( I2C_TYPE* i2c, U8 Addr, U8* pstr, U8 size, U8 options )
{
    U8 i;
    I2C_IBCR_TYPE *ibcr;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x7;
    ibcr = ( I2C_IBCR_TYPE * )( I2C_IBCR_BITBAND_BASE + 0x2000 * i );
    /*
    //busy process here
    */
    i2c_message_array[i].message = pstr;
    i2c_message_array[i].size = size;
    i2c_message_array[i].options = options;
    i2c_message_array[i].userflag = I2C_BUS_BUSY_BIT;
    if( options & I2C_NOSTART )
    {
        i2c->TDR = *i2c_message_array[i].message++;
        i2c_message_array[i].size--;
        ibcr->I2C_IBCR_INT = 0x00; //Clear interrupt flag
        ibcr->I2C_IBCR_INTE = 0x01; //enable interrupt
    }
    else
    {
        i2c->TDR = Addr;
        i2c->IBCR = 0x84; //MSS bit make a start
    }
}



/***********************************************************************
*  Name        : I2C_ReadFrame
*  Description : i2c can select I2C_0,I2C_1,...,I2C_7
                 Addr is the slave to Read
                 pstr is the buffer address to store
                 size is the count to read
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void I2C_ReadFrame( I2C_TYPE* i2c, U8 Addr, U8* pstr, U8 size, U8 options )
{
    U8 i;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x7;
    /*
     //busy process here
    */
    i2c_message_array[i].message = pstr;
    i2c_message_array[i].size = size;
    i2c_message_array[i].options = options;
    i2c_message_array[i].userflag = I2C_BUS_BUSY_BIT;
    i2c->TDR = Addr | 0x01;
    i2c->IBCR = 0xF4; //MSS bit make a start
}

/***********************************************************************
*  Name        : I2C_Disable
*  Description : i2c can select I2C_0,I2C_1,...,I2C_7
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void I2C_Disable( I2C_TYPE* i2c )
{
    assert_param( IS_I2C_TYPE(i2c) );
    i2c->ISMK &= ( U8 )~0x80;
}

/***********************************************************************
*  Name        : I2C_Is_Busy
*  Description : i2c can select I2C_0,I2C_1,...,I2C_7
*  Parameter   : None
*  Returns     : None
***********************************************************************/
BOOL I2C_Get_Flag( I2C_TYPE* i2c,U8 flag )
{
    U8 i;
    assert_param( IS_I2C_TYPE(i2c) );
    i = ( ( U32 )i2c >> 8 ) & 0x7;
    if(flag ==I2C_BUS_BUSY_FLAG)
    {
        if( 0x00 == ( i2c->IBSR & 0x01 ) )
        {
            return FALSE;
        }
        else if( i2c_message_array[i].options & I2C_NOSTOP )
        {
            if( !( i2c_message_array[i].userflag & I2C_BUS_BUSY_BIT ) )
            {
                return FALSE;
            }
        }
        
    }
    else if(flag ==I2C_BUS_NACK_FLAG)
    {
       if(!(i2c_message_array[i].userflag & I2C_BUS_NACK_BIT))
       {
          return FALSE;  
       }
    }
    return TRUE;
    
}
/****************************** END OF FILE ***************************/
