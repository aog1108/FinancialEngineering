#pragma once
#include <memory>
#include <Source/PricingEngine/Calculator.h>
#include <Source/Instrument/Payoff/Payoff.h>

class BlackCalculator : public Calculator {
public:
	BlackCalculator() = default;
	BlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff, double forward, double discount, double stddev, double variance);

	double value() const;
	double delta(double S) const;
	double gamma() const;
	double vega() const;
	double theta() const;
	double rho() const;

private:
	class Builder;

	void initialize(const std::shared_ptr<StrikedTypePayoff>&);

	double forward_, strike_, discount_, stddev_, variance_;
	
	double d1_, d2_;
	double Nd1_, Nd2_;
	double nd1_, nd2_;
	double alpha_, beta_, x_;

	double DNd1Dd1_, DNd2Dd2_;
	double DalphaDd1_, DbetaDd2_;
};