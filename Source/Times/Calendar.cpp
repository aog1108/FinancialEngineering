#include <Source/Times/Calendar.h>
#include <Source/Times/Date.h>
#include <algorithm>
#include <stdexcept>
#include <Source/Times/DateHandling.h>

bool Calendar::isBusinessDay(const Date& d) const
{
	return !isHoliday(d);
}

bool Calendar::isHoliday(const Date& d) const
{
	if (isWeekEnd(d) || std::find(holidays_.begin(), holidays_.end(), d) != holidays_.end())
		return true;
	else
		return false;
}

bool Calendar::isWeekEnd(const Date& d) const
{
	if (d.day_of_week() == boost::date_time::Saturday || d.day_of_week() == boost::date_time::Sunday)
		return true;
	else
		return false;
}

bool Calendar::isEndOfMonth(const Date& d) const
{
	if ((d - EndOfMonth(*this, d)).days() >= 0 && (d.end_of_month() - d).days() >= 0)
		return true;
	else
		return false;
}