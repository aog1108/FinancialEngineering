#include <stdexcept>
#include <Source/PricingEngine/BlackCalculator.h>
#include <Source/Math/NormalDistribution.h>
#include <Source/DesignPattern/VisitorPattern.h>

//Builder 클래스를 visitor로 정의하고, 구현을 cpp 파일에 두는 것으로 visit 함수가 추가될 때마다 새로 컴파일해야 되는 것을 방지.

class BlackCalculator::Builder : public AcyclicVisitor,
								public Visitor<Payoff>,
								public Visitor<PlainVanillaPayoff>,
								public Visitor<CashOrNothingPayoff>,
								public Visitor<AssetOrNothingPayoff> {
public:
	Builder(BlackCalculator& black) : black_(black) { }

	void visit(Payoff&);
	void visit(PlainVanillaPayoff&);
	void visit(CashOrNothingPayoff&);
	void visit(AssetOrNothingPayoff&);

private:
	BlackCalculator& black_;
};

BlackCalculator::BlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff, double forward, double discount, double stddev, double variance)
	: forward_(forward), discount_(discount), stddev_(stddev), variance_(variance) { initialize(payoff); }

void BlackCalculator::initialize(const std::shared_ptr<StrikedTypePayoff>& payoff)
{
	OptionType type = payoff->optionType();
	d1_ = (std::log(forward_ / strike_) + 0.5 * variance_) / stddev_;
	d2_ = (d1_ - stddev_);
	Nd1_ = normcdf(type * d1_), Nd2_ = normcdf(type * d2_);
	nd1_ = normpdf(d1_), nd2_ = normpdf(d2_);
	DNd1Dd1_ = type * normpdf(type * d1_), DNd2Dd2_ = type * normpdf(type * d2_);

	Builder builder(*this);
	payoff->accept(builder);
}

void BlackCalculator::Builder::visit(Payoff& payoff)
{
	throw std::runtime_error("Unsupported payoff time.");
}

void BlackCalculator::Builder::visit(PlainVanillaPayoff& payoff)
{
	black_.alpha_ = payoff.optionType() * black_.Nd1_, black_.beta_ = payoff.optionType() * black_.Nd2_;
}

void BlackCalculator::Builder::visit(CashOrNothingPayoff& payoff)
{
	black_.alpha_ = 0.0, black_.beta_ = payoff.cashpayoff() * black_.Nd2_;
}

void BlackCalculator::Builder::visit(AssetOrNothingPayoff& payoff)
{
	black_.alpha_ = payoff.optionType() * black_.Nd1_, black_.beta_ = 0.0;
}

double BlackCalculator::value() const
{
	return discount_ * (forward_ * alpha_ - strike_ * beta_);
}