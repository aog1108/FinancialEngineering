#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/DayCounter.h>

class Cashflow {
public:
	Cashflow() = default;
	virtual ~Cashflow() = default;

	Cashflow(const Date& calculation_start_date,
		const Date& calculation_end_date,
		const Date& fixing_date,
		const Date& payment_date,
		const std::shared_ptr<DayCounter>& day_counter) :
		calculation_start_date_(calculation_start_date),
		calculation_end_date_(calculation_end_date),
		fixing_date_(fixing_date),
		payment_date_(payment_date),
		day_counter_(day_counter),
		fixing_rate_(0.),
		calculated_amount_(0.),
		isCalculated_(false) { }

	Cashflow(const Date& calculation_start_date,
		const Date& calculation_end_date,
		const Date& fixing_date,
		const Date& payment_date,
		const std::shared_ptr<DayCounter>& day_counter,
		double fixing_rate) :
		calculation_start_date_(calculation_start_date),
		calculation_end_date_(calculation_end_date),
		fixing_date_(fixing_date),
		payment_date_(payment_date),
		day_counter_(day_counter),
		fixing_rate_(fixing_rate),
		calculated_amount_(0.),
		isCalculated_(false) { }

	const Date& CalculationStartDate() const { return calculation_start_date_; }
	const Date& CalculationEndDate() const { return calculation_end_date_; }
	const Date& FixingDate() const { return fixing_date_; }
	const Date& PaymentDate() const { return payment_date_; }

	int CalculationPeriod() const { return day_counter_->DayCount(calculation_start_date_, calculation_end_date_); }
	double CalculationPeriodYearFraction() const { return day_counter_->Getcvg(calculation_start_date_, calculation_end_date_); }

	bool hasOccured(const Date& value_date) const { return value_date > payment_date_; }
	void doFixing(double fixing_rate) { fixing_rate_ = fixing_rate; }

	virtual double amount() const 
	{ 
		if (!isCalculated_) {
			calculated_amount_ = fixing_rate_ * CalculationPeriodYearFraction();
			isCalculated_ = true;
		}
		return calculated_amount_;
	}
	double accrued_amount(const Date& accrual_date) const 
	{ 
		if (accrual_date >= payment_date_)
			throw std::runtime_error("Accrual date should be earlier than payment date");

		return fixing_rate_* day_counter_->Getcvg(calculation_start_date_, accrual_date); 
	}

private:
	Date calculation_start_date_; 
	Date calculation_end_date_;
	Date fixing_date_;
	Date payment_date_;

	std::shared_ptr<DayCounter> day_counter_;

	double fixing_rate_;
	mutable double calculated_amount_;
	mutable bool isCalculated_;
};