#pragma once
#include <Source/Instrument/Cashflow/Cashflow.h>

class FloatingRateCashflow : public Cashflow {
public:
	FloatingRateCashflow() = default;
	FloatingRateCashflow(const Date& fixing_date, const Date& calculation_start_date, const Date& calculation_end_date, const Date& payment_date,
		const std::shared_ptr<DayCounter>& day_counter) : Cashflow(calculation_start_date, calculation_end_date, payment_date, day_counter), 
		fixing_date_(fixing_date), fixing_rate_(0.), is_fixed_(false) { }

	void doFixing(double fixing_rate) 
	{ 
		fixing_rate_ = fixing_rate; 
		is_fixed_ = true;
	}

	double amount() const { 
		if (is_fixed_)
			return fixing_rate_ * CalculationPeriodYearFraction();
		else
			throw std::runtime_error("Not fixed cashflow");
	}

	double accrued_amount(const Date& accrual_date) const {
		if (is_fixed_)
			return fixing_rate_ * day_counter_->Getcvg(calculation_start_date_, accrual_date);
		else
			throw std::runtime_error("Not fixed cashflow");
	}

protected:
	Date fixing_date_;
	double fixing_rate_;

	mutable bool is_fixed_;
};