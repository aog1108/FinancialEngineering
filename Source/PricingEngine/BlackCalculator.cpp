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
	D2Nd1Dd12_ = -type * d1_ * norm_dist.pdf(type * d1_), D2Nd2Dd22_ = -type * d2_ * norm_dist.pdf(type * d2_);

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
	black_.D2alphaDd12_ = type * black_.D2Nd1Dd12_, black_.D2betaDd22_ = type * black_.D2Nd2Dd22_;
}

void BlackCalculator::Builder::visit(CashOrNothingPayoff& payoff)
{
	OptionType type = payoff.optionType();
	NormalDistribution norm_dist(0.0, 1.0);
	black_.alpha_ = 0.0, black_.beta_ = black_.Nd2_;
	black_.x_ = payoff.cashpayoff();
	black_.DalphaDd1_ = 0.0, black_.DbetaDd2_ = black_.DNd2Dd2_;
	black_.D2alphaDd12_ = 0.0, black_.D2betaDd22_ = black_.D2Nd2Dd22_;
}

void BlackCalculator::Builder::visit(AssetOrNothingPayoff& payoff)
{
	OptionType type = payoff.optionType();
	NormalDistribution norm_dist(0.0, 1.0);
	black_.alpha_ = black_.Nd1_, black_.beta_ = 0.0;
	black_.x_ = 0.0;
	black_.DalphaDd1_ = black_.DNd1Dd1_, black_.DbetaDd2_ = 0.0;
	black_.D2alphaDd12_ = black_.D2Nd1Dd12_, black_.DbetaDd2_ = 0.0;
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

double BlackCalculator::gamma(double S) const
{
	double DforwardDS = forward_ / S;
	double Dd1DS = 1 / (stddev_ * forward_) * DforwardDS, Dd2DS = Dd1DS;
	double D2d1DS2 = -1 / (S * S * stddev_), D2d2DS2 = D2d1DS2;
	double temp1 = DforwardDS * DalphaDd1_ * Dd1DS;
	double temp2 = DforwardDS * DalphaDd1_ * Dd1DS + forward_ * (D2alphaDd12_ * Dd1DS * Dd1DS + DalphaDd1_ * D2d1DS2);
	double temp3 = x_ * (D2betaDd22_ * Dd2DS * Dd2DS + DbetaDd2_ * D2d2DS2);

	return discount_ * (temp1 + temp2 + temp3);
}

double BlackCalculator::vega(double ttm) const
{
	double Dd1Dsigma = std::sqrt(ttm) * (-std::log(forward_ / strike_) / (variance_)+1 / 2.0);
	double Dd2Dsigma = Dd1Dsigma - std::sqrt(ttm);

	return discount_ * (forward_ * DalphaDd1_ * Dd1Dsigma + x_ * DbetaDd2_ * Dd2Dsigma);
}
	
double BlackCalculator::theta(double spot, double ttm) const
{
	double DdiscountDt = -1 / ttm * std::log(discount_) * discount_;
	double DFDt = -forward_ * 1 / ttm * std::log(forward_ / spot);
	double Dd1Dt = -1 / (stddev_ * ttm) * (-1 / 2.0 * std::log(spot / strike_) + 1 / 2.0 * std::log(forward_ / spot) + variance_ / 4);
	double Dd2Dt = Dd1Dt + stddev_ / (2 * ttm);
	double temp1 = DdiscountDt * value() / discount_;
	double temp2 = DFDt * alpha_;
	double temp3 = forward_ * DalphaDd1_ * Dd1Dt;
	double temp4 = x_ * DbetaDd2_ * Dd2Dt;

	return temp1 + discount_ * (temp2 + temp3 + temp4);
}

double BlackCalculator::rho(double ttm) const
{
	double DforwardDr = ttm * forward_;
	double Dd1Dr = ttm / stddev_, Dd2Dr = Dd1Dr;
	double temp1 = -ttm * value();
	double temp2 = discount_ * (DforwardDr * alpha_ + forward_ * DalphaDd1_ * Dd1Dr + x_ * DbetaDd2_ * Dd2Dr);

	return temp1 + temp2;
}

double BlackCalculator::epsilon(double ttm) const
{
	double DforwardDq = -ttm * forward_;
	double Dd1Dq = -ttm / stddev_, Dd2Dq = Dd1Dq;

	return discount_ * (DforwardDq * alpha_ + forward_ * DalphaDd1_ * Dd1Dq + x_ * DbetaDd2_ * Dd2Dq);
}