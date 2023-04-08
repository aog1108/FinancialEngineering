#include <Source/Times/Schedule/DrivenDateGenerator.h>
#include <Source/Times/ICalendar.h>
#include <Source/Times/DateHandling.h>
#include <Source/Times/Schedule/Period.h>
#include <Source/Times/Schedule/GenerationRule.h>

DrivenDateGenerator::DrivenDateGenerator(const std::shared_ptr<ICalendar>& calendar, DrivenDirection direction, const std::shared_ptr<Period>& period,
	BusinessDayConvention dc, bool is_eom)
	: calendar_(calendar), direction_(direction), period_(period), dc_(dc), is_eom_(is_eom) 
{ 
	auto driven_period_sign_func = [](DrivenDirection direction) { return direction == DrivenDirection::Forward ? 1 : -1; };
	if (period->getPeriodNum() * driven_period_sign_func(direction) < 0) {
		throw std::runtime_error("The sign of period num should be matched with the driven direction");
	}
}

std::vector<Date> DrivenDateGenerator::generateDrivenDate(const Date& effective_date, const Date& termination_date) const
{
	std::vector<Date> driven_dates;
	generateDrivenDate(driven_dates, effective_date, termination_date);
	return driven_dates;
}

bool DrivenDateGenerator::generateDrivenDate(std::vector<Date>& driven_dates, const Date& effective_date, const Date& termination_date) const
{
	Date driven_date_from = direction_ == DrivenDirection::Forward ? effective_date : termination_date;
	Date driven_date_to = direction_ == DrivenDirection::Forward ? termination_date : effective_date;
	const ICalendar* driven_calendar = calendar_.get();

	driven_dates.emplace_back(GetGoodDay(*driven_calendar, driven_date_from, dc_));
	//End of month가 적용되어 있는 경우, Driven date를 End of month로 만듦.
	driven_date_from = period_->advance(*driven_calendar, driven_date_from, Unadjusted, is_eom_);
	bool is_driven_end = isDrivenEnd(driven_date_from, driven_date_to, direction_);
	bool is_exist_stub = false;
	while (!is_driven_end) {
		driven_dates.emplace_back(GetGoodDay(*driven_calendar, driven_date_from, dc_));
		driven_date_from = period_->advance(*driven_calendar, driven_date_from, Unadjusted, is_eom_);
		is_driven_end = isDrivenEnd(driven_date_from, driven_date_to, direction_);
	}

	is_exist_stub = driven_date_from != period_->advance(*driven_calendar, driven_date_to, Unadjusted) ? true : false;

	//Stub가 존재하는 경우, 마지막 driven_date != driven_date_to이므로 따로 추가 필요
	if (is_exist_stub)
		driven_dates.emplace_back(GetGoodDay(*driven_calendar, driven_date_to, dc_));

	std::sort(driven_dates.begin(), driven_dates.end());
	return is_exist_stub;
}

bool isDrivenEnd(const Date& current_date, const Date& driven_end_date, DrivenDirection direction)
{
	switch (direction) {
	case (DrivenDirection::Forward):
		return current_date > driven_end_date;
	case (DrivenDirection::Backward):
		return current_date < driven_end_date;
	default:
		throw std::runtime_error("Unknown driven direction");
	}
}