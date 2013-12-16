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
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
#include "..\INC\PWM.H"
#include "..\INC\PWM_IF.H"
#include "..\INC\mb9bf404r.h"
static U32 pwm_period[6];

/***********************************************************************
*  Name        : PWM_Init
*  Description : Setup and Init the PWM
*  Parameter   : pwm_channel can select PWM_0,PWM_1...,PWM5
                 pwm period value = 25ns*period
                 duty can't large than 100
                 eg:PWM_Init(PWM_0,20000,50);
*  Returns     : None
***********************************************************************/
void PWM_Init( PWM_CHANNEL_TYPE pwm_channel, U32 period, U8 duty )
{
    assert_param(IS_PWM_CHANNEL_TYPE(pwm_channel));
    if(duty >100)
    {
       duty = 100;
    } 
    PWM_Cmd( pwm_channel, DISABLE );
    switch( pwm_channel )
    {
    case PWM_0 :
        pwm_period[0] = period;
        FM3_MFT0_WFG->WFSA10 = 0x0c00;
        FM3_MFT_PPG->REVC = 0x00;                    // Don't reverse the output
        FM3_MFT_PPG->PPGC1 = 0x08;                   //div4
        FM3_MFT_PPG->PPGC0 = 0x0C;                  // No interrupt generation, 16 bit operation
        FM3_MFT_PPG->PRL0 = pwm_period[0] * duty / 100;          // Set the High width of PPG
        FM3_MFT_PPG->PRL1 = pwm_period[0] * ( 100 - duty ) / 100 ;      // Set the Low width of PPG
        bFM3_MFT_PPG_GATEC0_EDGE0 = 0x00;           // Start by TRG-Bit, Start low level
        bFM3_MFT_PPG_GATEC0_STRG0 = 0x00;
        break;
    case PWM_1:
        pwm_period[1] = period;
        FM3_MFT0_WFG->WFSA32 = 0x0D00;
        FM3_MFT_PPG->REVC = 0x00;                // Don't reverse the output
        FM3_MFT_PPG->PPGC2 = 0x04;               // No interrupt generation, 16 bit operation
        FM3_MFT_PPG->PRL2 = pwm_period[1] * duty / 100;          // Set the High width of PPG
        FM3_MFT_PPG->PRL3 = pwm_period[1] * ( 100 - duty ) / 100 ;     // Set the Low width of PPG
        bFM3_MFT_PPG_GATEC0_EDGE2 = 0x00;           // Start by TRG-Bit, Start low level
        bFM3_MFT_PPG_GATEC0_STRG2 = 0x00;
        break;
    case PWM_2:
        pwm_period[2] = period;
        FM3_MFT0_WFG->WFSA54 = 0x0E00;
        FM3_MFT_PPG->REVC = 0x00;                // Don't reverse the output
        FM3_MFT_PPG->PPGC4 = 0x04;               // No interrupt generation, 16 bit operation
        FM3_MFT_PPG->PRL4 = pwm_period[2] * duty / 100;          // Set the High width of PPG
        FM3_MFT_PPG->PRL5 = pwm_period[2] * ( 100 - duty ) / 100 ;     // Set the Low width of PPG
        bFM3_MFT_PPG_GATEC4_EDGE4 = 0x00;           // Start by TRG-Bit, Start low level
        bFM3_MFT_PPG_GATEC4_STRG4 = 0x00;
        break;
    case PWM_3:
        pwm_period[3] = period;
        FM3_MFT1_WFG->WFSA10 = 0x0c00;
        FM3_MFT_PPG->REVC = 0x00;                    // Don't reverse the output
        FM3_MFT_PPG->PPGC8 = 0x04;                  // No interrupt generation, 16 bit operation
        FM3_MFT_PPG->PRL8 = pwm_period[3] / 100 * duty;         // Set the High width of PPG
        FM3_MFT_PPG->PRL9 = pwm_period[3] * ( 100 - duty ) / 100 ;       // Set the Low width of PPG
        bFM3_MFT_PPG_GATEC8_EDGE8 = 0x00;           // Start by TRG-Bit, Start low level
        bFM3_MFT_PPG_GATEC8_STRG8 = 0x00;
        break;
    case PWM_4:
        pwm_period[4] = period;
        FM3_MFT1_WFG->WFSA32 = 0x0D00;
        FM3_MFT_PPG->REVC = 0x00;                    // Don't reverse the output
        FM3_MFT_PPG->PPGC10 = 0x04;                 // No interrupt generation, 16 bit operation
        FM3_MFT_PPG->PRL10 = pwm_period[4] / 100 * duty;          // Set the High width of PPG
        FM3_MFT_PPG->PRL11 = pwm_period[4] / 100 * ( 100 - duty ) ;       // Set the Low width of PPG
        bFM3_MFT_PPG_GATEC8_EDGE10 = 0x00;          // Start by TRG-Bit, Start low level
        bFM3_MFT_PPG_GATEC8_STRG10 = 0x00;
        break;
    case PWM_5:
        pwm_period[5] = period;
        FM3_MFT1_WFG->WFSA54 = 0x0E00;
        FM3_MFT_PPG->REVC = 0x00;                // Don't reverse the output
        FM3_MFT_PPG->PPGC12 = 0x04;               // No interrupt generation, 16 bit operation
        FM3_MFT_PPG->PRL12 = pwm_period[5] / 100 * duty;         // Set the High width of PPG
        FM3_MFT_PPG->PRL13 = pwm_period[5] * ( 100 - duty ) / 100 ;       // Set the Low width of PPG
        bFM3_MFT_PPG_GATEC12_EDGE12 = 0x00;          // Start by TRG-Bit, Start low level
        bFM3_MFT_PPG_GATEC12_STRG12 = 0x00;
        break;
    default:
        break;
    }

}

