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

//AnalyticPricingEngine은 추상클래스. 파생클래스는 Instrument와 Calculator의 템플릿을 지정해서 상속받을 것.
//calculator_ 멤버 변수는 직접 초기화하는 변수가 아니라, 생성자에 입력되는 매개변수들을 통해 간접 초기화.
//setupCalculator는 그를 위한 순수 가상 함수.
template <typename T, typename AnalyticFormCalculator>
class AnalyticPricingEngine : public PricingEngine<T> {
public:
	AnalyticPricingEngine() = default;
	AnalyticPricingEngine(const T& instrument) : PricingEngine(instrument) { }

	virtual void calculate() = 0;

protected:
	virtual void setupCalculator() = 0;

	AnalyticFormCalculator calculator_;
};