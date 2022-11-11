#pragma once
#include <Source/DesignPattern/VisitorPattern.h>

enum OptionType {
	Call = 1,
	Put = -1
};

class Payoff {
public:
	virtual ~Payoff() = default;

	virtual double operator()(double price) const = 0;
	virtual void accept(AcyclicVisitor& v)
	{
		if (auto v1 = dynamic_cast<Visitor<Payoff>*>(&v))
			v1->visit(*this);
	}
};

class TypePayoff : public Payoff {
public:
	OptionType optionType() const { return type_; }

protected:
	TypePayoff(OptionType type) : type_(type) { }
	OptionType type_;
};

class StrikedTypePayoff : public TypePayoff {
public:
	double strike() const { return strike_; }

protected:
	StrikedTypePayoff(OptionType type, double strike) : TypePayoff(type), strike_(strike) { }
	double strike_;
};

class PlainVanillaPayoff : public StrikedTypePayoff {
public:
	PlainVanillaPayoff(OptionType type, double strike) : StrikedTypePayoff(type, strike) { }

	double operator()(double price) const override;
	void accept(AcyclicVisitor&) override;
};

class CashOrNothingPayoff : public StrikedTypePayoff {
public:
	CashOrNothingPayoff(OptionType type, double strike, double cashpayoff) : StrikedTypePayoff(type, strike), cashpayoff_(cashpayoff) { }
	
	double cashpayoff() const { return cashpayoff_; }

	double operator()(double price) const override;
	void accept(AcyclicVisitor&) override;

protected:
	double cashpayoff_;
};

class AssetOrNothingPayoff : public StrikedTypePayoff {
public:
	AssetOrNothingPayoff(OptionType type, double strike) : StrikedTypePayoff(type, strike) { }

	double operator()(double price) const override;
	void accept(AcyclicVisitor&) override;
};