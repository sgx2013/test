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
#ifndef _EXTI_H_
#define _EXTI_H_
#include "mb9bf404r.h"
/**** Definition of constants ****/


/**** Definition of types ****/


/**** Definition of macros ****/
#define EXTI_BASE           0x42600000
#define EXTI_ENIR_BASE      EXTI_BASE
#define EXTI_ENRR_BASE      EXTI_BASE+0x80
#define EXTI_EICL_BASE      EXTI_BASE+0x100
#define EXTI_ELVR_A_BASE    EXTI_BASE+0x180
#define EXTI_ELVR_B_BASE    EXTI_BASE+0x184
#define EXTI                FM3_EXTI

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#endif /* _EXTI_H_ */
/****************************** END OF FILE ***************************/
