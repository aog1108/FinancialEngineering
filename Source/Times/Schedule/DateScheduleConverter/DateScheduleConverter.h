#pragma once
#include <vector>
#include <memory>
#include <map>
#include <Source/Times/Date.h>
#include <source/Times/Schedule/GenerationRule.h>

class Cashflow;
class DayCounter;

using DateSchedule = std::map<KindsOfDate, std::vector<Date>>;

//기본클래스 Cashflow로의 변환 담당.
//Cashflow의 파생클래스가 생길 시 해당 파생클래스의 포인터로 변환하는 파생클래스를 만들어 쓸 목적으로 virtual 함수를 통해 구현.
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