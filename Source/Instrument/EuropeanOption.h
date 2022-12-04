#pragma once
#include <Source/Instrument/Option.h>
#include <Source/Instrument/Exercise/Exercise.h>
#include <Source/Instrument/Payoff/Payoff.h>

struct EuropeanOption : public Option {
	EuropeanOption() = default;
	EuropeanOption(const std::shared_ptr<StrikedTypePayoff>& payoff, const std::shared_ptr<EuropeanExercise>& exercise) : Option(payoff, exercise) { }
};