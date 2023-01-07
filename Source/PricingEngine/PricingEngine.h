#pragma once
#include <memory>
#include <map>
#include <boost/any.hpp>

struct Results {
	void reset() { *this = Results(); }

	double value_;
	std::map<std::string, boost::any> additionalResults;
};

//PricingEngine�� �߻�Ŭ������ ����ϱ� ���ؼ� template Ŭ������ �ƴϾ�� ��.
//EngineBase Ŭ������ template Ŭ������ �ϰ� PricingEngine Ŭ������ �߻�Ŭ������ ����.
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

//AnalyticPricingEngine�� �߻�Ŭ����. �Ļ�Ŭ������ Instrument�� Calculator�� ���ø��� �����ؼ� ��ӹ��� ��.
//calculator_ ��� ������ ���� �ʱ�ȭ�ϴ� ������ �ƴ϶�, �����ڿ� �ԷµǴ� �Ű��������� ���� ���� �ʱ�ȭ.
//setupCalculator�� �׸� ���� ���� ���� �Լ�.
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