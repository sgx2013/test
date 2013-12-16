/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : ADC.C
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
#include "..\INC\ADC_IF.H"
#include "..\..\SYSTEM\INC\SYS_ASSERT_IF.H"
volatile static U8 conv_flag;
volatile static U16 adc_ch_value_array[16]=
{
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF, 
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,    
};

/***********************************************************************
*  Name        : ADC_Init
*  Description :
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void ADC_Init( void )
{
    FM3_ADC0->SCIS23 = 0x00;
    FM3_ADC0->SCIS01 = ADC_IN_CHANNEL; //only the low 16 channel are available
    FM3_ADC0->ADST1 = 0x6F;
    FM3_ADC0->ADST0 = 0x6F; /*257/80M=3.2us */
    FM3_ADC0->ADSS23 = 0x00;
    FM3_ADC0->ADSS01 = 0x00; //all channel select the ADST0 setup
    FM3_ADC0->CMPCR = 0x00; //no compare
    FM3_ADC0->ADCEN = 0x01; //adc enable
    while( 0x03 != FM3_ADC0->ADCEN );
    FM3_ADC0->SFNS = ADC_CHANNEL_COUNT - 1; // channel count
    FM3_ADC0->ADSR = 0x00;
    FM3_ADC0->ADCR = 0x08;
    FM3_ADC0->SCCR = 0x10; //No START
    IRQ_Enable( 25 );

}


/***********************************************************************
*  Name        : ADC_Cmd
*  Description :
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void ADC_Cmd( FUNCTION_STATE state )
{
    assert_param(IS_FUNCTION_STATE(state));
    if( state != DISABLE )
    {
        FM3_ADC0->ADCEN = 0x1;
        while( 0x03 != FM3_ADC0->ADCEN );
    }
    else//disable
    {
        FM3_ADC0->ADCEN = 0x0;
    }

}

/***********************************************************************
*  Name        : ADC_GetValue
*  Description :
*  Parameter   : adc_ch can select ADC_0,ADC_1,...ADC_15

*  Returns     : None
***********************************************************************/
U8 ADC_GetValue( ADC_TYPE adc_ch, U16* val )
{
    assert_param(IS_RANGE(adc_ch,ADC_0,ADC_15) );
	*val = adc_ch_value_array[adc_ch];		
    //if(conv_flag==0)//invailed
    {
        //    return ERR_NOTAVAIL;
    }
    
    return ERR_OK;
}


/***********************************************************************
*  Name        : ADC_Conversion
*  Description :
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void  ADC_Conversion( void )
{
	  if(FM3_ADC0->ADSR&0x01)//if adc conversion is in progress
		{
				return ;
		}
    if( 0x03 != FM3_ADC0->ADCEN )
    {
        ADC_Cmd( ENABLE );
    }
    conv_flag = 0x00;
    if((FM3_ADC0->ADSR&0x01)!=0x01)//If no in conversion progress
    {
      FM3_ADC0->SCCR = 0x01; //start conversion  
    }    
    
}
/***********************************************************************
*  Name        : ADC_Update_Handler
*  Description : this function just for IRQ only!
*  Parameter   : None

*  Returns     : None
***********************************************************************/
void ADC_Update_Handler( void )
{
    U8 i;
    U32 scfd_value;
    i = ( FM3_ADC0->SFNS & 0x0F ) + 1; //Get the channel count
    while( i > 0 )
    {
        scfd_value = FM3_ADC0->SCFD;
        adc_ch_value_array[scfd_value & 0x0F] = ( U16 )( scfd_value >> 20 );
        i--;
    }
    bFM3_ADC0_ADCR_SCIF = 0x0;
    conv_flag = 0xFF; //means has conversion complete
}
/**************** END OF FILE *****************************************/

