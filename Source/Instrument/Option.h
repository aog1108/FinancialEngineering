#pragma once
#include <Source/Instrument/Instrument.h>
#include <Source/Instrument/Payoff/Payoff.h>
#include <Source/Instrument/Exercise/Exercise.h>

struct Option : public Instrument {
	Option() { }
	Option(const std::shared_ptr<ICalendar>& calendar, const std::shared_ptr<DayCounter>& daycounter, 
		const std::shared_ptr<Payoff>& payoff, const std::shared_ptr<Exercise>& exercise) 
		: Instrument(calendar, daycounter), payoff_(payoff), exercise_(exercise) { }

	std::shared_ptr<Payoff> payoff_;
	std::shared_ptr<Exercise> exercise_;
};