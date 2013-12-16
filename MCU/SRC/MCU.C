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
* Version       Date    Name    Changes and comment__DSB__DSBs
*
*=====================================================================*/
/****  include files ****/

#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\MCU.H"
#include "..\INC\MCU_IF.H"
#include "..\INC\CortexM3.h"
#include "..\INC\mb9bf404r.h"
#include "..\INC\IRQ_IF.H"
#include "..\INC\PIN_MAP.h"
#include PROJECT_MCU_CONFIG_FILE
#include <core_cm3.h>
#include <core_cmInstr.h>



#if 0
/***********************************************************************
*  Name        : WFI
*  Description : make mcu to sleep
*  Parameter   :
*  Returns     : None
***********************************************************************/
__asm static void WFI( void )
{
    WFI
    BX LR

}
#endif

volatile U8 mcu_to_reset_flag = 0;

void mcu_reset_flag_set(void)
{
	mcu_to_reset_flag = 0x01;
}
/***********************************************************************
*  Name        : mcu_setup_high_clock
*  Description : setup mcu clock in high speed mode,here use the
                 main oscillation and pll circuit cpu->80M
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void mcu_setup_high_clock( void )
{
    FM3_CRG->BSC_PSR   = BSC_DIV_1;  //setup the base clock  frequency division
    FM3_CRG->APBC0_PSR = APB_DIV_4;//setup the apb0 clock  frequency division
    FM3_CRG->APBC1_PSR = APB_DIV_4 | APB_ENABLE_BIT;
    FM3_CRG->APBC2_PSR = APB_DIV_4 | APB_ENABLE_BIT;
    FM3_CRG->TTC_PSR   = TRACE_CLK_DIV_1 ;//setup the TPIUCLK clock  frequency division

    FM3_CRG->CSW_TMR = SUB_CLK_WAIT_TIME | MAIN_CLK_WAIT_TIME; //setup  main and sub oscillation wait time
#ifdef  MCU_RTC_FUNC 
    FM3_CRG->SCM_CTL = SCM_CTL_SOSCE_BIT | SCM_CTL_MOSCE_BIT; //Enable the main and sub oscillation
#else
    FM3_CRG->SCM_CTL = SCM_CTL_MOSCE_BIT; //Enable the main and sub oscillation
#endif       
    while( ( FM3_CRG->SCM_STR & SCM_STR_MORDY_BIT ) != SCM_STR_MORDY_BIT ); //wait for main oscillation stability
    FM3_CRG->PSW_TMR = PLL_CLK_WAIT_TIME ;//setup pll oscillation wait time
    FM3_CRG->PLL_CTL1 = 0x00; //k=1;m=1;
    FM3_CRG->PLL_CTL2 = 0x09; //N=10
    FM3_CRG->SCM_CTL |= SCM_CTL_PLLE_BIT; //enable pll oscillation
    while( ( FM3_CRG->SCM_STR & SCM_STR_PLRDY_BIT ) != SCM_STR_PLRDY_BIT ); //wait for pll oscillation stability
    FM3_CRG->SCM_CTL |= PLL_CLK_MODE; //Set master clock switch to pll mode
    while( ( FM3_CRG->SCM_STR & 0xE0 ) != PLL_CLK_MODE );
    FM3_CRG->CSV_CTL = CSV_CTL_DEF_FCD|CSV_CTL_MCSVE_BIT;//enable main oscillation fail detection
    FM3_CRG->FCSWH_CTL = FCSWH_CTL_DEF_VAL;
    FM3_CRG->FCSWL_CTL = FCSWL_CTL_DEF_VAL;
    FM3_CRG->CSV_CTL |= CSV_CTL_FCSRE_BIT;
    //FM3_CRG->INT_CLR = 0x27;//clear the interrupt flags
    FM3_CRG->CSV_CTL |= CSV_CTL_FCSDE_BIT;
    
    
    //sucess switch to pll clk mode
    FM3_CANPRES->CANPRE = 0x0C;		 //CAN CLOCK = 16MHz (80MHz / 5 = 16MHz)
}


/***********************************************************************
*  Name        : mcu_setup_low_clock
*  Description : setup mcu clock in low speed mode,
*  				 after this function the mcu speen to 100khz
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void mcu_setup_low_clock( void )
{
	FM3_CRG->SCM_CTL = (FM3_CRG->SCM_STR&0x1F)|LC_CLK_MODE;
	while((FM3_CRG->SCM_STR&0xE0) != LC_CLK_MODE);//
	FM3_CRG->BSC_PSR = BSC_DIV_1;//100k-->cpu
	FM3_CRG->APBC0_PSR = APB_DIV_1;
	FM3_CRG->APBC1_PSR = APB_DIV_1;
	FM3_CRG->APBC2_PSR = APB_ENABLE_BIT |APB_DIV_1;//
	FM3_CRG->SCM_CTL &=(U8)~(SCM_CTL_PLLE_BIT|SCM_CTL_MOSCE_BIT);//close pll and main clock
}

/***********************************************************************
*  Name        : mcu_io_init_active
*  Description :
*  Parameter   :
*  Returns     : None
***********************************************************************/
void mcu_io_init_active( void )
{
    /***************************************/
    /* GPIO to be use as I/O of Peripheral */
    /**************************************/
    GPIO->ADE   = ADC_REMAP_VAL ;
    GPIO->PFR0  = GPIO_PFR0_ACTIVE_VAL;
    GPIO->PFR1  = GPIO_PFR1_ACTIVE_VAL ;
    GPIO->PFR2  = GPIO_PFR2_ACTIVE_VAL;
    GPIO->PFR3  = GPIO_PFR3_ACTIVE_VAL;
    GPIO->PFR4  = GPIO_PFR4_ACTIVE_VAL;
    GPIO->PFR5  = GPIO_PFR5_ACTIVE_VAL;
    GPIO->PFR6  = GPIO_PFR6_ACTIVE_VAL;//can here
    GPIO->PFR7  = GPIO_PFR7_ACTIVE_VAL;
    GPIO->PFR8  = GPIO_PFR8_ACTIVE_VAL;//All be GPIO

    /**************************************/
    /*   I/O of Peripheral remap          */
    /**************************************/
    MFS03_REMAP = MFS03_REMAP_VAL;
    MFS47_REMAP = MFS47_REMAP_VAL;
    EXTI_REMAP  = EXTI_REMAP_VAL;
    MFT0_REMAP  = MFT0_REMAP_VAL;
    MFT1_REMAP  = MFT1_REMAP_VAL;
    CAN_ADC_REMAP = CAN_ADC_REMAP_VAL;
    FM3_MFT0_WFG->WFSA10 = 0x0C00;
    FM3_MFT0_WFG->WFSA32 = 0x0D00;
    FM3_MFT0_WFG->WFSA54 = 0x0E00;
    FM3_MFT1_WFG->WFSA10 = 0x0C00;
    FM3_MFT1_WFG->WFSA32 = 0x0D00;
    FM3_MFT1_WFG->WFSA54 = 0x0E00;
    /*********************************************/
    /***     GPIO output Value Config     *******/
    /********************************************/

    GPIO->PDOR0 = GPIO_PDOR0_ACTIVE_VAL; //No gpio to be high lever
    GPIO->PDOR1 = GPIO_PDOR1_ACTIVE_VAL; //No gpio to be high lever
    GPIO->PDOR2 = GPIO_PDOR2_ACTIVE_VAL; //No gpio to be high lever
    GPIO->PDOR3 = GPIO_PDOR3_ACTIVE_VAL;
    GPIO->PDOR4 = GPIO_PDOR4_ACTIVE_VAL; //No gpio to be high lever
    GPIO->PDOR5 = GPIO_PDOR5_ACTIVE_VAL;
    GPIO->PDOR6 = GPIO_PDOR6_ACTIVE_VAL; //No gpio to be high lever
    GPIO->PDOR7 = GPIO_PDOR7_ACTIVE_VAL; //No gpio to be high lever
    GPIO->PDOR8 = GPIO_PDOR8_ACTIVE_VAL; //No gpio to be high lever


    /*********************************************/
    /***     Config GPIO to be output     *******/
    /********************************************/
    GPIO->DDR0 = GPIO_DDR0_ACTIVE_VAL;//out
    GPIO->DDR1 = GPIO_DDR1_ACTIVE_VAL;
    GPIO->DDR2 = GPIO_DDR2_ACTIVE_VAL;
    GPIO->DDR3 = GPIO_DDR3_ACTIVE_VAL;
    GPIO->DDR4 = GPIO_DDR4_ACTIVE_VAL;
    GPIO->DDR5 = GPIO_DDR5_ACTIVE_VAL;
    GPIO->DDR6 = GPIO_DDR6_ACTIVE_VAL;
    GPIO->DDR7 = GPIO_DDR7_ACTIVE_VAL;
    GPIO->DDR8 = GPIO_DDR8_ACTIVE_VAL;

    /*********************************************/
    /*** Config the input GPIO to set PULL up ****/
    /*********************************************/
    GPIO->PCR0 = PORT_ALL_NO_PULL_UP;
    GPIO->PCR1 = PORT_ALL_NO_PULL_UP;
    GPIO->PCR2 = PORT_ALL_NO_PULL_UP;
    GPIO->PCR3 = PORT_ALL_NO_PULL_UP;
#ifndef J36R_HW_B2_1    
    GPIO->PCR4 = 0x600;
#else    
    GPIO->PCR4 = PORT_ALL_NO_PULL_UP;
#endif    
    GPIO->PCR5 = PORT_ALL_NO_PULL_UP;
    GPIO->PCR6 = PORT_ALL_NO_PULL_UP;
    GPIO->PCR7 = PORT_ALL_NO_PULL_UP;
}

