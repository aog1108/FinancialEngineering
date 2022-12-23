#pragma once
#include <memory>
#include <Source/Times/ICalendar.h>
#include <Source/Times/DayCounter.h>

struct Instrument {
	Instrument() = default;
	Instrument(const std::shared_ptr<ICalendar>& calendar, const std::shared_ptr<DayCounter>& daycounter) : calendar_(calendar), daycounter_(daycounter) { }
	virtual ~Instrument() = default;

	std::shared_ptr<ICalendar> calendar_;
	std::shared_ptr<DayCounter> daycounter_;
};