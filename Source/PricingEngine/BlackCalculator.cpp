#include <stdexcept>
#include <Source/PricingEngine/BlackCalculator.h>
#include <Source/Math/NormalDistribution.h>

void BlackCalculator::initialize(const std::shared_ptr<StrikedTypePayoff>& payoff)
{
	d1_ = (std::log(forward_ / strike_) + 0.5 * variance_) / stddev_;
	d2_ = (d1_ - stddev_);
	Nd1_ = normcdf(payoff->optionType() * d1_), Nd2_ = normcdf(payoff->optionType() * d2_);
	nd1_ = normpdf(d1_), nd2_ = normpdf(d2_);
}

void BlackCalculator::visit(Payoff& payoff)
{
	throw std::runtime_error("Unsupported payoff time.");
}

void BlackCalculator::visit(PlainVanillaPayoff& payoff)
{
	alpha_ = payoff.optionType() * Nd1_, beta_ = payoff.optionType() * Nd2_;
}

void BlackCalculator::visit(CashOrNothingPayoff& payoff)
{
	alpha_ = 0.0, beta_ = payoff.cashpayoff() * Nd2_;
}

void BlackCalculator::visit(AssetOrNothingPayoff& payoff)
{
	alpha_ = payoff.optionType() * Nd1_, beta_ = 0.0;
}

double BlackCalculator::value() const
{
	return discount_ * (forward_ * alpha_ - strike_ * beta_);
}

double BlackCalculator::delta(double S) const
{

}