/***********************************************************************
*  Name        : mcu_io_init_reset
*  Description :
*  Parameter   :
*  Returns     : None
***********************************************************************/
void mcu_io_init_reset( void )
{

    /***************************************/
    /* GPIO to be use as I/O of Peripheral */
    /**************************************/
    GPIO->ADE   = 0x00;//None pin to be  AD Port
    GPIO->PFR0  = GPIO_PFR0_RESET_VAL;
    GPIO->PFR1  = GPIO_PFR1_RESET_VAL;
    GPIO->PFR2  = GPIO_PFR2_RESET_VAL;
    GPIO->PFR3  = GPIO_PFR3_RESET_VAL;
    GPIO->PFR4  = GPIO_PFR4_RESET_VAL;
    GPIO->PFR5  = GPIO_PFR5_RESET_VAL;
    GPIO->PFR6  = GPIO_PFR6_RESET_VAL;
    GPIO->PFR7  = GPIO_PFR7_RESET_VAL;
    GPIO->PFR8  = GPIO_PFR8_RESET_VAL;

    /**************************************/
    /*   I/O of Peripheral remap          */
    /**************************************/
    MFS03_REMAP = MFS03_REMAP_VAL;
    MFS47_REMAP = MFS47_REMAP_VAL;
    EXTI_REMAP  = EXTI_REMAP_VAL;
    MFT0_REMAP  = MFT0_REMAP_VAL;
    MFT1_REMAP  = MFT1_REMAP_VAL;
    CAN_ADC_REMAP = CAN_ADC_REMAP_VAL;

    /*********************************************/
    /***     GPIO output Value Config     *******/
    /********************************************/
    GPIO->PDOR0 = GPIO_PDOR0_RESET_VAL;
    GPIO->PDOR1 = GPIO_PDOR1_RESET_VAL;
    GPIO->PDOR2 = GPIO_PDOR2_RESET_VAL;
    GPIO->PDOR3 = GPIO_PDOR3_RESET_VAL;
    GPIO->PDOR4 = GPIO_PDOR4_RESET_VAL;
    GPIO->PDOR5 = GPIO_PDOR5_RESET_VAL;
    GPIO->PDOR6 = GPIO_PDOR6_RESET_VAL;
    GPIO->PDOR7 = GPIO_PDOR7_RESET_VAL;
    GPIO->PDOR8 = GPIO_PDOR8_RESET_VAL;
    /*********************************************/
    /***     Config GPIO to be output     *******/
    /********************************************/
    GPIO->DDR0 = GPIO_DDR0_RESET_VAL;
    GPIO->DDR1 = GPIO_DDR1_RESET_VAL;
    GPIO->DDR2 = GPIO_DDR2_RESET_VAL;
    GPIO->DDR3 = GPIO_DDR3_RESET_VAL;
    GPIO->DDR4 = GPIO_DDR4_RESET_VAL ;
    GPIO->DDR5 = GPIO_DDR5_RESET_VAL;
    GPIO->DDR6 = GPIO_DDR6_RESET_VAL;
    GPIO->DDR7 = GPIO_DDR7_RESET_VAL;
    GPIO->DDR8 = GPIO_DDR8_RESET_VAL;

}

