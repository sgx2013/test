/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : FRT.C
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
#include "..\INC\FRT.H"
#include "..\INC\FRT_IF.H"
#include "..\INC\mb9bf404r.h"
#include "..\INC\IRQ_IF.H"
#include "..\INC\EVENT_IF.H"
/***********************************************************************
*  Name        : FRT_Init
*  Description : init the free-run-timer
*  Parameter   : frt-> FRT_MFT_0,FRT_MFT_1
                 frt_unit -> FRT_UNIT_0,FRT_UNIT_1,FRT_UNIT_2
                 options->FRT_IRQZE|FRT_ICRE|FRT_PCLK_DIV1...FRT_PCLK_DIV256
*  Returns     : None
***********************************************************************/
void FRT_Init( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, U16 options )
{
    assert_param(IS_FRT_MFT_TYPE( frt ));
    assert_param(IS_RANGE(frt_unit,FRT_UNIT_0,FRT_UNIT_2) );
    switch( frt_unit )
    {
    case FRT_UNIT_0:
        frt->TCSA0 = FRT_STOP | FRT_UP_MODE | FRT_SCLR | FRT_BFE | FRT_ICRE | options;
        break;
    case FRT_UNIT_1:
        frt->TCSA1 = FRT_STOP | FRT_UP_MODE | FRT_SCLR | FRT_BFE | FRT_ICRE | options;
        break;
    case FRT_UNIT_2:
        frt->TCSA2 = FRT_STOP | FRT_UP_MODE | FRT_SCLR | FRT_BFE | FRT_ICRE | options;
        break;
    default:
				return;
        //break;
    }
    IRQ_Enable( 28 );
}

/***********************************************************************
*  Name        : FRT_Isr_Cmd
*  Description : enable or disable  interrupt of the  free-run-timer
*  Parameter   : frt-> FRT_MFT_0,FRT_MFT_1
                 frt_unit -> FRT_UNIT_0,FRT_UNIT_1,FRT_UNIT_2
                 state->ENABLE,DISABLE
*  Returns     : None
***********************************************************************/
void FRT_Isr_Cmd( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, FUNCTION_STATE state )
{
    assert_param(IS_FRT_MFT_TYPE( frt ));
    assert_param(IS_RANGE(frt_unit,FRT_UNIT_0,FRT_UNIT_2) );
    assert_param(IS_FUNCTION_STATE(state));
    if( state != DISABLE )
    {
        switch( frt_unit )
        {
        case FRT_UNIT_0:
            frt->TCSA0 |= FRT_ICRE;
            break;
        case FRT_UNIT_1:
            frt->TCSA1 |= FRT_ICRE;
            break;
        case FRT_UNIT_2:
            frt->TCSA2 |= FRT_ICRE;
            break;
        default:
            break;
        }
    }
    else
    {
        switch( frt_unit )
        {
        case FRT_UNIT_0:
            frt->TCSA0 &= ~ FRT_ICRE;
            break;
        case FRT_UNIT_1:
            frt->TCSA1 &= ~ FRT_ICRE;
            break;
        case FRT_UNIT_2:
            frt->TCSA2 &= ~ FRT_ICRE;
            break;
        default:
            break;
        }
    }

}
/***********************************************************************
*  Name        : FRT_SetPeriod
*  Description : set the free-run-timer's period
*  Parameter   : frt-> FRT_MFT_0,FRT_MFT_1
                 frt_unit -> FRT_UNIT_0,FRT_UNIT_1,FRT_UNIT_2

*  Returns     : None
***********************************************************************/
void FRT_SetPeriod( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, U16 period )
{
    assert_param(IS_FRT_MFT_TYPE( frt ));
    assert_param(IS_RANGE(frt_unit,FRT_UNIT_0,FRT_UNIT_2) );
    switch( frt_unit )
    {
    case FRT_UNIT_0:
        frt->TCCP0 = period ;
        break;
    case FRT_UNIT_1:
        frt->TCCP1 = period;
        break;
    case FRT_UNIT_2:
        frt->TCCP2 = period;
        break;
    default:
        break;
    }
}

