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
#include "..\INC\IRQ.H"
#include "..\INC\IRQ_IF.H"
#include "..\INC\CortexM3.h"
#include "..\INC\mb9bf404r.h"
/***********************************************************************
*  Name        : IRQ_Init_Setup
*  Description : setup mcu irq Priority group and irq Priority
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void IRQ_Init_Setup( void )
{
    NVIC->ICER[0] = 0xFFFFFFFF;//disable irq 0~31
    NVIC->ICER[1] = 0xFFFFFFFF;//disable irq 32~47
    /*Handlers Priority group select*/
    SCB->AIRCR = SCB_AIRCR_PRIGROUP;

    /*********************************/
    /* System Handlers Priority Setup*/
    /********************************/
    SCB->SHP[0] = IRQ_LEVEL0_0;//memory fault
    SCB->SHP[1] = IRQ_LEVEL0_0;//bus fault
    SCB->SHP[2] = IRQ_LEVEL0_0;//usage fault
    SCB->SHP[7] = IRQ_LEVEL0_1;//svc
    SCB->SHP[8] = IRQ_LEVEL0_0;//debug monitor
    SCB->SHP[10] = IRQ_LEVEL0_1; //PendSV
    SCB->SHP[11] = IRQ_LEVEL0_2;//SysTick

    /*Enable usage fault& bus fault & memory fault*/
    SCB->SHCSR = USGFAULTENA | BUSFAUTLENA | MEMFAULTENA ;

    /*********************************/
    /* IRQ Priority Setup*/
    /********************************/
    NVIC->IP[0] = IRQ_DEFAULT_LEVEL;//IRQ_0
    NVIC->IP[1] = IRQ_DEFAULT_LEVEL;//IRQ_1
    NVIC->IP[2] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[3] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[4] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[5] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[6] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[7] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[8] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[9] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[10] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[11] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[12] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[13] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[14] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[15] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[16] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[14] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[15] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[16] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[17] = IRQ_LEVEL1_3;
    NVIC->IP[18] = IRQ_LEVEL1_3;
    NVIC->IP[19] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[20] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[21] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[22] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[23] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[24] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[25] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[26] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[27] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[28] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[29] = IRQ_LEVEL1_0;
    NVIC->IP[30] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[31] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[32] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[33] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[34] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[35] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[36] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[37] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[38] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[39] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[40] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[41] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[42] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[43] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[44] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[45] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[46] = IRQ_DEFAULT_LEVEL;
    NVIC->IP[47] = IRQ_DEFAULT_LEVEL; //IRQ_47
}


/***********************************************************************
*  Name        : mcu_enable_interrupt
*  Description : mcu enable interrupt
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__asm void mcu_enable_interrupt( void )
{
    CPSIE i;
    BX  lr;
}


/***********************************************************************
*  Name        : mcu_disable_interrupt
*  Description : mcu disable interrupt
*  Parameter   : None
*  Returns     : None
***********************************************************************/
__asm void mcu_disable_interrupt( void )
{
    CPSID i;
    BX  lr;
}


/***********************************************************************
*  Name        : IRQ_Enable
*  Description : enable one interrupt of user selection
*  Parameter   : irq_num is one of the 48 IRQs
*  Returns     : None
***********************************************************************/
void IRQ_Enable( U8 irq_num )
{
    NVIC->ISER[irq_num >> 5] = ( U32 )0x00000001 << ( irq_num & 0x1F ) ;
}

/***********************************************************************
*  Name        : IRQ_Disable
*  Description : disable one interrupt of user selection
*  Parameter   : irq_num is one of the 48 IRQs
*  Returns     : None
***********************************************************************/
void IRQ_Disable( U8 irq_num )
{
    NVIC->ICER[irq_num >> 5] = ( U32 )0x00000001 << ( irq_num & 0x1F ) ;
}

/***********************************************************************
*  Name        : IRQ_Flag_Clear
*  Description : clear one interrupt  flag of user selection
*  Parameter   : irq_num is one of the 48 IRQs
*  Returns     : None
***********************************************************************/
void IRQ_Flag_Clear( U8 irq_num )
{
    NVIC->ICPR[irq_num >> 5] = ( U32 )0x00000001 << ( irq_num & 0x1F ) ;
}
/**************** END OF FILE *****************************************/
