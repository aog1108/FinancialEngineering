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
	NormalDistribution norm_dist(0.0, 1.0);

	strike_ = payoff->strike();
	d1_ = (std::log(forward_ / strike_) + 0.5 * variance_) / stddev_;
	d2_ = (d1_ - stddev_);
	Nd1_ = norm_dist.cdf(type * d1_), Nd2_ = norm_dist.cdf(type * d2_);
	nd1_ = norm_dist.pdf(d1_), nd2_ = norm_dist.pdf(d2_);
	DNd1Dd1_ = type * norm_dist.pdf(type * d1_), DNd2Dd2_ = type * norm_dist.pdf(type * d2_);

	Builder builder(*this);
	payoff->accept(builder);
}

void BlackCalculator::Builder::visit(Payoff& payoff)
{
	throw std::runtime_error("Unsupported payoff time.");
}

void BlackCalculator::Builder::visit(PlainVanillaPayoff& payoff)
{
	OptionType type = payoff.optionType();
	black_.alpha_ = type * black_.Nd1_, black_.beta_ = type * black_.Nd2_;
	black_.x_ = -black_.strike_;
	black_.DalphaDd1_ = type * black_.DNd1Dd1_, black_.DbetaDd2_ = type * black_.DNd2Dd2_;
}

void BlackCalculator::Builder::visit(CashOrNothingPayoff& payoff)
{
	OptionType type = payoff.optionType();
	NormalDistribution norm_dist(0.0, 1.0);
	black_.alpha_ = 0.0, black_.beta_ = black_.Nd2_;
	black_.x_ = payoff.cashpayoff();
	black_.DalphaDd1_ = 0.0, black_.DbetaDd2_ = type * norm_dist.pdf(type * black_.d2_);
}

void BlackCalculator::Builder::visit(AssetOrNothingPayoff& payoff)
{
	OptionType type = payoff.optionType();
	NormalDistribution norm_dist(0.0, 1.0);
	black_.alpha_ = black_.Nd1_, black_.beta_ = 0.0;
	black_.x_ = 0.0;
	black_.DalphaDd1_ = type * norm_dist.pdf(type * black_.d1_), black_.DbetaDd2_ = 0.0;
}

double BlackCalculator::value() const
{
	return discount_ * (forward_ * alpha_ + x_ * beta_);
}

double BlackCalculator::delta(double S) const
{
	double DforwardDS = forward_ / S;
	double Dd1DS = 1 / (stddev_ * forward_) * DforwardDS, Dd2DS = Dd1DS;
	
	return discount_ * (DforwardDS * alpha_ + forward_ * DalphaDd1_ * Dd1DS + x_ * DbetaDd2_ * Dd2DS);
}