/***********************************************************************
*  Name        : mcu_io_init_standby
*  Description :
*  Parameter   :
*  Returns     : None
***********************************************************************/
void mcu_io_init_standby( void )
{
    /***************************************/
    /* GPIO to be use as I/O of Peripheral */
    /**************************************/
    GPIO->ADE   = 0x00;//None pin to be  AD Port
    GPIO->PFR0  = GPIO_PFR0_STANDBY_VAL;
    GPIO->PFR1  = GPIO_PFR1_STANDBY_VAL;
    GPIO->PFR2  = GPIO_PFR2_STANDBY_VAL;
    GPIO->PFR3  = GPIO_PFR3_STANDBY_VAL;
    GPIO->PFR4  = GPIO_PFR4_STANDBY_VAL;
    GPIO->PFR5  = GPIO_PFR5_STANDBY_VAL;
    GPIO->PFR6  = GPIO_PFR6_STANDBY_VAL;
    GPIO->PFR7  = GPIO_PFR7_STANDBY_VAL;
    GPIO->PFR8  = GPIO_PFR8_STANDBY_VAL;

    /**************************************/
    /*   I/O of Peripheral remap          */
    /**************************************/
    MFS03_REMAP = MFS03_REMAP_VAL;
    MFS47_REMAP = MFS47_REMAP_VAL;
    EXTI_REMAP  = EXTI_REMAP_VAL;
    MFT0_REMAP  = MFT0_REMAP_VAL;
    MFT1_REMAP  = MFT1_REMAP_VAL;
    CAN_ADC_REMAP = CAN_ADC_REMAP_VAL;

    /*********************************************/
    /***     GPIO output Value Config     *******/
    /********************************************/
    GPIO->PDOR0 = GPIO_PDOR0_STANDBY_VAL;
    GPIO->PDOR1 = GPIO_PDOR1_STANDBY_VAL;
    GPIO->PDOR2 = GPIO_PDOR2_STANDBY_VAL;
    GPIO->PDOR3 = GPIO_PDOR3_STANDBY_VAL;
    GPIO->PDOR4 = GPIO_PDOR4_STANDBY_VAL;
    GPIO->PDOR5 = GPIO_PDOR5_STANDBY_VAL;
    GPIO->PDOR6 = GPIO_PDOR6_STANDBY_VAL;
    GPIO->PDOR7 = GPIO_PDOR7_STANDBY_VAL;
    GPIO->PDOR8 = GPIO_PDOR8_STANDBY_VAL;
    /*********************************************/
    /***     Config GPIO to be output     *******/
    /********************************************/
    GPIO->DDR0 = GPIO_DDR0_STANDBY_VAL;
    GPIO->DDR1 = GPIO_DDR1_STANDBY_VAL;
    GPIO->DDR2 = GPIO_DDR2_STANDBY_VAL;
    GPIO->DDR3 = GPIO_DDR3_STANDBY_VAL;
    GPIO->DDR4 = GPIO_DDR4_STANDBY_VAL;
    GPIO->DDR5 = GPIO_DDR5_STANDBY_VAL;
    GPIO->DDR6 = GPIO_DDR6_STANDBY_VAL;
    GPIO->DDR7 = GPIO_DDR7_STANDBY_VAL;
    GPIO->DDR8 = GPIO_DDR8_STANDBY_VAL;
}

