#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/ICalendar.h>
#include <Source/Times/DateHandling.h>

class Period {
public:
	Period() = default;
	virtual ~Period() = default;

	explicit Period(int num) : num_(num) { }

	int getPeriodNum() { return num_; }

	virtual Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const = 0;
	
protected:
	int num_;
};

class Year : public Period {
public:
	Year() = default;
	explicit Year(int num) : Period(num) { }

	Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const override
	{
		return YearAdd(calendar, date, num_, dc, eom);
	}
};

class Month : public Period {
public:
	Month() = default;
	explicit Month(int num) : Period(num) { }

	Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const override
	{
		return MonthAdd(calendar, date, num_, dc, eom);
	}
};

class Day : public Period {
public:
	Day() = default;
	explicit Day(int num) : Period(num) { }

	Date advance(const ICalendar& calendar, const Date& date, BusinessDayConvention dc, bool eom = false) const override
	{
		return DayAdd(calendar, date, num_, dc);
	}
};