#pragma once
#include <Source/Instrument/Option.h>
#include <Source/Instrument/Exercise/Exercise.h>
#include <Source/Instrument/Payoff/Payoff.h>

class EuropeanOption : public Option {
public:
	EuropeanOption() = default;
	EuropeanOption(const std::shared_ptr<StrikedTypePayoff>& payoff, const std::shared_ptr<EuropeanExercise>& exercise) : Option(payoff, exercise) { }
};