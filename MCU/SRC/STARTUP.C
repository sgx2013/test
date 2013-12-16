/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao  Guangxian									   *
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
#include "..\..\MCU\INC\STARTUP.H"
#include "..\..\MCU\INC\mb9bf404r.h"
#include "..\..\MCU\INC\WDG_IF.H"
#include "..\..\MCU\INC\FRT_IF.H"
#include "..\..\MCU\INC\I2C_IF.H"
#include "..\..\MCU\INC\EXTI_IF.H"
#include "..\..\MCU\INC\ADC_IF.H"
#include "..\..\MCU\INC\CAP_IF.H"
#include "..\..\MCU\INC\UART_IF.H"
#include "..\..\MCU\INC\LIN_IF.H"
#include "..\..\MCU\INC\SPI_IF.H"
#include "..\..\MCU\INC\MCU_IF.H"
#include "..\..\MCU\INC\DMA_IF.H"
#include "..\..\AUDIO\INC\PA_IF.H"

typedef  void( * const ExecFuncPtr )( void )  __irq ;
extern unsigned int Image$$ARM_LIB_STACKHEAP$$ZI$$Limit;
extern int __main( void );
extern void SysTick_Handler( void );
extern void SVC_Handler( void )	;
extern void PendSV_Handler( void );
extern void WCUT_Handler(void);
extern void CanIsr_0(void);
extern void CanIsr_1(void);
extern void mcu_disable_interrupt( void );
extern void mcu_enable_interrupt( void );
extern void pa_mute(void);
/***********************************************************************
*  Name        : exceptions_area
*  Description : here is the mcu interrupt table vector
*  Parameter   : None
*  Returns     : None
***********************************************************************/
#pragma arm section rodata="exceptions_area"
ExecFuncPtr exceptons_table[] =
{
    ( ExecFuncPtr )&Image$$ARM_LIB_STACKHEAP$$ZI$$Limit,
    ( ExecFuncPtr )Reset_Handler,
    NMI_Handler, //  Non-Maskable Interrupt  | Hardware Watchdog Timer
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    ( ExecFuncPtr )SVC_Handler,
    DebugMon_Handler,
    0,//0
    ( ExecFuncPtr )PendSV_Handler,
    ( ExecFuncPtr )SysTick_Handler,
    Clock_Handler,//DummyHandler,//0: Clock Super Visor
    DummyHandler,//1: Software Watchdog Timer
    DummyHandler,//2: Low Voltage Detector
    DummyHandler,//3: Wave Form Generator / DTIF
    ( ExecFuncPtr )EXTI_Handler4, //4: External Interrupt Request ch.0 to ch.7
    ( ExecFuncPtr )EXTI_Handler5, //5: External Interrupt Request ch.8 to ch.15
    DummyHandler,//6: Dual Timer / Quad Decoder
    CSIO_0_Rx_Handler,//7: MultiFunction Serial ch.0
    CSIO_0_Tx_Handler,//8: MultiFunction Serial ch.0
    CSIO_1_Rx_Handler,//9: MultiFunction Serial ch.1
    CSIO_1_Tx_Handler,// 10: MultiFunction Serial ch.1
    CSIO_2_Rx_Handler,//11: MultiFunction Serial ch.2
    CSIO_2_Tx_Handler,//12: MultiFunction Serial ch.2
    CSIO_3_Rx_Handler,//13: MultiFunction Serial ch.3
    CSIO_3_Tx_Handler,//14: MultiFunction Serial ch.3
    CSIO_4_Rx_Handler,//15: MultiFunction Serial ch.4
    CSIO_4_Tx_Handler,//16: MultiFunction Serial ch.4
    CSIO_5_Rx_Handler,//17: MultiFunction Serial ch.5
    CSIO_5_Tx_Handler,//18: MultiFunction Serial ch.5
    CSIO_6_Rx_Handler,//19: MultiFunction Serial ch.6
    CSIO_6_Tx_Handler,// 20: MultiFunction Serial ch.6
    CSIO_7_Rx_Handler,//21: MultiFunction Serial ch.7
    CSIO_7_Tx_Handler,//22: MultiFunction Serial ch.7
    DummyHandler,//23: PPG
    ( ExecFuncPtr )DummyHandler,//WCUT_Handler,//24: OSC / PLL / Watch Counter
    ( ExecFuncPtr )ADC_Update_Handler, //25: ADC0
    DummyHandler,//26: ADC1
    DummyHandler,//27: ADC2
    ( ExecFuncPtr )FRT_Handler, //28: Free-run Timer
    ( ExecFuncPtr )CAP_Handle_Process, //29: Input Capture
    DummyHandler,//30: Output Compare
    DummyHandler,//31: Base Timer ch.0 to ch.7
#ifdef CAN_BUS_OPTION
    ( ExecFuncPtr )CanIsr_0,//32: CAN ch.0
    ( ExecFuncPtr )CanIsr_1,//33: CAN ch.1
#else
    DummyHandler,//32: CAN ch.0
    DummyHandler,//33: CAN ch.1
#endif/*CAN_BUS_OPTION*/
    DummyHandler,//34: Reserved
    DummyHandler,//35: Reserved
    DummyHandler,//36: Reserved
    DummyHandler,//37: Reserved
    DMA_Ch0_Handler,//38: DMAC ch.0
    DMA_Ch1_Handler,//39: DMAC ch.1
    DMA_Ch2_Handler,//40: DMAC ch.2
    DMA_Ch3_Handler,//41: DMAC ch.3
    DMA_Ch4_Handler,//42: DMAC ch.4
    DMA_Ch5_Handler,//43: DMAC ch.5
    DMA_Ch6_Handler,//44: DMAC ch.6
    DMA_Ch7_Handler,//45: DMAC ch.7
    DummyHandler,//46: Reserved
    DummyHandler//47: Reserved

};
#pragma arm section
/***********************************************************************
*  Name        : Reset_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void Reset_Handler( void )
{
    WATCHDOG_Disable();
    /********************************************************/
    /*	PLL setting											*/
    /********************************************************/
    /*the high clock is enable in bootloader,so we don't need*/
    /*to enable again in here!*/
    //mcu_setup_high_clock();

    mcu_disable_interrupt();
    /********************************************************/
    /* setup Interrupt table address   							*/
    /********************************************************/
