/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : SPI.C
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
#include "..\INC\SPI.H"
#include "..\INC\SPI_IF.H"
#include "..\INC\EVENT_IF.H"
#include "..\INC\IRQ_IF.H"
#include "..\..\SYSTEM\INC\TOOLS_IF.H"
volatile static U8  spi_write_buffer[8];
volatile static U8  spi_read_buffer[8];
volatile static U8  user_flag[8] __attribute__( ( section ( "bit_band_ram_data" ) ) );

/***********************************************************************
*  Name        : SPI_Init
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
                 baudrate just for the master mode
                 options->SPI_MASTER|SPI_MODE_0|SPI_RX_IT|SPI_RX_EN|SPI_TX_EN...
*  Returns     : None
***********************************************************************/

void SPI_Init( SPI_TYPE* spi, U32 baudrate, U16 options )
{
	assert_param(IS_SPI_TYPE(spi));
	assert_param(IS_RANGE(baudrate,1000,10000000) );//baudrate should between 1k,10M
    spi->SCR = SPI_RESET_BIT;
	spi->SMR = ( U8 )( options >> 8 );
	spi->SCR = ( ( options & 0xFC ) );
	spi->SSR = 0x80;
	spi->ESCR = 0x00; //8bit
	spi->BGR = APB2_BUS_CLK / baudrate - 1;
	spi->SCR |= ( U8 )( options & 0x3 );
	/*slave mode first byte  bug process below*/
	/*if(options&0x40)
	{
		spi->TDR=0x00;//dummy data
	}*/
	user_flag[( U8 )( ( ( int )spi & 0x700 ) >> 8 )] = 0x00;
	IRQ_Enable( ( ( ( int )spi & 0x700 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
	IRQ_Enable( ( ( ( int )spi & 0x700 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
}

#ifdef CT_G4_IPC
//spi init and reset
void SPI_InitFIFOMode(SPI_FIFOTYPE *spi)
{
    spi->SCR &= ~(RX_ENABLE|TX_ENABLE); //disable tx, rx
    spi->SCR |= SPI_SW_RESET; //UPCL
    spi->SMR = MFS_MODE2|MSB_FIRST|SOUT_ENABLE;
    spi->SCR = SPI_SLAVE_MODE;
    spi->SSR = CLR_RX_ERROR; //REC
    spi->ESCR = 0x00;

    spi->FCR0 = FIFO2_RESET|FIFO1_RESET|FIFO2_ENABLE|FIFO1_ENABLE;
    spi->FCR1 = 0x00; //tx: fifo1, rx:fifo2
    spi->FBYTE2 = FIFO_DEPTH;

    spi->SCR |= RX_INT_ENABLE/*|TX_INT_ENABLE*/;
    spi->SCR |= (RX_ENABLE | TX_ENABLE);
}

void SPI_Enable(SPI_FIFOTYPE *spi)
{
    IRQ_Flag_Clear(( ( ( int )spi & 0xF00 ) >> 8 ) * 2 + 7);
    IRQ_Flag_Clear(( ( ( int )spi & 0xF00 ) >> 8 ) * 2 + 8);
    IRQ_Enable(( ( ( int )spi & 0xF00 ) >> 8 ) * 2 + 7); //IRQ17: Rx interrupt to CPU
    IRQ_Enable(( ( ( int )spi & 0xF00 ) >> 8 ) * 2 + 8); //IRQ18: Tx interrupt to CPU
}

void SPI_FIFO_TIE_On(SPI_FIFOTYPE *spi)
{
    spi->SCR |= TX_INT_ENABLE;
}
void SPI_FIFO_TIE_Off(SPI_FIFOTYPE *spi)
{
    spi->SCR &= ~TX_INT_ENABLE;
}

BOOL SPI_ORECheck(SPI_FIFOTYPE *spi)
{
    if (spi->SSR&RX_ORE_BIT) //interrupt trigerred by overrun error
        return (TRUE);
    else
        return (FALSE);
}

void SPI_CLRRxError(SPI_FIFOTYPE *spi)
{
    spi->SSR|=CLR_RX_ERROR;	//clear error
    spi->FCR0 |= FIFO2_ENABLE; //re-enable fifo2
}

void SPI_ResetFIFO(SPI_FIFOTYPE *spi)
{
    //spi reset sequence
    spi->SCR &= ~(RX_ENABLE|TX_ENABLE); //disable tx, rx
    spi->FCR0 &= ~(FIFO2_ENABLE|FIFO1_ENABLE); //disable fifo2/1
    spi->SCR |= SPI_SW_RESET;
    spi->FCR0 |= FIFO2_RESET|FIFO1_RESET;
    //enable back fifo2/1 and tx, rx
    spi->FCR0 |= (FIFO2_ENABLE|FIFO1_ENABLE); //enable fifo2/1
}

void SPI_EnableFIFO(SPI_FIFOTYPE *spi)
{
    spi->SCR |= RX_ENABLE | TX_ENABLE; //enable tx, rx
}
#endif //CT_G4_IPC

/***********************************************************************
*  Name        : SPI_For_Slave_CS
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
                 state->ENABLE/DISABLE
*  Returns     : None
***********************************************************************/
void SPI_For_Slave_CS( SPI_TYPE* spi, FUNCTION_STATE state )
{
	RAM_BIT_BAND_TYPE *pb_user_flag;
	assert_param(IS_SPI_TYPE(spi));
	assert_param(IS_FUNCTION_STATE(state));
	pb_user_flag = get_ram_bitband((U8 *)&user_flag[( U8 )( ( ( int )spi & 0x700 ) >> 8 )]);
	if( state != DISABLE )
	{
		spi->SCR |= 0x02; //enable receive
		if(!(pb_user_flag->bit[SPI_NOFIRSTCHAR_IN_TX]) )
		{
			spi->TDR = 0x00;
            pb_user_flag->bit[SPI_NOFIRSTCHAR_IN_TX]= 0x01;
		}
	}
	else
	{
		spi->SCR &= ( U8 )~0x02; //disable receive
		spi->SSR |= 0x80; //clear receive error
		spi->RDR;
	}

}

/***********************************************************************
*  Name        : SPI_TX_Isr_Enable
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
                 chr is the address to store
*  Returns     : U8
***********************************************************************/
void SPI_TX_Isr_Enable(SPI_TYPE* spi)
{
    spi->SCR |= SPI_TX_IT; //enable tx interrupt;
}

/***********************************************************************
*  Name        : SPI_RX_Enable
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
*  Returns     : 
***********************************************************************/
void SPI_RX_Enable(SPI_TYPE* spi)
{
    spi->SCR |= RX_ENABLE; //enable rx;
}

/***********************************************************************
*  Name        : SPI_RX_Disable
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
*  Returns     : 
***********************************************************************/
void SPI_RX_Disable(SPI_TYPE* spi)
{
    spi->SCR &= (U8)~RX_ENABLE; //disable rx;
}


/***********************************************************************
*  Name        : SPI_Read_Char
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
                 chr is the address to store
*  Returns     : U8
***********************************************************************/
U8 SPI_Read_Char( SPI_TYPE* spi, U8* chr )
{
    RAM_BIT_BAND_TYPE *pb_user_flag;
	  assert_param(IS_SPI_TYPE(spi));
    pb_user_flag = get_ram_bitband((U8 *)&user_flag[( U8 )( ( ( int )spi & 0x700 ) >> 8 )]);
	if( ( spi->SCR & 0x40 ) != 0x00 ) //slave mode
	{
		if( !( pb_user_flag->bit[SPI_CHAR_IN_RX] ) ) 
			return ERR_RXEMPTY;
	}
	*chr = spi_read_buffer[( U8 )( ( ( int )spi & 0xF00 ) >> 8 )];
    pb_user_flag->bit[SPI_CHAR_IN_RX] = 0x00;//clear char_in_rx
	return ERR_OK;
}

/***********************************************************************
*  Name        : SPI_Read_Char
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
                 chr is the byte to send
*  Returns     : U8
***********************************************************************/
U8 SPI_Write_Char( SPI_TYPE* spi, U8 chr )
{
	RAM_BIT_BAND_TYPE *pb_user_flag;
	assert_param(IS_SPI_TYPE(spi));
	pb_user_flag = get_ram_bitband((U8 *)&user_flag[( U8 )( ( ( int )spi & 0x700 ) >> 8 )]);
	if(pb_user_flag->bit[SPI_CHAR_IN_TX]) //buf is not empty
		return ERR_OVERRUN;
	spi_write_buffer[( U8 )( ( ( int )spi & 0x700 ) >> 8 )] = chr;
	pb_user_flag->bit[SPI_CHAR_IN_TX] = 0x01;//set char_in_tx flag
	if(((spi->SCR&0x40) != 0x40 ) ||(pb_user_flag->bit[SPI_NOFIRSTCHAR_IN_TX]))//if master mode or slave mode but 
	{
		spi->SCR |= SPI_TX_IT; //enable tx interrupt
	}
	return ERR_OK;
}

/***********************************************************************
*  Name        : SPI_Tx_Handler
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7

*  Returns     : None
***********************************************************************/
void SPI_Tx_Handler( SPI_TYPE* spi )
{
	RAM_BIT_BAND_TYPE *pb_user_flag;
	assert_param(IS_SPI_TYPE(spi));
	spi->SCR &= ( U8 )~SPI_TX_IT; //disable tx interrupt
	pb_user_flag = get_ram_bitband((U8 *)&user_flag[( U8 )( ( ( int )spi & 0x700 ) >> 8 )]);

	if(pb_user_flag->bit[SPI_CHAR_IN_TX])
	{
		spi->TDR = spi_write_buffer[( U8 )( ( ( int )spi & 0x700 ) >> 8 )];
		pb_user_flag->bit[SPI_CHAR_IN_TX] = 0x00;//clear char_in_tx flag
		/*invoke user function here*/
		if( NULL != mcu_spi_tx_callback[( U8 )( ( ( int )spi & 0xF00 ) >> 8 )] )
			mcu_spi_tx_callback[( U8 )( ( ( int )spi & 0xF00 ) >> 8 )]();
	}

}

#ifdef CT_G4_IPC
//entrance of fiforx isr, all handling implemented in spi_navi_fiforx_proc(SPI_TYPE *spi)
void SPI_FIFORx_Handler(SPI_FIFOTYPE *spi)
{
    U8 go_rxprocess = 0;
    U8 counter, eventcode = 0;

    if (spi->SSR & RX_ORE_BIT)
    {
        eventcode = 1; //ove interrupt
        //spi->SSR |= CLR_RX_ERROR;
        //spi->FCR0 |= FIFO2_ENABLE; //re-enable fifo2
        go_rxprocess = 1;
    }
    if (spi->SSR & RX_RDRF_BIT) //RDR full
    {
        eventcode = 2;
        counter = spi->FBYTE2;  //check #bytes in fifo2
        if (counter >= FIFO_DEPTH)
            go_rxprocess = 1;
        else
            eventcode = 3;
    }
    else
        eventcode = 88; //other strange interrupt causes!!!
    if (go_rxprocess == 1)
        mcu_spi_rx_callback[(U8)(((int)spi&0xF00)>>8)](); //rx process for actual application
}

void SPI_FIFOTx_Handler(SPI_FIFOTYPE *spi)
{
    /* U8 counter, eventcode =0; */

    if (spi->SSR & TX_TDRE_BIT)
        /*below for debugging purpose
        	eventcode =1; //byte tx interrupt
        if(spi->SSR & TX_BUSIDLE_BIT)
        		eventcode = 2; //tx bus idle int
        if(spi->FCR1 & TXFIFO_EMPTY)
        {
        	if(spi->FBYTE1 ==0) //real empty
        		eventcode = 3; //tx fifo empty int
        	else
        	{
        		counter = spi->FBYTE1; //strange, FBYTE should be 0!!!
        		eventcode =4;
                //SPI_FIFO_TIE_Off(spi); //switch off tx interrupt
        	}
        }
        else
        {
        	eventcode = 88; //strange, other interrupt causes!!!
        	SPI_FIFO_TIE_Off(spi); //switch off tx interrupt
        }
        if (eventcode == 1) //from 3 to 1
        */
    {
        //SPI_FIFO_TIE_Off(spi);
        mcu_spi_tx_callback[(U8)(((int)spi&0xF00)>>8)](); //tx process for actual application
    }
}
#endif //CT_G4_IPC


/***********************************************************************
*  Name        : SPI_Rx_Handler
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7

*  Returns     : None
***********************************************************************/


void SPI_Rx_Handler( SPI_TYPE* spi )
{
    RAM_BIT_BAND_TYPE *pb_user_flag;
	assert_param(IS_SPI_TYPE(spi));
    pb_user_flag = get_ram_bitband((U8 *)&user_flag[( U8 )( ( ( int )spi & 0x700 ) >> 8 )]);
	if( spi->SSR & 0x08 ) //overrun
	{
		spi->SSR |= SPI_RESET_BIT; //clear error
		spi->RDR;//flush RDR register
	}
	else if( spi->SSR & 0x04 ) //RX_no_empty
	{
		spi_read_buffer[( U8 )( ( ( int )spi & 0xF00 ) >> 8 )] = spi->RDR;
		pb_user_flag->bit[SPI_CHAR_IN_RX] = 0x01 ;//set char_in_rx flag
		/*invoke user function here*/
		if( NULL != mcu_spi_rx_callback[( U8 )( ( ( int )spi & 0xF00 ) >> 8 )] )
			mcu_spi_rx_callback[( U8 )( ( ( int )spi & 0xF00 ) >> 8 )]();
	}
}



/***********************************************************************
*  Name        : SPI_SetBaudrate
*  Description :
*  Parameter   : spi-> SPI_0,SPI_1...,SPI_7
                 baudrate just for the master mode to generate clock
*  Returns     : None
***********************************************************************/
void SPI_SetBaudrate( SPI_TYPE* spi, U32 baudrate )
{
    assert_param(IS_SPI_TYPE(spi));
    spi->SCR = SPI_RESET_BIT;
    spi->BGR = APB2_BUS_CLK / baudrate - 1;
}

/***********************************************************************
*  Name        : SPI_Disable
*  Description : disable spi module
*  Parameter   : uart->UART_0,UART_1,...UART_7
*  Returns     : None
***********************************************************************/
void SPI_Disable( SPI_TYPE* spi )
{
    assert_param(IS_SPI_TYPE(spi));
    spi->SMR = 0x00;//spi->uart mode ,and no CLK,or output enable
    spi->SCR = 0x80;//CPCL=1,NO tx,rx,and no interrupt enable
		IRQ_Disable( ( ( ( int )spi & 0x700 ) >> 8 ) * 2 + 7 ); //IRQ Rx  interrupt enable
		IRQ_Disable( ( ( ( int )spi & 0x700 ) >> 8 ) * 2 + 8 ); //IRQ Tx  interrupt enable
}

/**************** END OF FILE *****************************************/
