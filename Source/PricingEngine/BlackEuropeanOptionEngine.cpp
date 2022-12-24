#include <Source/PricingEngine/BlackEuropeanOptionEngine.h>
#include <Source/Settings.h>
#include <Source/Times/DateHandling.h>

BlackEuropeanOptionEngine::BlackEuropeanOptionEngine(const std::shared_ptr<EuropeanOption>& option, double S, double r, double q, double sigma) :
	AnalyticPricingEngine<EuropeanOption, BlackCalculator>(option), S_(S), r_(r), q_(q), sigma_(sigma) 
{ 
	setupCalculator();
}

void BlackEuropeanOptionEngine::setupCalculator()
{
	double ttm = instrument_->daycounter_->Getcvg(Settings::get().ValueDate(), instrument_->exercise_->date(0));
	double forward = S_ * std::exp((r_ - q_) * ttm);
	double discount = std::exp(-r_ * ttm);
	double stddev = sigma_ * std::sqrt(ttm);
	double variance = stddev * stddev;
	std::shared_ptr<StrikedTypePayoff> payoff = std::dynamic_pointer_cast<StrikedTypePayoff>(instrument_->payoff_);

	calculator_ = BlackCalculator(payoff, forward, discount, stddev, variance);
}

void BlackEuropeanOptionEngine::setupParameter(double S, double r, double q, double sigma)
{
	IsCalculated_ = false;
	IsSetup_ = false;
	S_ = S;
	r_ = r;
	q_ = q;
	sigma_ = sigma;
}

void BlackEuropeanOptionEngine::calculate()
{
	if (!IsCalculated_) {
		if (!IsSetup_)
			setupCalculator();

		double ttm = calculator_.variance() / (sigma_ * sigma_);

		results_.value_ = calculator_.value();
		results_.additionalResults["delta"] = calculator_.delta(S_);
		results_.additionalResults["gamma"] = calculator_.gamma(S_);
		results_.additionalResults["vega"] = calculator_.vega(ttm);
		results_.additionalResults["theta"] = calculator_.theta(S_, ttm);
		results_.additionalResults["rho"] = calculator_.rho(ttm);
		results_.additionalResults["epsilon"] = calculator_.epsilon(ttm);
	}
}
