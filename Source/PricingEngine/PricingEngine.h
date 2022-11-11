#pragma once
#include <memory>
#include <map>
#include <boost/any.hpp>

struct Results {
	void reset() { *this = Results(); }

	double value_;
	std::map<std::string, boost::any> additionalResults;
};

template <typename T>
class PricingEngine {
public:
	virtual ~PricingEngine() = default;
	virtual void calculate() = 0;

	explicit PricingEngine(const T& instrument) : instrument_(instrument) { }

	void setupInstruments(const T& instrument) { instrument_ = instrument; }
	void reset() { results_.reset(); }
	
protected:
	T instrument_;
	Results results_;
};

template <typename T, typename AnalyticFormCalculator>
class AnalyticPricingEngine : public PricingEngine<T> {
public:
	AnalyticPricingEngine() = default;
	AnalyticPricingEngine(const T& instrument, const AnalyticFormCalculator& calculator) : PricingEngine(insturment), calculator_(calculator) { }

	void calculate() = 0;

	void setupCalculator(const AnalyticFormCalculator& calculator) { calculator_ = calculator; }

protected:
	AnalyticFormCalculator calculator_;
};