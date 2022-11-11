#pragma once
#include <map>
#include <memory>
#include <Source/Times/ICalendar.h>
#include <Source/Times/Date.h>
#include <Source/Times/BusinessDayConvention.h>

class Calendar : public ICalendar {
public:
	Calendar() = default;
	explicit Calendar(const std::vector<Date>& holidays) : holidays_(holidays) { }
	
	const std::vector<Date>& GetHolidayList() const { return holidays_; }

	bool isBusinessDay(const Date&) const override;
	bool isHoliday(const Date&) const override;
	bool isWeekEnd(const Date&) const override;
	bool isEndOfMonth(const Date&) const override;

private:
	std::vector<Date> holidays_;
};