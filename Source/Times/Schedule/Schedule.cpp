#include <Source/Times/Schedule/Schedule.h>
#include <Source/Times/Schedule/DrivenDateGenerator.h>
#include <Source/Times/Schedule/DrivenDateSelector.h>
#include <Source/Times/Schedule/GenerationRule.h>
#include <Source/Times/Schedule/Relation.h>

class Schedule::Impl {
public:
	Impl() = default;
	virtual ~Impl() = default;

	Impl(const CalendarMapping& calendar_mapping,
		const std::shared_ptr<Period>& driven_period,
		KindsOfDate driven_date,
		DrivenDirection driven_direction,
		StubRule stub_rule,
		const BusinessDayConventionMapping& dc_mapping,
		const RelationList& relation_list,
		bool fixed_in_arrears,
		bool is_eom = false) : calendar_mapping_(calendar_mapping),
		driven_period_(driven_period),
		driven_date_(driven_date),
		driven_direction_(driven_direction),
		stub_rule_(stub_rule),
		dc_mapping_(dc_mapping),
		relation_list_(relation_list),
		fixed_in_arrears_(fixed_in_arrears),
		is_eom_(is_eom) { };

	virtual DateSchedule generateSchedule(const ScheduleInput& schedule_input) const;

protected:
	void resetGenerationMapping() const;

	CalendarMapping calendar_mapping_;
	std::shared_ptr<Period> driven_period_;
	KindsOfDate driven_date_;
	DrivenDirection driven_direction_;
	StubRule stub_rule_;
	BusinessDayConventionMapping dc_mapping_;
	bool fixed_in_arrears_;
	bool is_eom_;

	RelationList relation_list_;

	mutable std::map<KindsOfDate, bool> generation_complete_mapping_;
};

class Schedule::FixingDateDrivenImpl : public Schedule::Impl {
public:
	FixingDateDrivenImpl() = default;
	FixingDateDrivenImpl(const CalendarMapping& calendar_mapping,
		const std::shared_ptr<Period>& driven_period,
		KindsOfDate driven_date,
		DrivenDirection driven_direction,
		StubRule stub_rule,
		const BusinessDayConventionMapping& dc_mapping,
		const RelationList& relation_list,
		bool fixed_in_arrears,
		bool is_eom = false) : Impl(calendar_mapping, 
			driven_period,
			driven_date,
			driven_direction,
			stub_rule,
			dc_mapping,
			relation_list,
			fixed_in_arrears,
			is_eom) { }

	DateSchedule generateSchedule(const ScheduleInput& schedule_input) const override;
};

Schedule::Schedule(const CalendarMapping& calendar_mapping,
	const std::shared_ptr<Period>& driven_period,
	KindsOfDate driven_date,
	DrivenDirection driven_direction,
	StubRule stub_rule,
	const BusinessDayConventionMapping& dc_mapping,
	const RelationList& relation_list,
	bool is_fixed_in_arrears,
	bool is_eom) 
{
	if (driven_date == KindsOfDate::FixingDate)
		impl_ = std::make_shared<FixingDateDrivenImpl>(calendar_mapping,
			driven_period,
			driven_date,
			driven_direction,
			stub_rule,
			dc_mapping,
			relation_list,
			is_fixed_in_arrears,
			is_eom);
	else
		impl_ = std::make_shared<Impl>(calendar_mapping,
			driven_period,
			driven_date,
			driven_direction,
			stub_rule,
			dc_mapping,
			relation_list,
			is_fixed_in_arrears,
			is_eom);
}

void Schedule::Impl::resetGenerationMapping() const
{
	for (std::vector<std::shared_ptr<Relation>>::const_iterator relation = relation_list_.begin(); relation != relation_list_.end(); ++relation)
		generation_complete_mapping_[(*relation)->getRelationPair().first] = false;
}

