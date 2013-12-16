/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : WATCHDOG.h
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version: 0.1
* Date: 2011-12-02
* Author: Sgx
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*
*=====================================================================*/
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\INC\WDG.H"
#include "..\INC\WDG_IF.H"
#include "..\INC\mb9bf404r.h"



/***********************************************************************
*  Name        : WATCHDOG_Enable
*  Description : Enable the WATCHDOG
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WATCHDOG_Enable( void )
{
    WATCHDOG->WDG_LCK = ACCESS_KEY;
    WATCHDOG->WDG_LDR = WATCHDOG_LOAD;
    WATCHDOG->WDG_LCK = ACCESS_KEY;
    WATCHDOG->WDG_LCK = REVERSAL_ACCESS_KEY;
    WATCHDOG->WDG_CTL = WDG_CTL_RESEN_BIT | WDG_CTL_INTEN_BIT;/*watchdoe reset enalbe and enable interrupt first time*/
}


/***********************************************************************
*  Name        : WATCHDOG_Disable
*  Description : Disable the WATCHDOG
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WATCHDOG_Disable( void )
{
    WATCHDOG->WDG_LCK = ACCESS_KEY;
    WATCHDOG->WDG_LCK = REVERSAL_ACCESS_KEY;
    WATCHDOG->WDG_CTL = 0x00;
}

/***********************************************************************
*  Name        : WATCHDOG_Clear
*  Description : Clear the WATCHDOG
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WATCHDOG_Clear( void )
{
    WATCHDOG->WDG_LCK = ACCESS_KEY;
    WATCHDOG->WDG_ICL = ARBITRARY_VALUE;
    WATCHDOG->WDG_ICL = REVERSAL_VALUE;
}
/****************************** END OF FILE ***************************/
