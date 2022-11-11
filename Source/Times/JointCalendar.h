#pragma once
#include <Source/Times/ICalendar.h>
#include <Source/Times/Calendar.h>

class JointCalendar : public ICalendar {
public:
	JointCalendar() = default;
	JointCalendar(const std::string& names, const std::vector<Date>& holidays) : calendar_mapping_({ { names, Calendar(holidays)} }) { }
	explicit JointCalendar(const std::map<std::string, std::vector<Date>>& holiday_mapping)
	{
		for (auto it = holiday_mapping.begin(); it != holiday_mapping.end(); ++it) {
			calendar_mapping_[it->first] = Calendar(it->second);
		}
	}
	explicit JointCalendar(const std::map<std::string, Calendar>& calendar_mapping) : calendar_mapping_(calendar_mapping) { }
	
	void addCalendar(const std::string& name, const std::vector<Date>& holidays) { calendar_mapping_[name] = Calendar(holidays); }

	const std::vector<Date>& GetHolidayList(const std::string& name) const { return calendar_mapping_.at(name).GetHolidayList(); }

	bool isBusinessDay(const Date&) const override;
	bool isHoliday(const Date&) const override;
	bool isWeekEnd(const Date&) const override;
	bool isEndOfMonth(const Date&) const override;

private:
	std::map<std::string, Calendar> calendar_mapping_;
};