/***********************************************************************
*  Name        : FRT_Cmd
*  Description : make the FRT run or stop
*  Parameter   : frt-> FRT_MFT_0,FRT_MFT_1
                 frt_unit -> FRT_UNIT_0,FRT_UNIT_1,FRT_UNIT_2
                 state can be START or STOP
*  Returns     : None
***********************************************************************/
void FRT_Cmd( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit, FRT_STATE_TYPE state )
{
    assert_param(IS_FRT_MFT_TYPE( frt ));
    assert_param(IS_RANGE(frt_unit,FRT_UNIT_0,FRT_UNIT_2) );
    assert_param(IS_RANGE(state,FRT_STATE_START,FRT_STATE_STOP) );
    if( state == FRT_STATE_STOP )
    {
        switch( frt_unit )
        {
        case FRT_UNIT_0:
            frt->TCSA0 |= ( U16 )FRT_STOP | FRT_SCLR;
            break;
        case FRT_UNIT_1:
            frt->TCSA1 |= ( U16 )FRT_STOP | FRT_SCLR;
            break;
        case FRT_UNIT_2:
            frt->TCSA2 |= ( U16 )FRT_STOP | FRT_SCLR;
            break;
        default:
            break;
        }
    }
    else if( state == FRT_STATE_START )
    {
        switch( frt_unit )
        {
        case FRT_UNIT_0:
            frt->TCSA0 &= ~( U16 )FRT_STOP;
            break;
        case FRT_UNIT_1:
            frt->TCSA1 &= ~( U16 )FRT_STOP;
            break;
        case FRT_UNIT_2:
            frt->TCSA2 &= ~( U16 )FRT_STOP;
            break;
        default:
            break;
        }
    }

}

/***********************************************************************
*  Name        : FRT_Clr_Flag
*  Description : Clear one unit of free-run-timer irq's flag
*  Parameter   : frt-> FRT_MFT_0,FRT_MFT_1
                 frt_unit -> FRT_UNIT_0,FRT_UNIT_1,FRT_UNIT_2

*  Returns     : None
***********************************************************************/

void FRT_Clr_Flag( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit ) //JUST FOR CLR IQR FLAG
{
    assert_param(IS_FRT_MFT_TYPE( frt ));
    assert_param(IS_RANGE(frt_unit,FRT_UNIT_0,FRT_UNIT_2) );
    switch( frt_unit )
    {
    case FRT_UNIT_0:
        frt->TCSA0 &= ~( U16 )( FRT_IRQZF | FRT_ICLR );
        break;
    case FRT_UNIT_1:
        frt->TCSA1  &= ~( U16 )( FRT_IRQZF | FRT_ICLR );
        break;
    case FRT_UNIT_2:
        frt->TCSA2  &= ~( U16 )( FRT_IRQZF | FRT_ICLR );
        break;
    default:
        break;
    }
}

/***********************************************************************
*  Name        : FRT_GetCounter
*  Description : get the couter value of the  free-run-timer
*  Parameter   : frt-> FRT_MFT_0,FRT_MFT_1
                 frt_unit -> FRT_UNIT_0,FRT_UNIT_1,FRT_UNIT_2

*  Returns     : None
***********************************************************************/
U16 FRT_GetCounter( FRT_MFT_TYPE* frt, FRT_UNIT_TYPE frt_unit )
{
    U16 couter = 0;
    assert_param(IS_FRT_MFT_TYPE( frt ));
    assert_param(IS_RANGE(frt_unit,FRT_UNIT_0,FRT_UNIT_2) );
    switch( frt_unit )
    {
    case FRT_UNIT_0:
        couter = frt->TCDT0; //default unit0
        break;
    case FRT_UNIT_1:
        couter = frt->TCDT1;
        break;
    case FRT_UNIT_2:
        couter = frt->TCDT2;
        break;
    default:
        break;
    }
    return couter;
}
/***********************************************************************
*  Name        : FRT_Handler
*  Description : this function just for IRQ only!
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void FRT_Handler( void )
{
    U16 flag = FM3_INTREQ->IRQ28MON;
    if( flag & 0x9 ) //MFT0_timer0
    {
        bFM3_ADC0_SCCR_SSTR = 0x01; //ADC0_START
        if( mcu_frt_callback[0] != NULL )
            mcu_frt_callback[0]();
        FRT_Clr_Flag( FRT_MFT_0, FRT_UNIT_0 ) ;
    }

    if( flag & 0x12 ) ////MFT0_timer1
    {
        if( mcu_frt_callback[1] != NULL )
            mcu_frt_callback[1]();
        FRT_Clr_Flag( FRT_MFT_0, FRT_UNIT_1 ) ;
    }

    if( flag & 0x24 ) //MFT0_timer2
    {
        if( mcu_frt_callback[2] != NULL )
            mcu_frt_callback[2]();
        FRT_Clr_Flag( FRT_MFT_0, FRT_UNIT_2 ) ;
    }

    if( flag & 0x240 ) //MFT1_timer0
    {
        if( mcu_frt_callback[3] != NULL )
            mcu_frt_callback[3]();
        FRT_Clr_Flag( FRT_MFT_1, FRT_UNIT_0 ) ;
    }

    if( flag & 0x480 ) //MFT1_timer1
    {
        if( mcu_frt_callback[4] != NULL )
            mcu_frt_callback[4]();
        FRT_Clr_Flag( FRT_MFT_1, FRT_UNIT_1 ) ;
    }

    if( flag & 0x900 ) //MFT1_timer2
    {
        if( mcu_frt_callback[5] != NULL )
            mcu_frt_callback[5]();
        FRT_Clr_Flag( FRT_MFT_1, FRT_UNIT_2 ) ;
    }
}
/****************************** END OF FILE ***************************/

