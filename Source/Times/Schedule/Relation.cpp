#include <Source/Times/Schedule/Relation.h>

Date Equalto::next_date(const ICalendar& calendar, BusinessDayConvention dc, bool eom) const
{
	return GetGoodDay(calendar, *related_date_, dc);
}

Date DeducedFrom::next_date(const ICalendar& calendar, BusinessDayConvention dc, bool eom) const
{
	return period_->advance(calendar, *related_date_, dc, eom);
}