#ifdef CHERY_M16_MP5
    mcu_set_irq_table_address(0x00008100);
#else
    mcu_set_irq_table_address(0x00004000);
#endif

    mcu_enable_interrupt();
    /********************************************************/
    /*	All Interrupt setup  							*/
    /********************************************************/
    IRQ_Init_Setup();

    /********************************************************/
    /*	All IO config									   */
    /********************************************************/
    mcu_io_init_reset();

    __main();
}



/***********************************************************************
*  Name        : NMI_Handler
*  Description :No mask interrupt or hardware dog interrupt
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void NMI_Handler( void )
{
    pa_power_off();
    while ( 1 );
}

/***********************************************************************
*  Name        : HardFault_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void HardFault_Handler( void )
{
    while ( 1 );
}

/***********************************************************************
*  Name        : MemManage_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void MemManage_Handler( void )
{
    while ( 1 );
}


/***********************************************************************
*  Name        : BusFault_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void BusFault_Handler( void )
{
    while ( 1 );
}


/***********************************************************************
*  Name        : UsageFault_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  UsageFault_Handler( void )
{
    while ( 1 );
}


__irq void Clock_Handler(void)
{

}
/***********************************************************************
*  Name        : DebugMon_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DebugMon_Handler( void )
{
    while ( 1 );
}


/***********************************************************************
*  Name        : DummyHandler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void DummyHandler( void )
{
    while ( 1 );
}

/***********************************************************************
*  Name        : CSIO_0_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_0_Tx_Handler( void )
{
#ifdef CSIO_0_FOR_I2C
    I2C_Handler( I2C_0 );
#endif
    
#ifdef CSIO_0_FOR_LIN
    LIN_Tx_Handler( LIN_0 );
#endif
    
#ifdef CSIO_0_FOR_UART
    UART_Tx_Handler( UART_0 );
#endif

#ifdef CSIO_0_FOR_SPI
    SPI_Tx_Handler( SPI_0 );
#endif
}

/***********************************************************************
*  Name        : CSIO_0_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_0_Rx_Handler( void )
{
#ifdef CSIO_0_FOR_LIN
    LIN_Rx_Handler( LIN_0 );
#endif
    
#ifdef CSIO_0_FOR_UART
    UART_Rx_Handler( UART_0 );
#endif

#ifdef CSIO_0_FOR_SPI
    SPI_Rx_Handler( SPI_0 );
#endif
}


/***********************************************************************
*  Name        : CSIO_1_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_1_Tx_Handler( void )
{
#ifdef CSIO_1_FOR_I2C
    I2C_Handler( I2C_1 );
#endif
    
#ifdef CSIO_1_FOR_LIN
    LIN_Tx_Handler( LIN_1 );
#endif
    
#ifdef CSIO_1_FOR_UART
    UART_Tx_Handler( UART_1 );
#endif

#ifdef CSIO_1_FOR_SPI
    SPI_Tx_Handler( SPI_1 );
#endif
}

/***********************************************************************
*  Name        : CSIO_1_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_1_Rx_Handler( void )
{
#ifdef CSIO_1_FOR_LIN
    LIN_Rx_Handler( LIN_1 );
#endif
    
#ifdef CSIO_1_FOR_UART
    UART_Rx_Handler( UART_1 );
#endif

#ifdef CSIO_1_FOR_SPI
    SPI_Rx_Handler( SPI_1 );
#endif
}

/***********************************************************************
*  Name        : CSIO_2_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_2_Tx_Handler( void )
{
#ifdef CSIO_2_FOR_I2C
    I2C_Handler( I2C_2 );
#endif
    
#ifdef CSIO_2_FOR_LIN
    LIN_Tx_Handler( LIN_2 );
#endif
    
#ifdef CSIO_2_FOR_UART
    UART_Tx_Handler( UART_2 );
#endif

#ifdef CSIO_2_FOR_SPI
    SPI_Tx_Handler( SPI_2 );
#endif
}

/***********************************************************************
*  Name        : CSIO_2_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_2_Rx_Handler( void )
{
#ifdef CSIO_2_FOR_LIN
    LIN_Rx_Handler( LIN_2 );
#endif
    
#ifdef CSIO_2_FOR_UART
    UART_Rx_Handler( UART_2 );
#endif

#ifdef CSIO_2_FOR_SPI
    SPI_Rx_Handler( SPI_2 );
#endif
}

/***********************************************************************
*  Name        : CSIO_3_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_3_Tx_Handler( void )
{
#ifdef CSIO_3_FOR_I2C
    I2C_Handler( I2C_3 );
#endif
    
#ifdef CSIO_3_FOR_LIN
    LIN_Tx_Handler( LIN_3 );
#endif
    
#ifdef CSIO_3_FOR_UART
    UART_Tx_Handler( UART_3 );
#endif

#ifdef CSIO_3_FOR_SPI
    SPI_Tx_Handler( SPI_3 );
#endif
}

/***********************************************************************
*  Name        : CSIO_3_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_3_Rx_Handler( void )
{
#ifdef CSIO_3_FOR_LIN
    LIN_Rx_Handler( LIN_3 );
#endif
    
#ifdef CSIO_3_FOR_UART
    UART_Rx_Handler( UART_3 );
#endif

#ifdef CSIO_3_FOR_SPI
    SPI_Rx_Handler( SPI_3 );
#endif
}

/***********************************************************************
*  Name        : CSIO_4_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_4_Tx_Handler( void )
{
#ifdef CSIO_4_FOR_I2C
    I2C_Handler( I2C_4 );
#endif
    
#ifdef CSIO_4_FOR_LIN
    LIN_Tx_Handler( LIN_4 );
#endif
    
#ifdef CSIO_4_FOR_UART
    UART_Tx_Handler( UART_4 );
#endif

#ifdef CSIO_4_FOR_SPI
    SPI_Tx_Handler( SPI_4 );
#endif
}

/***********************************************************************
*  Name        : CSIO_4_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_4_Rx_Handler( void )
{
#ifdef CSIO_4_FOR_LIN
    LIN_Rx_Handler( LIN_4 );
#endif
    
#ifdef CSIO_4_FOR_UART
    UART_Rx_Handler( UART_4 );
#endif

#ifdef CSIO_4_FOR_SPI
    SPI_Rx_Handler( SPI_4 );
#endif
}

/***********************************************************************
*  Name        : CSIO_5_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_5_Tx_Handler( void )
{
#ifdef CSIO_5_FOR_I2C
    I2C_Handler( I2C_5 );
#endif
    
#ifdef CSIO_5_FOR_LIN
    LIN_Tx_Handler( LIN_5 );
#endif
    
#ifdef CSIO_5_FOR_UART
    UART_Tx_Handler( UART_5 );
#endif

#ifdef CSIO_5_FOR_SPI
#ifdef CT_G4_IPC
    SPI_FIFOTx_Handler(SPI_5FIFO); //fifo type ciso
#else
    SPI_Tx_Handler( SPI_5 );
#endif //CT_G4_IPC
#endif
}

/***********************************************************************
*  Name        : CSIO_5_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_5_Rx_Handler( void )
{
#ifdef CSIO_5_FOR_LIN
    LIN_Rx_Handler( LIN_5 );
#endif
    
#ifdef CSIO_5_FOR_UART
    UART_Rx_Handler( UART_5 );
#endif

#ifdef CSIO_5_FOR_SPI
#ifdef CT_G4_IPC
    SPI_FIFORx_Handler(SPI_5FIFO); //fifo type of ciso
#else
    SPI_Rx_Handler( SPI_5 );
#endif //CT_G4_IPC
#endif
}

/***********************************************************************
*  Name        : CSIO_6_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_6_Tx_Handler( void )
{
#ifdef CSIO_6_FOR_I2C
    I2C_Handler( I2C_6 );
#endif

#ifdef CSIO_6_FOR_LIN
    LIN_Tx_Handler( LIN_6 );
#endif
    
#ifdef CSIO_6_FOR_UART
    UART_Tx_Handler( UART_6 );
#endif

#ifdef CSIO_6_FOR_SPI
    SPI_Tx_Handler( SPI_6 );
#endif
}

/***********************************************************************
*  Name        : CSIO_6_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_6_Rx_Handler( void )
{
#ifdef CSIO_6_FOR_LIN
    LIN_Rx_Handler( LIN_6 );
#endif
    
#ifdef CSIO_6_FOR_UART
    UART_Rx_Handler( UART_6 );
#endif

#ifdef CSIO_6_FOR_SPI
    SPI_Rx_Handler( SPI_6 );
#endif
}

/***********************************************************************
*  Name        : CSIO_7_Tx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_7_Tx_Handler( void )
{
#ifdef CSIO_7_FOR_I2C
    I2C_Handler( I2C_7 );
#endif

#ifdef CSIO_7_FOR_LIN
    LIN_Tx_Handler( LIN_7 );
#endif
    
#ifdef CSIO_7_FOR_UART
    UART_Tx_Handler( UART_7 );
#endif

#ifdef CSIO_7_FOR_SPI
    SPI_Tx_Handler( SPI_7 );
#endif
}

/***********************************************************************
*  Name        : CSIO_7_Rx_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void CSIO_7_Rx_Handler( void )
{
#ifdef CSIO_7_FOR_LIN
    LIN_Rx_Handler( LIN_7 );
#endif
    
#ifdef CSIO_7_FOR_UART
    UART_Rx_Handler( UART_7 );
#endif

#ifdef CSIO_7_FOR_SPI
    SPI_Rx_Handler( SPI_7 );
#endif
}


/***********************************************************************
*  Name        : DMA_Ch0_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch0_Handler(void)
{
    DMA_Handle(0);
}

/***********************************************************************
*  Name        : DMA_Ch1_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch1_Handler(void)
{
    DMA_Handle(1);
}

/***********************************************************************
*  Name        : DMA_Ch2_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch2_Handler(void)
{
    DMA_Handle(2);
}

/***********************************************************************
*  Name        : DMA_Ch3_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch3_Handler(void)
{
    DMA_Handle(3);
}

/***********************************************************************
*  Name        : DMA_Ch4_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch4_Handler(void)
{
    DMA_Handle(4);
}

/***********************************************************************
*  Name        : DMA_Ch5_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch5_Handler(void)
{
    DMA_Handle(5);
}

/***********************************************************************
*  Name        : DMA_Ch6_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch6_Handler(void)
{
    DMA_Handle(6);
}

/***********************************************************************
*  Name        : DMA_Ch7_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__irq void  DMA_Ch7_Handler(void)
{
    DMA_Handle(7);
}

/**************** END OF FILE *****************************************/

