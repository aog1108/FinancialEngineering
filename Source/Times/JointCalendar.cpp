#include <Source/Times/JointCalendar.h>
#include <Source/Times/DateHandling.h>

bool JointCalendar::isBusinessDay(const Date& d) const
{
	return !isHoliday(d);
}

bool JointCalendar::isHoliday(const Date& d) const
{
	for (auto it = calendar_mapping_.begin(); it != calendar_mapping_.end(); ++it) {
		if (it->second.isHoliday(d))
			return true;
	}
	return false;
}

bool JointCalendar::isWeekEnd(const Date& d) const
{
	return calendar_mapping_.begin()->second.isWeekEnd(d);
}

bool JointCalendar::isEndOfMonth(const Date& d) const
{
	if ((d - EndOfMonth(*this, d)).days() >= 0 && (d.end_of_month() - d).days() >= 0)
		return true;
	else
		return false;
}