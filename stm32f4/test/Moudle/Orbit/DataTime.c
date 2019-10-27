/* **************************************************
文件名：    DataTime.c
文件作用：  时间计算文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#include "DataTime.h"

static int daysInMonth[2][13] = {
	//  1   2   3   4   5   6   7   8   9   10  11  12
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
static int cumulDaysInMonth[2][13] = {
	//  1  2   3   4   5    6    7    8    9    10   11   12
	{0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
	{0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
};

/**
	 * Constructor
	 * @param[in] year the year
	 * @param[in] month the month
	 * @param[in] day the day
	 * @param[in] hour the hour
	 * @param[in] minute the minute
	 * @param[in] second the second
	 * @param[in] microsecond the microsecond
	 */
unsigned long DataTime_Initialise(
	int year,
	int month,
	int day,
	int hour,
	int minute,
	int second)
{
	unsigned long tick = 0;

	tick = TimeSpan_CalculateTicks(
		AbsoluteDays_2(year, month, day),
		hour,
		minute,
		second);
	
	return tick;
}

/**
	 * Constructor
	 * @param[in] year the year
	 * @param[in] month the month
	 * @param[in] day the day
	 * @param[in] hour the hour
	 * @param[in] minute the minute
	 * @param[in] second the second
	 * @param[in] microsecond the microsecond
	 */
unsigned long DataTime_Initialise2(
	int year,
	double day)
{
	unsigned long tick = 0;

	tick = AbsoluteDays_1(year, day) * TicksPerDay;

	return tick;
}

/**
	 * Checks whether the given year is valid
	 * @param[in] year the year to check
	 * @returns whether the year is valid
	 */
unsigned char IsValidYear(int year)
{
	unsigned char valid = 1;
	if (year < 1 || year > 9999)
	{
		valid = 0;
	}
	return valid;
}

/**
	 * Find whether a year is a leap year
	 * @param[in] year the year to check
	 * @returns whether the year is a leap year
	 */
unsigned char IsLeapYear(int year)
{
	if (!IsValidYear(year))
	{
		printf("throw 1;\r\n");
		return 0;
	}

	return (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

/**
	 * Check whether the year/month is valid
	 * @param[in] year the year to check
	 * @param[in] month the month to check
	 * @returns whether the year/month is valid
	 */
unsigned char IsValidYearMonth(int year, int month)
{
	unsigned char valid = 1;
	if (IsValidYear(year))
	{
		if (month < 1 || month > 12)
		{
			valid = 0;
		}
	}
	else
	{
		valid = 0;
	}
	return valid;
}

/**
	 * Find the number of days in a month given the year/month
	 * @param[in] year the year
	 * @param[in] month the month
	 * @returns the days in the given month
	 */
int DaysInMonth(int year, int month)
{
	if (!IsValidYearMonth(year, month))
	{
		printf("DaysInMonth throw 1;\r\n");
		return 0;
	}

	const int* daysInMonthPtr;

	if (IsLeapYear(year))
	{
		daysInMonthPtr = daysInMonth[1];
	}
	else
	{
		daysInMonthPtr = daysInMonth[0];
	}

	return daysInMonthPtr[month];
}

/**
	 * Check whether the year/month/day is valid
	 * @param[in] year the year to check
	 * @param[in] month the month to check
	 * @param[in] day the day to check
	 * @returns whether the year/month/day is valid
	 */
unsigned char IsValidYearMonthDay(int year, int month, int day)
{
	unsigned char valid = 1;
	if (IsValidYearMonth(year, month))
	{
		if (day < 1 || day > DaysInMonth(year, month))
		{
			valid = 0;
		}
	}
	else
	{
		valid = 0;
	}
	return valid;
}

/**
	 * Find the day of the year given the year/month/day
	 * @param[in] year the year
	 * @param[in] month the month
	 * @param[in] day the day
	 * @returns the day of the year
	 */
int DayOfYear(int year, int month, int day) 
{
	if (!IsValidYearMonthDay(year, month, day))
	{
		printf("throw 1;\r\n");
		return 0;
	}

	int daysThisYear = day;

	if (IsLeapYear(year))
	{
		daysThisYear += cumulDaysInMonth[1][month];
	}
	else
	{
		daysThisYear += cumulDaysInMonth[0][month];
	}

	return daysThisYear;
}

unsigned long mktime(unsigned int year, unsigned int mon,
	unsigned int day, unsigned int hour,
	unsigned int min, unsigned int sec)
{
	if (0 >= (int)(mon -= 2)) {    /**//* 1..12 -> 11,12,1..10 */
		mon += 12;      /**//* Puts Feb last since it has leap day */
		year -= 1;
	}

	return (((
		(unsigned long)(year / 4 - year / 100 + year / 400 + 367 * mon / 12 + day) +
		year * 365 - 719499
		) * 24 + hour /**//* now have hours */
		) * 60 + min /**//* now have minutes */
		) * 60 + sec; /**//* finally seconds */
}

double AbsoluteDays_1(unsigned int year, double doy) 
{
	unsigned long previousYear = year - 1;

	/*
	 * + days in previous years ignoring leap days
	 * + Julian leap days before this year
	 * - minus prior century years
	 * + plus prior years divisible by 400 days
	 */
	unsigned long daysSoFar = 365 * previousYear
		+ previousYear / 4LL
		- previousYear / 100LL
		+ previousYear / 400LL;

	return (daysSoFar - 719499 + doy - 1.0);
}

int AbsoluteDays_2(int year, int month, int day)
{
	int previousYear = year - 1;

	/*
	 * days this year (0 - ...)
	 * + days in previous years ignoring leap days
	 * + Julian leap days before this year
	 * - minus prior century years
	 * + plus prior years divisible by 400 days
	 */
	int result = DayOfYear(year, month, day) - 719499 - 1
		+ 365 * previousYear
		+ previousYear / 4
		- previousYear / 100
		+ previousYear / 400;

	return result;
}


void FromTicks(int* year, int* month, int* day, unsigned long m_encoded) 
{
	int totalDays = (int)(m_encoded / TicksPerDay);

	/*
	 * number of 400 year cycles
	 */
	int num400 = totalDays / 146097;
	totalDays -= num400 * 146097;
	/*
	 * number of 100 year cycles
	 */
	int num100 = totalDays / 36524;
	if (num100 == 4)
	{
		/*
		 * last day of the last leap century
		 */
		num100 = 3;
	}
	totalDays -= num100 * 36524;
	/*
	 * number of 4 year cycles
	 */
	int num4 = totalDays / 1461;
	totalDays -= num4 * 1461;
	/*
	 * number of years
	 */
	int num1 = totalDays / 365;
	if (num1 == 4)
	{
		/*
		 * last day of the last leap olympiad
		 */
		num1 = 3;
	}
	totalDays -= num1 * 365;

	/*
	 * find year
	 */
	*year = (num400 * 400) + (num100 * 100) + (num4 * 4) + num1 + 1;

	/*
	 * convert day of year to month/day
	 */
	int x;
	if (IsLeapYear(*year))
	{
		x = 1;
	}
	else
	{
		x = 0;
	}

	*month = 1;
	while (totalDays >= (daysInMonth[x][*month]) && *month <= 12)
	{
		totalDays -= daysInMonth[x][*month];
		*month = *month + 1;
	}

	*day = totalDays + 1;
}

int Year(unsigned long t) 
{
	int year;
	int month;
	int day;
	FromTicks(&year, &month, &day, t);
	return year;
}

