#pragma once
#include <memory>
#include <vector>
#include <Source/Times/BusinessDayConvention.h>
#include <Source/Times/Date.h>

class ICalendar;
class Period;

enum class DrivenDirection;

class DrivenDateGenerator {
public:
	DrivenDateGenerator() = default;
	DrivenDateGenerator(const std::shared_ptr<ICalendar>& calendar, DrivenDirection direction, const std::shared_ptr<Period>& period, 
		BusinessDayConvention dc, bool is_eom);

	std::vector<Date> generateDrivenDate(const Date& effective_date, const Date& termination_date) const;
	bool generateDrivenDate(std::vector<Date>& driven_dates, const Date& effective_date, const Date& termination_date) const;

private:
	std::shared_ptr<ICalendar> calendar_;
	std::shared_ptr<Period> period_;
	DrivenDirection direction_;
	BusinessDayConvention dc_;
	bool is_eom_;
};

bool isDrivenEnd(const Date& current_date, const Date& driven_end_date, DrivenDirection direction);