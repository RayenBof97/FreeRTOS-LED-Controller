#include "main.h"



void show_time_date_itm(void) //Display time and date through ITM (Make sure to enable SWV for this)
{
	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;

	memset(&rtc_date,0,sizeof(rtc_date));
	memset(&rtc_time,0,sizeof(rtc_time));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";


	printf("%02d:%02d:%02d [%s]",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format); //You have to modify the syscall of _write to send chars to ITM
	printf("\t%02d-%02d-%2d\n",rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);
}


void show_time_date(void)
{
	static char showtime[40];   //String that will contain the time
	static char showdate[40];	//String that will contain the date

	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;

	static char *time = showtime;
	static char *date = showdate;

	memset(&rtc_date,0,sizeof(rtc_date));  //ensures that all fields in the structure start with a known state (in this case, zero).
	memset(&rtc_time,0,sizeof(rtc_time));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);  //Get the Time and date into our declared RTC Typedef
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";   	//We are checking if we have an AM or PM time

	/* Display time Format : hh:mm:ss [AM/PM] */
	sprintf((char*)showtime,"%s:\t%02d:%02d:%02d [%s]","\n Time&Date",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	xQueueSend(printQueue,&time,portMAX_DELAY);

	/* Display date Format : date-month-year */
	sprintf((char*)showdate,"\t%02d-%02d-%2d\n",rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);
	xQueueSend(printQueue,&date,portMAX_DELAY);
}

/*
 * Configures the RTC time with the provided settings (from user)
 * Ensures 12-hour AM format with no daylight saving or store operation adjustments.
 */
void rtc_configure_time(RTC_TimeTypeDef *time)
{

	time->TimeFormat = RTC_HOURFORMAT12_AM;
	time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	time->StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&hrtc,time,RTC_FORMAT_BIN);
}

/*
 * Configures the RTC date with the provided settings (from user)
 */
void rtc_configure_date(RTC_DateTypeDef *date)
{
	HAL_RTC_SetDate(&hrtc,date,RTC_FORMAT_BIN);
}

/*
 * This function is used to validate the provided time and date
 */
int validate_rtc_information(RTC_TimeTypeDef *time , RTC_DateTypeDef *date)
{
	if(time){
		if( (time->Hours > 12) || (time->Minutes > 59) || (time->Seconds > 59) )
			return 1;
	}

	if(date){
		if( (date->Date > 31) || (date->WeekDay > 7) || (date->Year > 99) || (date->Month > 12) )
			return 1;
	}

	return 0;
}
