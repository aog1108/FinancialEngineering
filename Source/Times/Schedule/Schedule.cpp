#include <Source/Times/Schedule/Schedule.h>

Schedule::Schedule(const std::shared_ptr<ICalendar>& fixing_calendar,
	const std::shared_ptr<ICalendar>& payment_calendar,
	StubRule stub_rule,
	DrivenDirection driven_direction,
	DateAdjust date_adjust,
	bool is_eom) : fixing_calendar_(fixing_calendar), payment_calendar_(payment_calendar),
	stub_rule_(stub_rule), driven_direction_(driven_direction), date_adjust_(date_adjust), is_eom_(is_eom) { }