/***********************************************************************
*  Name        : mcu_disable_all_interrupt
*  Description :
*  Parameter   :
*  Returns     : None
***********************************************************************/
void mcu_disable_all_interrupt(void)
{
	NVIC->ICER[0] = 0xFFFFFFFF;//disable irq 0~31
	NVIC->ICER[1] = 0xFFFFFFFF;//disable irq 32~47
	FM3_EXTI->ENIR = 0x0000;//disable exti interrupt 0~15
}

/***********************************************************************
*  Name        : mcu_sw_reset
*  Description :
*  Parameter   :
*  Returns     : None
***********************************************************************/
void mcu_sw_reset( void )
{
    SCB->AIRCR = SCB_AIRCR_PRIGROUP | 0x04; //bit2:SYSRESETREQ
}

/***********************************************************************
*  Name        : mcu_get_reset_reason
*  Description :this function return a flag to indicate reset reason
*  Parameter   : None
*  Returns     : U16
***********************************************************************/
MCU_RESET_TYPE mcu_get_reset_reason( void )
{
    U16 temp_val = ( U16 )FM3_CRG->RST_STR;
    MCU_RESET_TYPE ret_val = MCU_RESET_BY_ERR;
    if(temp_val&0x100)
    {
        ret_val = MCU_RESET_BY_SW;
    }
    else if(temp_val&0x001)
    {
        ret_val = MCU_RESET_BY_PWR;
    }
    else if(temp_val&0x002)
    {
        ret_val = MCU_RESET_BY_PIN;
    }
    else if(temp_val&0x0F0)
    {
        ret_val = MCU_RESET_BY_ERR;
    }
    return ret_val;
}

