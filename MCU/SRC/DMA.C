/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : DMA.C
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:  0.1
* Date: 2013-9-22
* Author: Shao Guangxian
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\IRQ_IF.H"
#include "..\INC\EVENT_IF.H"
#include "..\INC\mb9bf404r.h"
#include "..\INC\DMA.H"
#include "..\INC\DMA_IF.H"
#include "..\INC\SPI_IF.H"

U8 dma_status[8];
U32 dma_req_flag[8];
/***********************************************************************
*  Name        : void DMA_Config
*  Description :
*  Parameter   : 
*  Returns     : None 
***********************************************************************/
void DMA_Config(DMA_TYPE * dma)
{
    dma_status[dma->ch]= 0;
    if((FM3_DMAC->DMACR&DMA_ENABLE)!= DMA_ENABLE)
    {
        FM3_DMAC->DMACR = DMA_ENABLE;
    }        
    *((int *)(DMACSA_BASE + dma->ch*0x10)) = dma->s_addr;
    *((int *)(DMACDA_BASE + dma->ch*0x10)) = dma->d_addr;
    if((dma->opt&DMA_MODE_HWT) == DMA_MODE_HWT)
    {
        *((int *)(DMACB_BASE + dma->ch*0x10)) |= (DEMAND_TRANSFER_MOD|DMACB_FD|DMACB_RD |0x180000) ;
    }
    else if((dma->opt&DMA_MODE_HWR) == DMA_MODE_HWR)
    {
        *((int *)(DMACB_BASE + dma->ch*0x10)) |= (DEMAND_TRANSFER_MOD|DMACB_FS|DMACB_RS |0x180000);
    }    
    else
    {
        *((int *)(DMACB_BASE + dma->ch*0x10)) = BLOCK_TRANSFER_MOD |0x180000;//software
    }
    *((int *)(DMACA_BASE + dma->ch*0x10)) = ((U32)(dma->opt &0x3F)<<23)|(dma->len-1)|DMACA_EB ;
    dma_req_flag[dma->ch] = (U32)1<<(dma->opt &0x1F);
    FM3_INTREQ->DRQSEL |= dma_req_flag[dma->ch];
    IRQ_Enable(38+dma->ch);
}

/***********************************************************************
*  Name        : U8 DMA_CheckStatus
*  Description :
*  Parameter   : 
*  Returns     : None 
***********************************************************************/
U8 DMA_CheckStatus(U8 ch)
{
   return dma_status[ch]; 
}

/***********************************************************************
*  Name        : void DMA_Handle
*  Description :
*  Parameter   : 
*  Returns     : None 
***********************************************************************/
void DMA_Handle(U8 ch)
{
    dma_status[ch] = (*(int *)(DMACB_BASE+ch*0x10)>>16)&0x7;
    FM3_INTREQ->DRQSEL &= (U32)~dma_req_flag[ch];//
    *(int *)(DMACB_BASE+ch*0x10) &= 0xFFF8FFFF;// clear interrupt flag
    if( NULL != mcu_dma_callback[( U8 )( ch)] )
        mcu_dma_callback[( U8 )( ch)]();
}
