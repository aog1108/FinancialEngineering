#include <Source/Times/DateHandling.h>

const Date GetGoodDay(const ICalendar& calendar, const Date& d, BusinessDayConvention dc)
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

//DayAdd의 경우 Unadjusted를 제외하고 Convention에 따라서 함수 행태가 차이나진 않음.
//사용자는 MonthAdd, YearAdd와는 동작하는 방식이 다를 것을 기대하기 때문.
//ex) 9월 5일 기준, 9월 7일 휴일 9월 9일 휴일인 경우,
//사용자는 3일 DayAdd를 하면 9월 10일이 반환되는 것을 기대하지 
//9월 5일에 단순 3일을 더한 9월 8일을 Convention에 따라 
//조정한 날짜를 반환하는 것을 기대하지 않을 것이기 때문.
//전자는 Convention에 따라 반환값이 차이 날 수 없는 구조임.
const Date DayAdd(const ICalendar& calendar, const Date& d, int k, BusinessDayConvention dc)
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

const Date MonthAdd(const ICalendar& calendar, const Date& d, int k, BusinessDayConvention dc, bool eom)
{
	boost::gregorian::months month_unit(k);
	Date ret = d;

	ret += month_unit;
	if (eom && calendar.isEndOfMonth(d)) {
		return EndOfMonth(calendar, ret);
	}
	else {
		ret = GetGoodDay(calendar, ret, dc);
		return ret;
	}
}

const Date YearAdd(const ICalendar& calendar, const Date& d, int k, BusinessDayConvention dc, bool eom)
{
	boost::gregorian::years year_unit(k);
	Date ret = d;

	ret += year_unit;
	if (eom && calendar.isEndOfMonth(d)) {
		return EndOfMonth(calendar, ret);
	}
	else {
		ret = GetGoodDay(calendar, ret, dc);
		return ret;
	}
}

const Date EndOfMonth(const ICalendar& calendar, const Date& d)
{
	return GetGoodDay(calendar, d.end_of_month(), Preceding);
}