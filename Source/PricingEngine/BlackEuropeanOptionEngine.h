#pragma once
#include <Source/PricingEngine/PricingEngine.h>
#include <Source/Instrument/EuropeanOption.h>
#include <Source/PricingEngine/BlackCalculator.h>

class BlackEuropeanOptionEngine : public AnalyticPricingEngine<EuropeanOption, BlackCalculator>{
public:
	BlackEuropeanOptionEngine() = default;
	BlackEuropeanOptionEngine(const std::shared_ptr<EuropeanOption>& option, double S, double r, double q, double sigma);
	
	void setupParameter(double S, double r, double q, double sigma);
	void calculate();

private:
	void setupCalculator();

	double S_, r_, q_, sigma_;
};