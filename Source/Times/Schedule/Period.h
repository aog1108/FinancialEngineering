#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/ICalendar.h>
#include <Source/Times/DateHandling.h>

class Period {
public:
	Period() = default;
	virtual ~Period() = default;

	Period(int num) : num_(num) { }

	virtual Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const = 0;

protected:
	int num_;
};

class Year : public Period {
public:
	Year() = default;
	Year(int num) : Period(num) { }

	Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const
	{
		return YearAdd(calendar, date, num_, dc, eom);
	}
};

class Month : public Period {
public:
	Month() = default;
	Month(int num) : Period(num) { }

	Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const
	{
		return MonthAdd(calendar, date, num_, dc, eom);
	}
};

class Day: public Period {
public:
	Day() = default;
	Day(int num) : Period(num) { }

	Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const
	{
		return DayAdd(calendar, date, num_, dc);
	}
};