/***********************************************************************
*  Name        : PWM_Cmd
*  Description : Enable one channel of PWMs
*  Parameter   : pwm_channel can select PWM_0,PWM_1...,PWM5
                 state can be ENABLE or DISABLE
*  Returns     : None
***********************************************************************/
void PWM_Cmd( PWM_CHANNEL_TYPE pwm_channel, FUNCTION_STATE state )
{
    //U32 pwm_trg_bit;
    assert_param(IS_PWM_CHANNEL_TYPE(pwm_channel));
    //pwm_trg_bit = PPG_TRG_BASE + pwm_channel * 4;
    if( state != DISABLE )
    {
        FM3_MFT_PPG->TRG |= 0x3<<pwm_channel;
       //*( ( volatile  U32 * )pwm_trg_bit ) = 0x01;
       //*( ( volatile  U32 * )( pwm_trg_bit + 4 ) ) = 0x01;
    }
    else
    {
        FM3_MFT_PPG->TRG &= ~(0x3<<pwm_channel);
        //*( ( volatile U32 * )pwm_trg_bit ) = 0x00;
        //*( ( volatile U32 *)( pwm_trg_bit + 4 ) ) = 0x00;
    }
}

/***********************************************************************
*  Name        : PWM_SetDuty
*  Description : SET one channel'S Duty
*  Parameter   : pwm_channel can select PWM_0,PWM_1...,PWM5
                 duty can be 1 to 100
*  Returns     : None
***********************************************************************/
void PWM_SetDuty( PWM_CHANNEL_TYPE pwm_channel, U8 duty )
{
    assert_param(IS_PWM_CHANNEL_TYPE(pwm_channel));
    if(duty >100)
    {
        return ;
    }    
    switch( pwm_channel )
    {
    case PWM_0:
        FM3_MFT_PPG->PRL0 = pwm_period[0] * duty / 100;
        FM3_MFT_PPG->PRL1 = pwm_period[0] * ( 100 - duty ) / 100 ;
        break;
    case PWM_1:
        FM3_MFT_PPG->PRL2 = pwm_period[1] * duty / 100;          // Set the High width of PPG
        FM3_MFT_PPG->PRL3 = pwm_period[1] * ( 100 - duty ) / 100 ;
        break;
    case PWM_2:
        FM3_MFT_PPG->PRL4 = pwm_period[2] * duty / 100;          // Set the High width of PPG
        FM3_MFT_PPG->PRL5 = pwm_period[2] * ( 100 - duty ) / 100 ;
        break;
    case PWM_3:
        FM3_MFT_PPG->PRL8 = pwm_period[3] * duty/ 100 ;         // Set the High width of PPG
        FM3_MFT_PPG->PRL9 = pwm_period[3] * ( 100 - duty ) / 100 ;
        break;
    case PWM_4:
        FM3_MFT_PPG->PRL10 = pwm_period[4] * duty / 100 ;          // Set the High width of PPG
        FM3_MFT_PPG->PRL11 = pwm_period[4] * ( 100 - duty )/ 100  ;
        break;
    case PWM_5:
        FM3_MFT_PPG->PRL12 = pwm_period[5] * duty/ 100 ;         // Set the High width of PPG
        FM3_MFT_PPG->PRL13 = pwm_period[5] * ( 100 - duty ) / 100 ;
        break;
    default:
        break;

    }

}
/**************** END OF FILE *****************************************/
