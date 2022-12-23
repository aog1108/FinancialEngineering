#pragma once
#include <Source/Instrument/Option.h>

struct EuropeanOption : public Option {
	EuropeanOption() = default;
	EuropeanOption(const std::shared_ptr<ICalendar>& calendar, const std::shared_ptr<DayCounter>& daycounter, 
		const std::shared_ptr<StrikedTypePayoff>& payoff, const std::shared_ptr<EuropeanExercise>& exercise) : 
		Option(calendar, daycounter, payoff, exercise) { }
};