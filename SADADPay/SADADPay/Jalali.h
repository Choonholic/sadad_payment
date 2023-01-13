// Jalali.h

/*
[Introduction]
This article is presented to all programmers, especially Iranian ones who want to use Jalali calendar in their applications. I have designed the class CJalaliCalendar, by which you can get the current Jalali date, determine the time interval between two dates, get the day name, and more.

With the provided class, there is no need to deal with the leap year problems of the Gregorian and Jalali calendars. All currently adopted methods for two calendars are considered here.

I have tried to add any necessary function related to Jalali calendar in this class and I hope this helps you.


[Background]
Let me tell you some facts about Jalali and Gregorian calendars. As you may know, it takes 365.2421898 days (non-integer value) for the earth to orbit the sun. We should take this into account. Gregorian and Jalali calendars have different solutions to this problem.

According to the article, Leap Years by Chris Maunder and LeRoy Baxter, the rule for the Gregorian calendar is:

Most years divisible by 4 are Leap Years (i.e. 1996 was).
However, most years divisible by 100 are NOT! (1900 was not).
Unless they are also divisible by 400 (2000 will be).
The provided class works for the years above 1900. For the years below that, my code produces unexpected results.

For the Jalali calendar, we can say the statements below:

Jalali calendar is cyclic with the period of 33 years. each cycle starts at the year 33k + 23 (i.e. 1376).
The first 28 years of each cycle are seven 4-year subcycles with the forth year of each subcycle being a leap year.
The last 5 years of the cycle are composed of 4 ordinary years and a fifth year being a leap year.
The above rules for the Jalali calendar are adopted since the year 1304. And in fact, the code I've written works only for the years above 1337 Shamsi. For the years below 1337, unexpected results will arise.


[Using the Code]
In each method, the J and G prefixes to the arguments show whether the argument is a Jalali date variable or a Gregorian one. Make sure that the system time/date is correct.
GetJalaliDate computes the current Jalali date and gives the year, month, day, day of week, day name and month name (both in Farsi).
JalaliToGregorian and GregorianToJalali convert the date format between the two calendar types.
GetJalaliDayOfWeek and GetGregorianDayOfWeek give the English/Farsi day name of the specified date.
GetGregorianDayDifference and GetJalaliDayDifference return the time difference in days between two dates.
GetJalaliDateWithOffset and GetGregorianDateWithOffset compute the date before/after the specified date. The offset is in the Offset argument, which can be positive or negative.
The variables szJalaliDays, szJalalimonth, szGregorianDays and szGregorianmonth contain the day and month names of both calendar types, which may be useful for you.
*/

#pragma once

class CJalali
{
public:
	CJalali(void);
	~CJalali(void);

	void GetJalaliDate(SYSTEMTIME st, int *Year, int *Month, int *Day, int *DayOfWeek = NULL, TCHAR *DayName = NULL, TCHAR *MonthName = NULL);
	
	void GregorianToJalali(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay, int *JDayOfWeek = NULL, TCHAR *JDayName = NULL, TCHAR *JMonthName = NULL);
	void JalaliToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay, int *GDayOfWeek = NULL, TCHAR *GDayName = NULL, TCHAR *GMonthName = NULL);

	void GetJalaliDayOfWeek(int JYear, int JMonth, int JDay, int *DayOfWeek, TCHAR *szDayName = NULL);
	void GetGregorianDayOfWeek(int GYear, int GMonth, int GDay, int *DayOfWeek, TCHAR *szDayName = NULL);

	int GetGregorianDayDifference(int GYear1, int GMonth1, int GDay1, int GYear2, int GMonth2, int GDay2);
	int GetJalaliDayDifference(int JYear1, int JMonth1, int JDay1, int JYear2, int JMonth2, int JDay2);

	void GetJalaliDateWithOffset(int JYear, int JMonth, int JDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek = NULL, TCHAR *DayName = NULL, TCHAR *MonthName = NULL);
	void GetGregorianDateWithOffset(int GYear, int GMonth, int GDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek = NULL, TCHAR *DayName = NULL, TCHAR *MonthName = NULL);

	const TCHAR *szJalaliDays[7], *szJalalimonth[12];
	const TCHAR *szGregorianDays[7], *szGregorianmonth[12];

private:
	int GetGregorianOffset(int GYear, int GMonth, int GDay);
	int GetJalaliOffset(int JYear, int JMonth, int JDay);
};
