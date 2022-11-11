#pragma once
#include <Source/PricingEngine/PricingEngine.h>
#include <Source/Instrument/EuropeanOption.h>
#include <Source/PricingEngine/BlackCalculator.h>

class BlackEuropeanOptionEngine : public AnalyticPricingEngine<EuropeanOption, BlackCalculator>{
public:
	void calculate();

};