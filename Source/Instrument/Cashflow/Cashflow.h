#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/DayCounter.h>

class Cashflow {
public:
	Cashflow() = default;
	virtual ~Cashflow() = default;
	Cashflow(const Date& calculation_start_date, const Date& calculation_end_date, const Date& payment_date, const std::shared_ptr<DayCounter>& day_counter)
		: calculation_start_date_(calculation_start_date), calculation_end_date_(calculation_end_date), payment_date_(payment_date), day_counter_(day_counter) { }

	const Date& CalculationStartDate() const { return calculation_start_date_; }
	const Date& CalculationEndDate() const { return calculation_end_date_; }
	const Date& PaymentDate() const { return payment_date_; }

	int CalculationPeriod() const { return day_counter_->DayCount(calculation_start_date_, calculation_end_date_); }
	double CalculationPeriodYearFraction() const { return day_counter_->Getcvg(calculation_start_date_, calculation_end_date_); }

	bool hasOccured(const Date& value_date) const { return value_date > payment_date_; }

	virtual double amount() const = 0;
	virtual double accrued_amount(const Date& accrual_date) const = 0;

protected:
	Date calculation_start_date_; 
	Date calculation_end_date_;
	Date payment_date_;

	std::shared_ptr<DayCounter> day_counter_;
};