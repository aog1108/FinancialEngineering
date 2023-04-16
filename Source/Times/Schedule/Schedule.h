#pragma once
#include <map>
#include <vector>
#include <Source/Times/BusinessDayConvention.h>
#include <Source/Times/Date.h>

class ICalendar;
class Cashflow;
class Relation;
class Period;

enum class KindsOfDate;
enum class DrivenDirection;
enum class StubRule;

using DateSchedule = std::map<KindsOfDate, std::vector<Date>>;

class Schedule {
public:
	using CalendarMapping = std::map<KindsOfDate, std::shared_ptr<ICalendar>>;
	using RelationList = std::vector<std::shared_ptr<Relation>>;
	using BusinessDayConventionMapping = std::map<KindsOfDate, BusinessDayConvention>;

	Schedule() = default;
	Schedule(const CalendarMapping& calendar_mapping,
		const std::shared_ptr<Period>& driven_period,
		KindsOfDate driven_date,
		DrivenDirection driven_direction,
		StubRule stub_rule,
		const BusinessDayConventionMapping& dc_mapping,
		const RelationList& relation_list,
		bool fixed_in_arrears,
		bool is_eom = false);

	//각 Date에 대한 vector 생성
	DateSchedule generateSchedule(const Date& effective_date, const Date& termination_date) const;

protected:
	void resetGenerationMapping() const;
	
	CalendarMapping calendar_mapping_;
	std::shared_ptr<Period> driven_period_;
	KindsOfDate driven_date_;
	DrivenDirection driven_direction_;
	StubRule stub_rule_;
	BusinessDayConventionMapping dc_mapping_;
	bool fixed_in_arrears_;
	bool is_eom_;

	RelationList relation_list_;

	mutable std::map<KindsOfDate, bool> generation_complete_mapping_;
};

class ScheduleFactory {
public:
	ScheduleFactory() = default;

	ScheduleFactory& addCalendar(KindsOfDate kinds_of_date, const std::shared_ptr<ICalendar>& calendar);
	ScheduleFactory& withDrivenPeriod(const std::shared_ptr<Period>& driven_period);
	ScheduleFactory& withDrivenDate(KindsOfDate driven_date);
	ScheduleFactory& withDrivenDirection(DrivenDirection driven_direction);
	ScheduleFactory& withStubRule(StubRule stub_rule);
	ScheduleFactory& withBusinessDayConventionMapping(const std::map<KindsOfDate, BusinessDayConvention>& dc_mapping);
	ScheduleFactory& withFixedinArrears(bool fixed_in_arrears);
	ScheduleFactory& withEndOfMonth(bool is_eom);
	ScheduleFactory& addRelation(const std::shared_ptr<Relation>& relation);

	void resetRelation() { relation_list_.clear(); }

	operator Schedule() const;

private:
	Schedule::CalendarMapping calendar_mapping_;
	std::shared_ptr<Period> driven_period_;
	KindsOfDate driven_date_;
	DrivenDirection driven_direction_;
	StubRule stub_rule_;
	Schedule::BusinessDayConventionMapping dc_mapping_;
	bool fixed_in_arrears_;
	bool is_eom_;

	Schedule::RelationList relation_list_;
};

inline ScheduleFactory& ScheduleFactory::addCalendar(KindsOfDate kinds_of_date, const std::shared_ptr<ICalendar>& calendar)
{
	calendar_mapping_[kinds_of_date] = calendar;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::withDrivenPeriod(const std::shared_ptr<Period>& driven_period)
{
	driven_period_ = driven_period;
	return *this;
};

inline ScheduleFactory& ScheduleFactory::withDrivenDate(KindsOfDate driven_date)
{
	driven_date_ = driven_date;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::withDrivenDirection(DrivenDirection driven_direction)
{
	driven_direction_ = driven_direction;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::withStubRule(StubRule stub_rule)
{
	stub_rule_ = stub_rule;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::withBusinessDayConventionMapping(const std::map<KindsOfDate, BusinessDayConvention>& dc_mapping)
{
	dc_mapping_ = dc_mapping;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::withFixedinArrears(bool fixed_in_arrears)
{
	fixed_in_arrears_ = fixed_in_arrears;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::withEndOfMonth(bool is_eom)
{
	is_eom_ = is_eom;
	return *this;
}

inline ScheduleFactory& ScheduleFactory::addRelation(const std::shared_ptr<Relation>& relation)
{
	relation_list_.emplace_back(relation);
	return *this;
}

inline ScheduleFactory::operator Schedule() const
{
	return Schedule(calendar_mapping_,
		driven_period_,
		driven_date_,
		driven_direction_,
		stub_rule_,
		dc_mapping_,
		relation_list_,
		fixed_in_arrears_,
		is_eom_);
}