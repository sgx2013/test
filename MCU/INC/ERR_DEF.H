/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        : ERROR_DEF.h
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
#ifndef _ERROR_DEF_H_
#define _ERROR_DEF_H_

/**** Definition of constants ****/


/**** Definition of types ****/


/**** Definition of macros ****/
typedef enum
{
	ERR_OK ,                       /* OK */
	ERR_SPEED,                     /* This device does not work in the active speed mode. */
	ERR_RANGE,                     /* Parameter out of range. */
	ERR_VALUE ,                    /* Parameter of incorrect value. */
	ERR_OVERFLOW,                  /* Timer overflow. */
	ERR_MATH ,                     /* Overflow during evaluation. */
	ERR_ENABLED ,                  /* Device is enabled. */
	ERR_DISABLED ,                 /* Device is disabled. */
	ERR_BUSY ,                     /* Device is busy. */
	ERR_NOTAVAIL,                  /* Requested value or method not available. */
	ERR_RXEMPTY,                  /* No data in receiver. */
	ERR_TXFULL,                   /* Transmitter is full. */
	ERR_BUSOFF,                  /* Bus not available. */
	ERR_OVERRUN ,                 /* Overrun error is detected. */
	ERR_FRAMING ,                 /* Framing error is detected. */
	ERR_PARITY ,                  /* Parity error is detected. */
	ERR_NOISE ,                   /* Noise error is detected. */
	ERR_IDLE ,                    /* Idle error is detected. */
	ERR_FAULT ,                   /* Fault error is detected. */
	ERR_BREAK ,                   /* Break char is received during communication. */
	ERR_CRC ,                     /* CRC error is detected. */
	ERR_ARBITR,                  /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
	ERR_PROTECT ,                 /* Protection error is detected. */
	ERR_UNDERFLOW ,               /* Underflow error is detected. */
	ERR_UNDERRUN ,                /* Underrun error is detected. */
	ERR_COMMON ,                  /* Common error of a device. */
	ERR_LINSYNC,                  /* LIN synchronization error is detected. */
	ERR_FAILED ,                  /* Requested functionality or process failed. */
} ERR_TYPE;
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/



#endif /*_ERROR_DEF_H_*/

/****************************** END OF FILE ***************************/

