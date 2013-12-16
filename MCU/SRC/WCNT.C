#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\MCU\INC\mb9bf404r.h"
#include "..\INC\IRQ_IF.H"
#include "..\INC\WCNT.H"
#include "..\INC\WCNT_IF.H"
const U8 rtc_month_dates[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
RTC_DATA_TYPE rtc_data;
RTC_DATA_TYPE rtc_mirror_data;
static U8 rtc_update_flag;

/***********************************************************************
*  Name        : WCNT_Init
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WCNT_Init(RTC_DATA_TYPE *rtc)
{
	FM3_WC->CLK_SEL = 0x0000;//select sub osc and 2^12 div clock
	FM3_WC->CLK_EN = 0x01;//clk enable
	FM3_WC->WCRL = 0x08; //1s
	rtc_data.second=rtc->second;
	rtc_data.minute =rtc->minute;
	rtc_data.hour = rtc->hour;
	rtc_data.date =rtc->date;
	rtc_data.month =rtc->month;
	rtc_data.year = rtc->year;
	rtc_update_flag = RTC_UPDATE_ALL;
	FM3_WC->WCCR = 0x82;
	IRQ_Enable(24);
}


/***********************************************************************
*  Name        : WCNT_Read
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WCNT_GetAll(RTC_DATA_TYPE *rtc)
{
	U8 couter = 0;
    WCNT_UpdateRead();
    do{
	    rtc->second = rtc_data.second;
	    rtc->minute = rtc_data.minute;
	    rtc->hour = rtc_data.hour;
	    rtc->date = rtc_data.date;
	    rtc->month = rtc_data.month;
	    rtc->year = rtc_data.year;
        WCNT_UpdateRead();
        couter++;
       }while((rtc->second != rtc_data.second)&&(couter<5));
}

void WCNT_SetALL(RTC_DATA_TYPE *rtc)
{
	if((rtc->second >=60) ||(rtc->minute >= 60) ||(rtc->hour >= 24) ||(rtc->date > 31) || (rtc->month > 12) ||(rtc->year > 100))
	{
		return;
	}
	rtc_data.second = rtc->second;
	rtc_data.minute = rtc->minute;
	rtc_data.hour = rtc->hour;
	rtc_data.date = rtc->date;
	rtc_data.month = rtc->month;
	rtc_data.year = rtc->year;
	rtc_update_flag = RTC_UPDATE_ALL;
}

void WCNT_SetMinute(U8 minute)
{
	if(minute >= 60)
	{
		return;
	}
	rtc_data.minute = minute;
	rtc_update_flag = RTC_UPDATE_MINUTE;
}

void WCNT_SetHour(U8 hour)
{
	if(hour >=24)
	{
		return;
	}
	rtc_data.hour = hour;
	rtc_update_flag = RTC_UPDATE_HOUR;
}

void WCNT_SetDate(U8 date)
{
	WCNT_UpdateRead();
	if(rtc_data.month != 2)
	{
		if(date<=rtc_month_dates[rtc_data.month-1])
		{
			rtc_data.date = date;
		}
	}
	else//  month2
	{
		if(rtc_data.year%4==0)
		{
			if(date>29)
			{
				return;
			}
		}
		else
		{
			if(date>28)
			{
				return;
			}
		}
		rtc_data.date = date;
	}
	rtc_update_flag = RTC_UPDATE_DATE;
}

void WCNT_SetMonth(U8 month)
{
	if(month>12)
	{
		return;
	}
	rtc_data.month = month;
	rtc_update_flag = RTC_UPDATE_MONTH;
}

void WCNT_SetYear(U8 year)
{
	if(year>100)
	{
		return;
	}
	rtc_data.year = year;
	rtc_update_flag = RTC_UPDATE_YEAR;
}
/***********************************************************************
*  Name        : WCNT_UpdateRead
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
static void WCNT_UpdateRead(void)
{
	rtc_data.second = rtc_mirror_data.second;
	rtc_data.minute = rtc_mirror_data.minute;
	rtc_data.hour = rtc_mirror_data.hour;
	rtc_data.date = rtc_mirror_data.date;
	rtc_data.month = rtc_mirror_data.month;
	rtc_data.year = rtc_mirror_data.year;
}



/***********************************************************************
*  Name        : WCUT_Handler
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WCUT_Handler(void)
{
	if(FM3_INTREQ->IRQ24MON & 0x10)
	{
		bFM3_WC_WCCR_WCIF = 0x00;
		if(rtc_update_flag != RTC_UPDATE_NONE)
		{
			switch(rtc_update_flag)
			{
			case RTC_UPDATE_ALL:
				rtc_update_flag = RTC_UPDATE_NONE;
				rtc_mirror_data.second = rtc_data.second;
				rtc_mirror_data.minute = rtc_data.minute;
				rtc_mirror_data.hour = rtc_data.hour;
				rtc_mirror_data.date = rtc_data.date;
				rtc_mirror_data.month = rtc_data.month;
				rtc_mirror_data.year = rtc_data.year;
				break;
			case RTC_UPDATE_MINUTE:
				rtc_mirror_data.minute = rtc_data.minute;
				break;
			case RTC_UPDATE_HOUR:
				rtc_mirror_data.hour = rtc_data.hour;
				break;
			case RTC_UPDATE_DATE:
				rtc_mirror_data.date = rtc_data.date;
				break;
			case RTC_UPDATE_MONTH:
				rtc_mirror_data.month = rtc_data.month;
				break;
			case RTC_UPDATE_YEAR:
				rtc_mirror_data.year = rtc_data.year;
				break;
			default:
				break;
			}
            rtc_update_flag = RTC_UPDATE_NONE;
		}
		/*   counting below   */
		if(++rtc_mirror_data.second == 60)//second
		{
			rtc_mirror_data.minute++;
			rtc_mirror_data.second = 0;
		}
		else
		{
			return;
		}
		if(rtc_mirror_data.minute == 60)//minute
		{
			rtc_mirror_data.hour++;
			rtc_mirror_data.minute = 0;
		}
		else
		{
			return;
		}
		if(rtc_mirror_data.hour == 24)//hour
		{
			rtc_mirror_data.date++;
			rtc_mirror_data.hour = 0;
		}
		else
		{
			return;
		}
		if(rtc_mirror_data.date >=rtc_month_dates[rtc_mirror_data.month-1])//date
		{
			if(rtc_mirror_data.month!=2 )
			{
				rtc_mirror_data.month++;
				rtc_mirror_data.date = 0;
			}
			else
			{
				if(rtc_mirror_data.year%4!=0)
				{
					rtc_mirror_data.month++;
					rtc_mirror_data.date = 0;
				}
			}


		}
		else
		{
			return;
		}

		if(rtc_mirror_data.month == 12)//month
		{
			rtc_mirror_data.year++;
			rtc_mirror_data.month = 0;
		}
		else
		{
			return;
		}

	}
}


/***********************************************************************
*  Name        : WCUT_Disable
*  Description :
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void WCUT_Disable(void)
{
	FM3_WC->WCCR &=(U8)~0x80;
	IRQ_Disable(24);
}

