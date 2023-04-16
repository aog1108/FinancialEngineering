#include <Source/Times/Schedule/DateScheduleConverter/DateScheduleConverter.h>
#include <Source/Instrument/Cashflow/Cashflow.h>
#include <Source/Times/DayCounter.h>

std::vector<std::shared_ptr<Cashflow>> DateScheduleConverter::noFixingRateCashflows(const DateSchedule& date_schedule,
	const std::shared_ptr<DayCounter>& day_counter) const
{
	auto length = date_schedule.begin()->second.size();
	std::vector<std::shared_ptr<Cashflow>> cashflows;

	for (auto i = 0; i != length; ++i)
		cashflows.push_back(std::make_shared<Cashflow>(date_schedule.at(KindsOfDate::CalculationStartDate)[i],
			date_schedule.at(KindsOfDate::CalculationEndDate)[i],
			date_schedule.at(KindsOfDate::FixingDate)[i],
			date_schedule.at(KindsOfDate::PaymentDate)[i],
			day_counter));

	return cashflows;
}

std::vector<std::shared_ptr<Cashflow>> DateScheduleConverter::FixedRateCashflows(const DateSchedule& date_schedule,
	const std::shared_ptr<DayCounter>& day_counter,
	double fixed_rate) const
{
	auto length = date_schedule.begin()->second.size();
	std::vector<std::shared_ptr<Cashflow>> cashflows;

	for (auto i = 0; i != length; ++i)
		cashflows.push_back(std::make_shared<Cashflow>(date_schedule.at(KindsOfDate::CalculationStartDate)[i],
			date_schedule.at(KindsOfDate::CalculationEndDate)[i],
			date_schedule.at(KindsOfDate::FixingDate)[i],
			date_schedule.at(KindsOfDate::PaymentDate)[i],
			day_counter,
			fixed_rate));

	return cashflows;
}

std::vector<std::shared_ptr<Cashflow>> DateScheduleConverter::RateVectorCashflows(const DateSchedule& date_schedule,
	const std::shared_ptr<DayCounter>& day_counter,
	const std::vector<double>& rate_vector) const
{
	auto length = date_schedule.begin()->second.size();
	if (length != rate_vector.size())
		throw std::runtime_error("Rate vector size should be equal to date vector size");

	std::vector<std::shared_ptr<Cashflow>> cashflows;

	for (auto i = 0; i != length; ++i)
		cashflows.push_back(std::make_shared<Cashflow>(date_schedule.at(KindsOfDate::CalculationStartDate)[i],
			date_schedule.at(KindsOfDate::CalculationEndDate)[i],
			date_schedule.at(KindsOfDate::FixingDate)[i],
			date_schedule.at(KindsOfDate::PaymentDate)[i],
			day_counter,
			rate_vector[i]));

	return cashflows;
}