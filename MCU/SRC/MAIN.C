/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :            										   *
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

/***************************** System include files *******************/
#include <RTL.H>
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
/*---Public include files---------------------------------------------*/
#include "..\..\OS\INC\TASK_IF.H"
#include "..\..\MCU\INC\MCU_IF.H"
#include "..\..\POWER\INC\POWER_DRV.H"
#include "..\..\POWER\INC\POWER_DRV_IF.H"

/*---Private include files--------------------------------------------*/

/*===VARIABLES========================================================*/

/*---Global-----------------------------------------------------------*/

/*---Private----------------------------------------------------------*/



/*===FUNCTIONS========================================================*/

/*---Global-----------------------------------------------------------*/

/***********************************************************************
*  Name        : main
*  Description : startup code
*  Parameter   : None
*  Returns     : int
***********************************************************************/
int main(void)
{  
   pw_check_mcu_reset_source();
   pw_eeprom_iic_reset_create();

    /*if the voltage lower than 6.5v,we go to stop mode directly*/
    if(!PW_CHECK_6V5_VOLTAGE()) //below than 6.5v, enter stop mode
    {
        pw_mcu_entry_stop_mode();
    }
    else	
    {
        os_sys_init(task_init_power);
    }
    for(;;)
    {
        mcu_sw_reset();
    }  
}



/*---Private----------------------------------------------------------*/



/**************** END OF FILE *****************************************/
