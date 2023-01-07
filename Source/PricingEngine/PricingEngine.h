#pragma once
#include <memory>
#include <map>
#include <boost/any.hpp>

struct Results {
	void reset() { *this = Results(); }

	double value_;
	std::map<std::string, boost::any> additionalResults;
};

//PricingEngine을 추상클래스로 사용하기 위해선 template 클래스가 아니어야 함.
//EngineBase 클래스를 template 클래스로 하고 PricingEngine 클래스는 추상클래스로 만듦.
class PricingEngine {
public:
	PricingEngine() = default;
	virtual ~PricingEngine() = default;
	virtual void calculate() = 0;
};

template <typename T>
class EngineBase : public PricingEngine {
public:
	EngineBase() = default;

	explicit EngineBase(const std::shared_ptr<T>& instrument) : instrument_(instrument), IsCalculated_(false) { }

	void setupInstruments(const std::shared_ptr<T>& instrument)
	{
		IsCalculated_ = false;
		instrument_ = instrument;
	}

	virtual void reset()
	{
		IsCalculated_ = false;
		results_.reset();
	}

	const Results& GetResults() const { return results_; }

protected:
	std::shared_ptr<T> instrument_;
	Results results_;

	bool IsCalculated_;
};

//AnalyticPricingEngine은 추상클래스. 파생클래스는 Instrument와 Calculator의 템플릿을 지정해서 상속받을 것.
//calculator_ 멤버 변수는 직접 초기화하는 변수가 아니라, 생성자에 입력되는 매개변수들을 통해 간접 초기화.
//setupCalculator는 그를 위한 순수 가상 함수.
template <typename T, typename AnalyticFormCalculator>
class AnalyticPricingEngine : public EngineBase<T> {
public:
	AnalyticPricingEngine() = default;
	AnalyticPricingEngine(const std::shared_ptr<T>& instrument) : EngineBase<T>(instrument), IsSetup_(false) { }

	void reset()
	{
		EngineBase<T>::IsCalculated_ = false;
		IsSetup_ = false;
		EngineBase<T>::results_.reset();
	}

protected:
	virtual void setupCalculator() = 0;

	AnalyticFormCalculator calculator_;
	bool IsSetup_;
};