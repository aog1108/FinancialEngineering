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

	explicit PricingEngine(const T& instrument) : instrument_(instrument), IsCalculated_(false) { }

	void setupInstruments(const T& instrument) 
	{ 
		IsCalculated_ = false;
		instrument_ = instrument; 
	}

	void reset() 
	{ 
		IsCalculated_ = false;
		results_.reset(); 
	}

	const Results& GetResults() const { return results_; }
	
protected:
	T instrument_;
	Results results_;

	bool IsCalculated_;
};

//AnalyticPricingEngine�� �߻�Ŭ����. �Ļ�Ŭ������ Instrument�� Calculator�� ���ø��� �����ؼ� ��ӹ��� ��.
//calculator_ ��� ������ ���� �ʱ�ȭ�ϴ� ������ �ƴ϶�, �����ڿ� �ԷµǴ� �Ű��������� ���� ���� �ʱ�ȭ.
//setupCalculator�� �׸� ���� ���� ���� �Լ�.
template <typename T, typename AnalyticFormCalculator>
class AnalyticPricingEngine : public PricingEngine<T> {
public:
	AnalyticPricingEngine() = default;
	AnalyticPricingEngine(const T& instrument) : PricingEngine<T>(instrument) { }

protected:
	virtual void setupCalculator() = 0;

	AnalyticFormCalculator calculator_;
};