// Jalali.cpp

#include "StdAfx.h"
#include "Jalali.h"

CJalali::CJalali(void)
{
	szJalaliDays[0] = TEXT("شنبه");
	szJalaliDays[1] = TEXT("یه شنبه");
	szJalaliDays[2] = TEXT("دو شنبه");
	szJalaliDays[3] = TEXT("سه شنبه");
	szJalaliDays[4] = TEXT("چهار شنبه");
	szJalaliDays[5] = TEXT("پنج شنبه");
	szJalaliDays[6] = TEXT("جمعه");

	szJalalimonth[0] = TEXT("ژانبیه");
	szJalalimonth[1] = TEXT("فوریه");
	szJalalimonth[2] = TEXT("مارس");
	szJalalimonth[3] = TEXT("آوریل");
	szJalalimonth[4] = TEXT("مه");
	szJalalimonth[5] = TEXT("ژوئن");
	szJalalimonth[6] = TEXT("ژوئیه");
	szJalalimonth[7] = TEXT("اوت");
	szJalalimonth[8] = TEXT("سپتامبر");
	szJalalimonth[9] = TEXT("اکتبر");
	szJalalimonth[10] = TEXT("نوامبر");
	szJalalimonth[11] = TEXT("دسانبر");

	szGregorianDays[0] = TEXT("Saturday");
	szGregorianDays[1] = TEXT("Sunday");
	szGregorianDays[2] = TEXT("Monday");
	szGregorianDays[3] = TEXT("Tuesday");
	szGregorianDays[4] = TEXT("Wednesday");
	szGregorianDays[5] = TEXT("Thursday");
	szGregorianDays[6] = TEXT("Friday");

	szGregorianmonth[0] = TEXT("January");
	szGregorianmonth[1] = TEXT("February");
	szGregorianmonth[2] = TEXT("March");
	szGregorianmonth[3] = TEXT("April");
	szGregorianmonth[4] = TEXT("May");
	szGregorianmonth[5] = TEXT("June");
	szGregorianmonth[6] = TEXT("July");
	szGregorianmonth[7] = TEXT("August");
	szGregorianmonth[8] = TEXT("September");
	szGregorianmonth[9] = TEXT("October");
	szGregorianmonth[10] = TEXT("November");
	szGregorianmonth[11] = TEXT("December");
}

CJalali::~CJalali(void)
{
}

