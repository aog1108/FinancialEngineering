#include <Source/PricingEngine/QuantoBlackEuropeanOptionEngine.h>
#include <Source/Settings.h>
#include <Source/Times/DateHandling.h>

QuantoBlackEuropeanOptionEngine::QuantoBlackEuropeanOptionEngine(const std::shared_ptr<EuropeanOption>& option, double S, double r, double rf, double q, 
	double sigma, double sigma_fx, double rho_fx) :
	AnalyticPricingEngine<EuropeanOption, QuantoBlackCalculator>(option), S_(S), r_(r), rf_(rf), q_(q), sigma_(sigma), sigma_fx_(sigma_fx), rho_fx_(rho_fx)
{
	setupCalculator();
}

void QuantoBlackEuropeanOptionEngine::setupCalculator()
{
	double ttm = instrument_->daycounter_->Getcvg(Settings::get().ValueDate(), instrument_->exercise_->date(0));
	double forward = S_ * std::exp((rf_ - q_ - rho_fx_ * sigma_ * sigma_fx_) * ttm);
	double discount = std::exp(-r_ * ttm);
	double stddev = sigma_ * std::sqrt(ttm);
	double variance = stddev * stddev;
	std::shared_ptr<StrikedTypePayoff> payoff = std::dynamic_pointer_cast<StrikedTypePayoff>(instrument_->payoff_);

	calculator_ = QuantoBlackCalculator(payoff, forward, discount, stddev, variance);
}

void QuantoBlackEuropeanOptionEngine::setupParameter(double S, double r, double rf, double q, double sigma, double sigma_fx, double rho_fx)
{
	IsCalculated_ = false;
	IsSetup_ = false;
	S_ = S;
	r_ = r;
	rf_ = rf;
	q_ = q;
	sigma_ = sigma;
	sigma_fx_ = sigma_fx;
	rho_fx_ = rho_fx;
}

void QuantoBlackEuropeanOptionEngine::calculate()
{
	if (!IsCalculated_) {
		if (!IsSetup_)
			setupCalculator();

		double ttm = calculator_.variance() / (sigma_ * sigma_);

		results_.value_ = calculator_.value();
		results_.additionalResults["delta"] = calculator_.delta(S_);
		results_.additionalResults["gamma"] = calculator_.gamma(S_);
		results_.additionalResults["vega"] = calculator_.vega(ttm, sigma_fx_, rho_fx_);
		results_.additionalResults["theta"] = calculator_.theta(S_, ttm);
		results_.additionalResults["discount_rho"] = calculator_.discountrho(ttm);
		results_.additionalResults["drift_rho"] = calculator_.driftrho(ttm);
		results_.additionalResults["epsilon"] = calculator_.epsilon(ttm);
		results_.additionalResults["fx_vega"] = calculator_.fxvega(ttm, rho_fx_);
		results_.additionalResults["quanto_cega"] = calculator_.quantocega(ttm, sigma_fx_);
	}
}
