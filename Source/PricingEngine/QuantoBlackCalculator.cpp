#include <Source/PricingEngine/QuantoBlackCalculator.h>
#include <cmath>

QuantoBlackCalculator::QuantoBlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff, double forward, double discount, double stddev, double variance) :
	BlackCalculator(payoff, forward, discount, stddev, variance) { initialize(payoff); }

double QuantoBlackCalculator::value() const
{
	return BlackCalculator::value();
}

double QuantoBlackCalculator::delta(double S) const
{
	return BlackCalculator::delta(S);
}

double QuantoBlackCalculator::gamma(double S) const
{
	return BlackCalculator::gamma(S);
}

double QuantoBlackCalculator::vega(double ttm, double sigma_fx, double rho_fx) const
{
	double DforwardDsigma = -rho_fx * sigma_fx * ttm * forward_;
	double Dd1Dsigma = std::sqrt(ttm) * (-std::log(forward_ / strike_) / (variance_) + 1 / 2.0);
	double Dd2Dsigma = Dd1Dsigma - std::sqrt(ttm);

	return discount_ * (DforwardDsigma * alpha_ + forward_ * DalphaDd1_ * Dd1Dsigma + x_ * DbetaDd2_ * Dd2Dsigma);
}

double QuantoBlackCalculator::theta(double spot, double ttm) const
{
	return BlackCalculator::theta(spot, ttm);
}

double QuantoBlackCalculator::discountrho(double ttm) const
{
	return -ttm * value();
}

double QuantoBlackCalculator::driftrho(double ttm) const
{
	double DforwardDrf = ttm * forward_;
	double Dd1Drf = ttm / stddev_, Dd2Drf = Dd1Drf;

	return discount_ * (DforwardDrf * alpha_ + forward_ * DalphaDd1_ * Dd1Drf + x_ * DbetaDd2_ * Dd2Drf);
}

double QuantoBlackCalculator::fxvega(double ttm, double rho_fx) const
{
	double DforwardDsigmafx = -rho_fx * stddev_ * std::sqrt(ttm) * forward_;
	double Dd1Dsigmafx = -rho_fx / std::sqrt(ttm);
	double Dd2Dsigmafx = Dd1Dsigmafx;

	return discount_ * (DforwardDsigmafx * alpha_ + forward_ * DalphaDd1_ * Dd1Dsigmafx + x_ * DbetaDd2_ * Dd2Dsigmafx);
}

double QuantoBlackCalculator::quantocega(double ttm, double sigma_fx) const
{
	double DforwardDrhofx = -sigma_fx * stddev_ * std::sqrt(ttm) * forward_;
	double Dd1Drhofx = -sigma_fx / std::sqrt(ttm);
	double Dd2Drhofx = Dd1Drhofx;

	return discount_ * (DforwardDrhofx * alpha_ + forward_ * DalphaDd1_ * Dd1Drhofx + x_ * DbetaDd2_ * Dd2Drhofx);
}

double QuantoBlackCalculator::epsilon(double ttm) const
{
	return BlackCalculator::epsilon(ttm);
}

double QuantoBlackCalculator::forward() const
{
	return forward_;
}

double QuantoBlackCalculator::strike() const
{
	return strike_;
}

double QuantoBlackCalculator::discount() const
{
	return discount_;
}

double QuantoBlackCalculator::stddev() const
{
	return stddev_;
}

double QuantoBlackCalculator::variance() const
{
	return variance_;
}