#include <Source/Instrument/Payoff/Payoff.h>
#include <algorithm>

double PlainVanillaPayoff::operator()(double price) const
{
	return std::max(type_ * (price - strike_), 0.0);
}

void PlainVanillaPayoff::accept(AcyclicVisitor& v)
{
	if (auto v1 = dynamic_cast<Visitor<PlainVanillaPayoff>*>(&v))
		v1->visit(*this);
}

double CashOrNothingPayoff::operator()(double price) const
{
	return type_ * (price - strike_) > 0.0 ? cashpayoff_ : 0.0;
}

void CashOrNothingPayoff::accept(AcyclicVisitor& v)
{
	if (auto v1 = dynamic_cast<Visitor<CashOrNothingPayoff>*>(&v))
		v1->visit(*this);
}

double AssetOrNothingPayoff::operator()(double price) const
{
	return type_ * (price - strike_) > 0.0 ? price : 0.0;
}

void AssetOrNothingPayoff::accept(AcyclicVisitor& v)
{
	if (auto v1 = dynamic_cast<Visitor<AssetOrNothingPayoff>*>(&v))
		v1->visit(*this);
}