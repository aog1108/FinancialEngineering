#include <Source/Times/DateHandling.h>

Date GetGoodDay(const ICalendar& calendar, const Date& d, BusinessDayConvention dc)
{
	if (dc == Unadjusted)
		return d;
	else {
		Date ret = d;
		boost::gregorian::days day_unit;

		if (dc == Following || dc == ModifiedFollowing)
			day_unit = boost::gregorian::days(1);
		else if (dc == Preceding || dc == ModifiedPreceding)
			day_unit = boost::gregorian::days(-1);

		while (calendar.isHoliday(ret))
			ret += day_unit;

		if (dc == ModifiedFollowing || dc == ModifiedPreceding) {
			if (ret.month() != d.month()) {
				ret = d;
				while (calendar.isHoliday(ret))
					ret -= day_unit;
			}
		}

		return ret;
	}
}

//Unadjusted를 제외하고 Convention에 따라서 함수 행태가 차이나진 않음.
//BusinessDayAdd 기능이기 때문.
Date DayAdd(const ICalendar& calendar, const Date& d, int k, BusinessDayConvention dc)
{
	if (dc == Unadjusted)
		return d + boost::gregorian::days(k);
	else {
		int count = 0;
		Date ret = d;
		boost::gregorian::days day_unit;
		if (k >= 0)
			day_unit = boost::gregorian::days(1);
		else
			day_unit = boost::gregorian::days(-1);

		while (count != abs(k)) {
			ret += day_unit;
			if (calendar.isBusinessDay(ret))
				++count;
		}
		return ret;
	}
}

Date MonthAdd(const ICalendar& calendar, const Date& d, int k, BusinessDayConvention dc, bool eom)
{
	boost::gregorian::months month_unit(k);
	Date ret = d;

	ret += month_unit;
	if (eom && calendar.isEndOfMonth(d)) {
		if (dc == Unadjusted)
			return d.end_of_month();
		else
			return EndOfMonth(calendar, ret);
	}
	else {
		ret = GetGoodDay(calendar, ret, dc);
		return ret;
	}
}

Date YearAdd(const ICalendar& calendar, const Date& d, int k, BusinessDayConvention dc, bool eom)
{
	boost::gregorian::years year_unit(k);
	Date ret = d;

	ret += year_unit;
	if (eom && calendar.isEndOfMonth(d)) {
		if (dc == Unadjusted)
			return d.end_of_month();
		else
			return EndOfMonth(calendar, ret);
	}
	else {
		ret = GetGoodDay(calendar, ret, dc);
		return ret;
	}
}

Date EndOfMonth(const ICalendar& calendar, const Date& d)
{
	return GetGoodDay(calendar, d.end_of_month(), Preceding);
}