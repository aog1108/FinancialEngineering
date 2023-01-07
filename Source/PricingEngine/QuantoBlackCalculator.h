#pragma once
#include <Source/PricingEngine/BlackCalculator.h>

//Vega, Rho의 구현을 바꾸어야 하고, 그 때 BlackCalculator의 멤버 변수를 사용해야 하므로 private 상속으로 구현함.
//겹치는 부분이 많아서 public 상속을 받고, 바꾸는 일부 메소드에 대해서만 
//using 선언을 통해 BlackCalculator의 메소드를 private화시키는 것으로 구현할 수도 있으나,
//이렇게 구현 시 다형성을 적용해 BlackCalculator 포인터를 통해 
//QuantoBlackCalculator의 메소드 호출 시 BlackCalculator 메소드가 호출되는 걸 막을 방법이 없음.

class QuantoBlackCalculator : private BlackCalculator {
public:
	QuantoBlackCalculator() = default;
	QuantoBlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff, double forward, double discount, double stddev, double variance);

	double value() const;
	double delta(double S) const;
	double gamma(double S) const;
	double vega(double ttm, double sigma_fx, double rho_fx) const;
	double theta(double spot, double ttm) const;
	double discountrho(double ttm) const;
	double driftrho(double ttm) const;
	double epsilon(double ttm) const;
	double fxvega(double ttm, double rho_fx) const;
	double quantocega(double ttm, double sigma_fx) const;

	double forward() const;
	double strike() const;
	double discount() const;
	double stddev() const;
	double variance() const;
};