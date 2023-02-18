#pragma once
#include <Source/Instrument/Cashflow/Cashflow.h>

class FixedRateCashflow : public Cashflow {
public:
	FixedRateCashflow() = default;
	FixedRateCashflow(double fixed_rate, const Date& calculation_start_date, const Date& calculation_end_date, const Date& payment_date,
		const std::shared_ptr<DayCounter>& day_counter) : Cashflow(calculation_start_date, calculation_end_date, payment_date, day_counter), fixed_rate_(fixed_rate) { }

	double amount() const { return fixed_rate_ * CalculationPeriodYearFraction(); }
	double accrued_amount(const Date& accrual_date) const { return fixed_rate_ * day_counter_->Getcvg(calculation_start_date_, accrual_date); }

protected:
	double fixed_rate_;
};