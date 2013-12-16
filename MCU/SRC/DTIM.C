/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian   							   		               *
************************************************************************
* Object        : DTIM.C
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:  0.1
* Date: 2011-12-31
* Author: Sgx
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\DTIM.H"
#include "..\INC\DTIM_IF.H"
#include "..\INC\IRQ_IF.H"
#include "..\INC\EVENT_IF.H"
#include "..\INC\mb9bf404r.h"
/***********************************************************************
*  Name        : DTIM_Config
*  Description : This method setup one timer
*  Parameter   : dtim,period,options
*  Returns     : Nothing
***********************************************************************/
void DTIM_Config( DTIM_TYPE *dtim, U32 period, U8 options )
{
    assert_param(IS_DTIM_TYPE(dtim));
    dtim->INTCR = 0x00;//clear interrupt flag
    dtim->CRLR = options;//set control options
    dtim->INTMR = 0x01;//default set
    dtim->RLD = period;
}


/***********************************************************************
*  Name        : DTIM_Cmd
*  Description : This method enable or disable one timer
*  Parameter   : dtim,period,options
*  Returns     : Nothing
***********************************************************************/
void DTIM_Cmd( DTIM_TYPE *dtim, FUNCTION_STATE state )
{
    assert_param(IS_DTIM_TYPE(dtim));
    assert_param(IS_FUNCTION_STATE(state));
    if( state != DISABLE )
    {
        dtim->CRLR |= 0x80;
        IRQ_Enable( 6 );
    }
    else
    {
        dtim->CRLR &= ( U8 )~0x80;
        IRQ_Disable( 6 );
    }
}

/***********************************************************************
*  Name        : DTIM_SetPeriod
*  Description : This method enable or disable one timer
*  Parameter   : dtim,period,options
*  Returns     : Nothing
***********************************************************************/
void DTIM_SetPeriod( DTIM_TYPE *dtim, U32 period )
{
    assert_param(IS_DTIM_TYPE(dtim));
    dtim->BRLD = period;
}

/***********************************************************************
*  Name        : DTIM_Hander
*  Description : This function just for IRQ only
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void DTIM_Hander( void )
{
    U16 flag = FM3_INTREQ->IRQ06MON;
    if( flag & 0x01 ) //if DTIM_1 interrupt
    {
        if( mcu_dtim_callback[0] != NULL )
            mcu_dtim_callback[0]();
        DTIM_1->INTCR = 0x00;//clear interrupt flag
    }
    if( flag & 0x02 ) //if DTIM_2 interrupt
    {
        if( mcu_dtim_callback[1] != NULL )
            mcu_dtim_callback[1]();
        DTIM_2->INTCR = 0x00;//clear interrupt flag
    }
}

/**************** END OF FILE *****************************************/



