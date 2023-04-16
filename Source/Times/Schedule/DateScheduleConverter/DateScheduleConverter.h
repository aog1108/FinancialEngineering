#pragma once
#include <vector>
#include <memory>
#include <map>
#include <Source/Times/Date.h>
#include <source/Times/Schedule/GenerationRule.h>

class Cashflow;
class DayCounter;

using DateSchedule = std::map<KindsOfDate, std::vector<Date>>;

class DateScheduleConverter {
public:
	DateScheduleConverter() = default;
	virtual ~DateScheduleConverter() = default;

	virtual std::vector<std::shared_ptr<Cashflow>> noFixingRateCashflows(const DateSchedule& date_schedule,
		const std::shared_ptr<DayCounter>& day_counter) const;
	
	virtual std::vector<std::shared_ptr<Cashflow>> FixedRateCashflows(const DateSchedule& date_schedule,
		const std::shared_ptr<DayCounter>& day_counter,
		double fixed_rate) const;
	
	virtual std::vector<std::shared_ptr<Cashflow>> RateVectorCashflows(const DateSchedule& date_schedule,
		const std::shared_ptr<DayCounter>& day_counter,
		const std::vector<double>& rate_vector) const;
};