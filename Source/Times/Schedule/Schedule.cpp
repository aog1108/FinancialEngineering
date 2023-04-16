#include <Source/Times/Schedule/Schedule.h>
#include <Source/Times/Schedule/DrivenDateGenerator.h>
#include <Source/Times/Schedule/DrivenDateSelector.h>
#include <Source/Times/Schedule/GenerationRule.h>
#include <Source/Times/Schedule/Relation.h>

Schedule::Schedule(const CalendarMapping& calendar_mapping,
	const std::shared_ptr<Period>& driven_period,
	KindsOfDate driven_date,
	DrivenDirection driven_direction,
	StubRule stub_rule,
	const BusinessDayConventionMapping& dc_mapping,
	const RelationList& relation_list,
	bool is_fixed_in_arrears,
	bool is_eom) : calendar_mapping_(calendar_mapping),
	driven_period_(driven_period),
	driven_date_(driven_date),
	driven_direction_(driven_direction),
	stub_rule_(stub_rule), 
	dc_mapping_(dc_mapping),
	relation_list_(relation_list),
	fixed_in_arrears_(is_fixed_in_arrears),
	is_eom_(is_eom) { }

DateSchedule Schedule::generateSchedule(const Date& effective_date, const Date& termination_date) const
{
	DateSchedule date_schedule;
	
	resetGenerationMapping();

	DrivenDateGenerator driven_date_generator(calendar_mapping_.at(driven_date_), driven_direction_, driven_period_, dc_mapping_.at(driven_date_), is_eom_);
	std::shared_ptr<DrivenDateSelector> driven_date_selector = generateDrivenDateSelector(driven_date_, stub_rule_, driven_direction_, fixed_in_arrears_);
	
	std::vector<Date> driven_dates;
	bool is_exist_stub = driven_date_generator.generateDrivenDate(driven_dates, effective_date, termination_date);
	std::vector<Date> selected_driven_dates = driven_date_selector->select(driven_dates, is_exist_stub);
	date_schedule[driven_date_] = selected_driven_dates;
	generation_complete_mapping_[driven_date_] = true;

	//Relation에 driven date 세팅
	for (auto& relation : relation_list_) {
		relation->setDrivenDates(&driven_dates);
	}

	//Driven date에 의해 생성되는 date들 생성
	for (const auto& relation : relation_list_) {
		const auto& relation_pair = relation->getRelationPair();
		KindsOfDate generated_date = relation_pair.first;
		if (relation_pair.second == driven_date_) {
			date_schedule[generated_date] = relation->applyRelation(*calendar_mapping_.at(generated_date), selected_driven_dates, dc_mapping_.at(generated_date), is_eom_);
			generation_complete_mapping_[generated_date] = true;
		}
	}

	//나머지 date 생성
	bool is_generation_end = date_schedule.size() == static_cast<std::size_t>(KindsOfDate::Count) ? true : false;
	while (!is_generation_end) {
		//아직 생성되지 않은 date 찾기
		const auto result = std::find_if(generation_complete_mapping_.begin(), generation_complete_mapping_.end(), [](const auto& pair) { return pair.second == false; });
		if (result != generation_complete_mapping_.end()) {
			//생성되지 않은 date 생성 위한 Relation 찾기
			KindsOfDate generated_date = result->first;
			const auto& relation = *std::find_if(relation_list_.begin(), relation_list_.end(),
				[generated_date = generated_date](const auto& relation) { return relation->getRelationPair().first == generated_date; });
			//해당 date를 생성하기 위한 date가 date_schdule 객체에 생성되었으면 생성
			const auto& relation_pair = relation->getRelationPair();
			if (date_schedule.find(relation_pair.second) != date_schedule.end()) {
				date_schedule[generated_date] = relation->applyRelation(*calendar_mapping_.at(generated_date), date_schedule.at(relation_pair.second), 
					dc_mapping_.at(generated_date), is_eom_);
				generation_complete_mapping_[generated_date] = true;
			}
		}
		is_generation_end = date_schedule.size() == static_cast<std::size_t>(KindsOfDate::Count) ? true : false;
	}

	return date_schedule;
}

void Schedule::resetGenerationMapping() const
{
	for (std::vector<std::shared_ptr<Relation>>::const_iterator relation = relation_list_.begin(); relation != relation_list_.end(); ++relation)
		generation_complete_mapping_[(*relation)->getRelationPair().first] = false;
}