DateSchedule Schedule::Impl::generateSchedule(const ScheduleInput& schedule_input) const
{
	DateSchedule date_schedule;

	resetGenerationMapping();

	const Date& effective_date = schedule_input.effective_date_;
	const Date& termination_date = schedule_input.termination_date_;

	std::vector<Date> driven_dates;
	DrivenDateGenerator driven_date_generator(calendar_mapping_.at(driven_date_), driven_direction_, driven_period_, dc_mapping_.at(driven_date_), is_eom_);
	std::shared_ptr<DrivenDateSelector> driven_date_selector = generateDrivenDateSelector(driven_date_, stub_rule_, driven_direction_, fixed_in_arrears_);
	bool is_exist_stub = driven_date_generator.generateDrivenDate(driven_dates, effective_date, termination_date);
	std::vector<Date> selected_driven_dates = driven_date_selector->select(driven_dates, is_exist_stub);
	date_schedule[driven_date_] = selected_driven_dates;
	generation_complete_mapping_[driven_date_] = true;

	//Relation�� driven date ����
	for (auto& relation : relation_list_) {
		relation->setDrivenDates(&driven_dates);
	}

	//Driven date�� ���� �����Ǵ� date�� ����
	for (const auto& relation : relation_list_) {
		const auto& relation_pair = relation->getRelationPair();
		KindsOfDate generated_date = relation_pair.first;
		if (relation_pair.second == driven_date_) {
			date_schedule[generated_date] = relation->applyRelation(*calendar_mapping_.at(generated_date), selected_driven_dates, dc_mapping_.at(generated_date), is_eom_);
			generation_complete_mapping_[generated_date] = true;
		}
	}

	//������ date ����
	bool is_generation_end = date_schedule.size() == static_cast<std::size_t>(KindsOfDate::Count) ? true : false;
	while (!is_generation_end) {
		//���� �������� ���� date ã��
		const auto result = std::find_if(generation_complete_mapping_.begin(), generation_complete_mapping_.end(), [](const auto& pair) { return pair.second == false; });
		if (result != generation_complete_mapping_.end()) {
			//�������� ���� date ���� ���� Relation ã��
			KindsOfDate generated_date = result->first;
			const auto& relation = *std::find_if(relation_list_.begin(), relation_list_.end(),
				[generated_date = generated_date](const auto& relation) { return relation->getRelationPair().first == generated_date; });
			//�ش� date�� �����ϱ� ���� date�� date_schdule ��ü�� �����Ǿ����� ����
			const auto& relation_pair = relation->getRelationPair();
			if (date_schedule.find(relation_pair.second) != date_schedule.end()) {
				date_schedule[generated_date] = relation->applyRelation(*calendar_mapping_.at(generated_date), date_schedule.at(relation_pair.second),
					dc_mapping_.at(generated_date), is_eom_);
				generation_complete_mapping_[generated_date] = true;
			}
		}
		is_generation_end = date_schedule.size() == static_cast<std::size_t>(KindsOfDate::Count) ? true : false;
	}

	//Fixing date�� �Է��� ���� ������, �� ������ ����.
	if (!fixed_in_arrears_ && !schedule_input.initial_in_advance_fixing_date_.is_not_a_date())
		date_schedule[KindsOfDate::FixingDate][0] = schedule_input.initial_in_advance_fixing_date_;
	else if (fixed_in_arrears_ && !schedule_input.last_in_arrears_fixing_date_.is_not_a_date())
		date_schedule[KindsOfDate::FixingDate].back() = schedule_input.last_in_arrears_fixing_date_;

	return date_schedule;
}

DateSchedule Schedule::FixingDateDrivenImpl::generateSchedule(const ScheduleInput& schedule_input) const
{
	DateSchedule date_schedule;

	resetGenerationMapping();

	const Date& effective_date = schedule_input.initial_in_advance_fixing_date_;
	const Date& termination_date = schedule_input.last_in_arrears_fixing_date_;

	std::vector<Date> driven_dates;
	DrivenDateGenerator driven_date_generator(calendar_mapping_.at(driven_date_), driven_direction_, driven_period_, dc_mapping_.at(driven_date_), is_eom_);
	std::shared_ptr<DrivenDateSelector> driven_date_selector = generateDrivenDateSelector(driven_date_, stub_rule_, driven_direction_, fixed_in_arrears_);
	bool is_exist_stub = driven_date_generator.generateDrivenDate(driven_dates, effective_date, termination_date);
	std::vector<Date> selected_driven_dates = driven_date_selector->select(driven_dates, is_exist_stub);
	date_schedule[driven_date_] = selected_driven_dates;
	generation_complete_mapping_[driven_date_] = true;

	//driven_dates���� 1��° ���� = effective date, ������ ���� = termination date�� ���� ����
	std::vector<Date> driven_dates_adjusted = driven_dates; 
	driven_dates_adjusted[0] = schedule_input.effective_date_;
	driven_dates_adjusted.back() = schedule_input.termination_date_;

	//Relation�� driven date ����
	for (auto& relation : relation_list_) {
		relation->setDrivenDates(&driven_dates_adjusted);
	}

	//Driven date�� ���� �����Ǵ� date�� ����
	for (const auto& relation : relation_list_) {
		const auto& relation_pair = relation->getRelationPair();
		KindsOfDate generated_date = relation_pair.first;
		if (relation_pair.second == driven_date_) {
			date_schedule[generated_date] = relation->applyRelation(*calendar_mapping_.at(generated_date), selected_driven_dates, dc_mapping_.at(generated_date), is_eom_);
			generation_complete_mapping_[generated_date] = true;
		}
	}

	//������ date ����
	bool is_generation_end = date_schedule.size() == static_cast<std::size_t>(KindsOfDate::Count) ? true : false;
	while (!is_generation_end) {
		//���� �������� ���� date ã��
		const auto result = std::find_if(generation_complete_mapping_.begin(), generation_complete_mapping_.end(), [](const auto& pair) { return pair.second == false; });
		if (result != generation_complete_mapping_.end()) {
			//�������� ���� date ���� ���� Relation ã��
			KindsOfDate generated_date = result->first;
			const auto& relation = *std::find_if(relation_list_.begin(), relation_list_.end(),
				[generated_date = generated_date](const auto& relation) { return relation->getRelationPair().first == generated_date; });
			//�ش� date�� �����ϱ� ���� date�� date_schdule ��ü�� �����Ǿ����� ����
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

DateSchedule Schedule::generateSchedule(const ScheduleInput& schedule_input) const
{
	return impl_->generateSchedule(schedule_input);
}