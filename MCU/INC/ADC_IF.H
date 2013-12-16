/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : ADC.H
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
#ifndef _ADC_H_
#define _ADC_H_

#include "mb9bf404r.h"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\CONFIG\INC\CONFIG.H"
#include "ERR_DEF.H"
#include "IRQ_IF.H"
#include "PIN_MAP.h"
#include  PROJECT_MCU_CONFIG_FILE
/**** Definition of constants ****/


/**** Definition of types ****/
typedef enum
{
    ADC_0 = 0x0,
    ADC_1 = 0x01,
    ADC_2 = 0x02,
    ADC_3 = 0x03,
    ADC_4 = 0x04,
    ADC_5 = 0x05,
    ADC_6 = 0x06,
    ADC_7 = 0x07,
    ADC_8 = 0x08,
    ADC_9 = 0x09,
    ADC_10 = 0x0A,
    ADC_11 = 0x0B,
    ADC_12 = 0x0C,
    ADC_13 = 0x0D,
    ADC_14 = 0x0E,
    ADC_15 = 0x0F
} ADC_TYPE;


/**** Definition of macros ****/
#define   ADC_IN_CHANNEL    (ADC_REMAP_VAL)
#define   ADC_CHANNEL_COUNT  (((ADC_IN_CHANNEL>>15)&0x1)+((ADC_IN_CHANNEL>>14)&0x1)+((ADC_IN_CHANNEL>>13)&0x1)+((ADC_IN_CHANNEL>>12)&0x1)+\
                               ((ADC_IN_CHANNEL>>11)&0x1)+((ADC_IN_CHANNEL>>10)&0x1)+((ADC_IN_CHANNEL>>9)&0x1)+((ADC_IN_CHANNEL>>8)&0x1)+\
                               ((ADC_IN_CHANNEL>>7)&0x1)+((ADC_IN_CHANNEL>>6)&0x1)+((ADC_IN_CHANNEL>>5)&0x1)+((ADC_IN_CHANNEL>>4)&0x1)+\
                               ((ADC_IN_CHANNEL>>3)&0x1)+((ADC_IN_CHANNEL>>2)&0x1)+((ADC_IN_CHANNEL>>1)&0x1)+(ADC_IN_CHANNEL&0x1))
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void ADC_Init( void );
extern void ADC_Cmd( FUNCTION_STATE state );
extern  U8  ADC_GetValue( ADC_TYPE adc_ch, U16* val );
extern void ADC_Conversion( void );
extern void ADC_Update_Handler( void );
#endif /*_ADC_H_*/