/***********************************************************************
*  Name        : mcu_to_stop_mode
*  Description :this function to make mcu to stop mode
*  Parameter   : None
*  Returns     : U16
***********************************************************************/
void mcu_to_stop_mode( void )
{   
#ifdef MCU_RTC_FUNC	
	mcu_setup_low_clock();
    FM3_CRG->STB_CTL = 0x1ACC0000 | 0x00; //timer mode
#else
	FM3_CRG->STB_CTL = 0x1ACC0000 | 0x02; //stop mode
#endif	
	mcu_to_reset_flag = 0;
    SCB->SCR = 0x04; //SLEEPDEEP=1;    
    bFM3_CAN0_CTRLR_INIT = 0x01; //CAN NO OPERATION
    for(;;)
    {
    	if(mcu_to_reset_flag == 0) //for watchcouter timer
    	{
    		__WFI();
    	}
    	else
    	{
    		mcu_sw_reset();
    	}
    }
}

/***********************************************************************
*  Name        : mcu_set_interrupt_table_address
*  Description :
*  Parameter   : None
*  Returns     : U16
***********************************************************************/
void mcu_set_irq_table_address(U32 addr)
{
   assert_param(IS_MCU_IRQ_RIGHT(addr)); 
   VTOR_REGISTER = addr; 
}
/**************** END OF FILE *****************************************/