void CJalali::GregorianToJalali(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay, int *JDayOfWeek /* = NULL */, TCHAR *JDayName /* = NULL */, TCHAR *JMonthName /* = NULL */)
{
	int TotalDays;

	TotalDays = GetGregorianOffset(GYear, GMonth, GDay);
	
	if (JDayOfWeek)
	{
		*JDayOfWeek = (TotalDays + 5) % 7;
	}
	
	if (JDayName)
	{
		lstrcpy(JDayName, szJalaliDays[(TotalDays + 5) % 7]);
	}

	*JYear = TotalDays / (7 * 1461 + 4 * 365 + 366);

	if ((*JYear) * (7 * 1461 + 4 * 365 + 366) == TotalDays)
	{
		(*JYear)--;
	}

	*JDay = TotalDays - (*JYear) * (7 * 1461 + 4 * 365 + 366);
	(*JYear) *= 33;
	(*JYear) += 1337;

	if ((*JDay) > 366)
	{
		(*JYear)++;
		(*JDay) -= 366;
	}

	if ((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}

	if ((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}

	if ((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}

	if ((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}

	while ((*JDay) > 366)
	{
		(*JYear)++;
		(*JDay) -= 366;

		if ((*JDay) > 365)
		{
			(*JYear)++;
			(*JDay) -= 365;
		}

		if ((*JDay) > 365)
		{
			(*JYear)++;
			(*JDay) -= 365;
		}

		if ((*JDay) > 365)
		{
			(*JYear)++;
			(*JDay) -= 365;
		}
	}

	if ((*JDay) < 187)
	{
		(*JMonth) = (*JDay) / 31;
		(*JDay) -= (*JMonth) * 31;

		if (!(*JDay))
		{
			(*JMonth)--;
			(*JDay) = 31;
		}
	}
	else
	{
		(*JDay) -= 186;
		(*JMonth) = (*JDay) / 30;
		(*JDay) -= (*JMonth) * 30;

		if (!(*JDay))
		{
			(*JMonth)--;
			(*JDay) = 30;
		}

		(*JMonth) += 6;
	}

	(*JMonth)++;

	if (JMonthName)
	{
		lstrcpy(JMonthName, szJalalimonth[(*JMonth) - 1]);
	}
}

void CJalali::GetJalaliDate(SYSTEMTIME st, int *Year, int *Month, int *Day, int *DayOfWeek /* = NULL */, TCHAR *DayName /* = NULL */, TCHAR *MonthName /* = NULL */)
{
	GregorianToJalali(st.wYear, st.wMonth, st.wDay, Year, Month, Day, DayOfWeek, DayName, MonthName);
}

void CJalali::JalaliToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay, int *GDayOfWeek /* = NULL */, TCHAR *GDayName /* = NULL */, TCHAR *GMonthName /* = NULL */)
{
	int TotalDays;

	TotalDays = GetJalaliOffset(JYear, JMonth, JDay);

	if (GDayOfWeek)
	{
		*GDayOfWeek = (TotalDays) % 7;
	}

	if (GDayName)
	{
		lstrcpy(GDayName, szGregorianDays[(TotalDays) % 7]);
	}

	*GYear = TotalDays / 1461;

	if ((*GYear) * 1461 == TotalDays)
	{
		(*GYear)--;
	}

	*GDay = TotalDays - (*GYear) * 1461;
	(*GYear) *= 4;
	(*GYear)++;

	if ((*GDay) > 365)
	{
		(*GYear)++;
		(*GDay) -= 365;
	}

	if ((*GDay) > 365)
	{
		(*GYear)++;
		(*GDay) -= 365;
	}

	if ((*GDay) > 365)
	{
		(*GYear)++;
		(*GDay) -= 365;
	}

	*GMonth = 1;

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;

		if ((*GDay) > (((*GYear) % 4)? 28 : 29))
		{
			(*GMonth)++;
			(*GDay) -= (((*GYear) % 4)? 28 : 29);
		}
	}

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;

		if ((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;

		if ((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
	}

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;

		if ((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;

		if ((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}

	if ((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
	}

	if (GMonthName)
	{
		lstrcpy(GMonthName, szGregorianmonth[(*GMonth) - 1]);
	}
}

void CJalali::GetJalaliDayOfWeek(int JYear, int JMonth, int JDay, int *DayOfWeek, TCHAR *szDayName /* = NULL*/)
{
	int TotalDays;

	TotalDays = GetJalaliOffset(JYear, JMonth, JDay);

	if (DayOfWeek)
	{
		*DayOfWeek = (TotalDays) % 7;
	}

	if (szDayName)
	{
		lstrcpy(szDayName, szJalaliDays[(TotalDays) % 7]);
	}
}

void CJalali::GetGregorianDayOfWeek(int GYear, int GMonth, int GDay, int *DayOfWeek, TCHAR *szDayName /* = NULL */)
{
	int TotalDays;

	TotalDays = GetGregorianOffset(GYear, GMonth, GDay);

	if (DayOfWeek)
	{
		*DayOfWeek = (TotalDays + 5) % 7;
	}

	if (szDayName)
	{
		lstrcpy(szDayName, szGregorianDays[(TotalDays + 5) % 7]);
	}
}

int CJalali::GetGregorianOffset(int GYear, int GMonth, int GDay)
{
	int TotalDays;
	int i, j;

	i = GYear / 4;

	if (i * 4 == GYear)
	{
		i--;
	}

	TotalDays = i * 1461;
	i *= 4;

	if (GYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	if (GYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	if (GYear > i + 1)
	{
		TotalDays += 365;
	}

	i = (GYear - 2000) / 100 - (GYear - 2000) / 400;
	TotalDays -= i;

	switch (GMonth)
	{
	case 1:
		i = 0;
		break;
	case 2:
	case 3:
		i = 1;
		break;
	case 4:
	case 5:
		i = 2;
		break;
	case 6:
	case 7:
		i = 3;
		break;
	case 8:
		i = 4;
		break;
	case 9:
	case 10:
		i = 5;
		break;
	case 11:
	case 12:
		i = 6;
	}

	switch (GMonth)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		j = 0;
		break;
	case 5:
	case 6:
		j = 1;
		break;
	case 7:
	case 8:
	case 9:
		j = 2;
		break;
	case 10:
	case 11:
		j = 3;
		break;
	case 12:
		j = 4;
	}

	TotalDays += GDay + i * 31 + j * 30;

	if (GMonth > 2)
	{
		if (GYear % 4)
		{
			TotalDays += 28;
		}
		else
		{
			TotalDays += 29;
		}
	}

	TotalDays -= (726926 - (7 * 1461 + 4 * 365 + 366));

	return TotalDays;
}

int CJalali::GetJalaliOffset(int JYear, int JMonth, int JDay)
{
	int TotalDays;
	int i, j;
	
	TotalDays = 726926 - (7 * 1461 + 4 * 365 + 366);
	i = (JYear - 1337) / 33;
	TotalDays += (7 * 1461 + 4 * 365 + 366) * i;
	i = i * 33 + 1336;

	if (JYear > i + 1)
	{
		i++;
		TotalDays += 366;
	}

	if (JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	if (JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	if (JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	if (JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	while (JYear > i + 1)
	{
		if (JYear > i + 1)
		{
			i++;
			TotalDays += 366;
		}

		if (JYear > i + 1)
		{
			i++;
			TotalDays += 365;
		}

		if (JYear > i + 1)
		{
			i++;
			TotalDays += 365;
		}

		if (JYear > i + 1)
		{
			i++;
			TotalDays += 365;
		}
	}

	i = min(JMonth - 1, 6);
	j = max(JMonth - 7, 0);
	TotalDays += JDay + i * 31 + j * 30;

	return TotalDays;
}

int CJalali::GetGregorianDayDifference(int GYear1, int GMonth1, int GDay1, int GYear2, int GMonth2, int GDay2)
{
	int Offset1, Offset2;

	Offset1 = GetGregorianOffset(GYear1, GMonth1, GDay1);
	Offset2 = GetGregorianOffset(GYear2, GMonth2, GDay2);

	return Offset2 - Offset1;
}

int CJalali::GetJalaliDayDifference(int GYear1, int GMonth1, int GDay1, int GYear2, int GMonth2, int GDay2)
{
	int Offset1, Offset2;

	Offset1 = GetJalaliOffset(GYear1, GMonth1, GDay1);
	Offset2 = GetJalaliOffset(GYear2, GMonth2, GDay2);

	return Offset2 - Offset1;
}

void CJalali::GetJalaliDateWithOffset(int JYear, int JMonth, int JDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek /* = NULL */, TCHAR *DayName /* = NULL */, TCHAR *MonthName /* = NULL */)
{
	int TotalDays;

	TotalDays = GetJalaliOffset(JYear, JMonth, JDay) + Offset - 726926 + (7 * 1461 + 4 * 365 + 366);

	if (DayOfWeek)
	{
		*DayOfWeek = (TotalDays + 5) % 7;
	}

	if (DayName)
	{
		lstrcpy(DayName, szJalaliDays[(TotalDays + 5) % 7]);
	}

	*Year = TotalDays / (7 * 1461 + 4 * 365 + 366);

	if ((*Year) * (7 * 1461 + 4 * 365 + 366) == TotalDays)
	{
		(*Year)--;
	}

	*Day = TotalDays - (*Year) * (7 * 1461 + 4 * 365 + 366);
	(*Year) *= 33;
	(*Year) += 1337;

	if ((*Day) > 366)
	{
		(*Year)++;
		(*Day) -= 366;
	}

	if ((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}

	if ((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}

	if ((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}

	if ((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}

	while ((*Day) > 366)
	{
		(*Year)++;
		(*Day) -= 366;

		if ((*Day) > 365)
		{
			(*Year)++;
			(*Day) -= 365;
		}

		if ((*Day) > 365)
		{
			(*Year)++;
			(*Day) -= 365;
		}

		if ((*Day) > 365)
		{
			(*Year)++;
			(*Day) -= 365;
		}
	}

	if ((*Day) < 187)
	{
		(*Month) = (*Day) / 31;
		(*Day) -= (*Month) * 31;

		if (!(*Day))
		{
			(*Month)--;
			(*Day) = 31;
		}
	}
	else
	{
		(*Day) -= 186;
		(*Month) = (*Day) / 30;
		(*Day) -= (*Month) * 30;

		if (!(*Day))
		{
			(*Month)--;
			(*Day) = 30;
		}

		(*Month) += 6;
	}

	(*Month)++;

	if (MonthName)
	{
		lstrcpy(MonthName, szJalalimonth[(*Month) - 1]);
	}
}

void CJalali::GetGregorianDateWithOffset(int GYear, int GMonth, int GDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek /* = NULL */, TCHAR *DayName /* = NULL */, TCHAR *MonthName /* = NULL */)
{
	GregorianToJalali(GYear, GMonth, GDay, &GYear, &GMonth, &GDay);
	GetJalaliDateWithOffset(GYear, GMonth, GDay, Offset, &GYear, &GMonth, &GDay);
	JalaliToGregorian(GYear, GMonth, GDay, Year, Month, Day, DayOfWeek, DayName, MonthName);
}
