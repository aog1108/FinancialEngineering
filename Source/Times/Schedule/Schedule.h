#pragma once
#include <Source/Times/ICalendar.h>
#include <Source/Times/Schedule/GenerationRule.h>
#include <map>

class Cashflow;

//Extrapolation enum class -> enum����
//���� error throw runtime error�� ��ȯ�ϱ�

class Schedule {
public:
	Schedule() = default;
	Schedule(const std::shared_ptr<ICalendar>& fixing_calendar,
		const std::shared_ptr<ICalendar>& payment_calendar,
		StubRule stub_rule,
		DrivenDirection driven_direction,
		DateAdjust date_adjust,
		const std::shared_ptr<DateRelations>& date_relations,
		bool is_eom = false) :
		fixing_calendar_(fixing_calendar), payment_calendar_(payment_calendar), 
		stub_rule_(stub_rule), driven_direction_(driven_direction),
		date_relations_(date_relations), is_eom_(is_eom) { }
	
	//�� Date�� ���� vector ����
	std::map<KindsOfDate, std::vector<Date>> generateSchedule() const;
	//Schedule�� Cashflow ��ü ���·� ����
	std::vector<std::shared_ptr<Cashflow>> generateCashflows() const;

private:
	std::shared_ptr<ICalendar> fixing_calendar_;
	std::shared_ptr<ICalendar> payment_calendar_;
	StubRule stub_rule_;
	DrivenDirection driven_direction_;
	DateAdjust date_adjust_;
	std::shared_ptr<DateRelations> date_relations_;
	bool is_eom_;
};