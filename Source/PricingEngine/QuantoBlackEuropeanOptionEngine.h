#pragma once
#include <Source/PricingEngine/PricingEngine.h>
#include <Source/Instrument/EuropeanOption.h>
#include <Source/PricingEngine/QuantoBlackCalculator.h>

class QuantoBlackEuropeanOptionEngine : public AnalyticPricingEngine<EuropeanOption, QuantoBlackCalculator> {
public:
	QuantoBlackEuropeanOptionEngine() = default;
	QuantoBlackEuropeanOptionEngine(const std::shared_ptr<EuropeanOption>& option, double S, double r, double rf, double q, 
		double sigma, double sigma_fx, double rho_fx);

	void setupParameter(double S, double r, double rf, double q, double sigma, double sigma_fx, double rho_fx);
	void calculate();

private:
	void setupCalculator();

	double S_, r_, rf_, q_, sigma_, sigma_fx_, rho_fx_;
};