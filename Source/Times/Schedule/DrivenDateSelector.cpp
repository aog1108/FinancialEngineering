#include <Source/Times/Schedule/DrivenDateSelector.h>
#include <Source/Times/Schedule/GenerationRule.h>

std::vector<Date> CalculationStartDateSelector::select(const std::vector<Date>& driven_dates, bool exist_stub) const
{
	return frontDrivenDateSelect(driven_dates, stub_rule_, direction_, exist_stub);
}

std::vector<Date> CalculationEndDateSelector::select(const std::vector<Date>& driven_dates, bool exist_stub) const
{
	return backDrivenDateSelect(driven_dates, stub_rule_, direction_, exist_stub);
}

std::vector<Date> FixingDateSelector::select(const std::vector<Date>& driven_dates, bool exist_stub) const
{
	if (isFixedInArrears_) {
		return backDrivenDateSelect(driven_dates, stub_rule_, direction_, exist_stub);
	}
	else {
		return frontDrivenDateSelect(driven_dates, stub_rule_, direction_, exist_stub);
	}
}

std::vector<Date> PaymentDateSelector::select(const std::vector<Date>& driven_dates, bool exist_stub) const
{
	return backDrivenDateSelect(driven_dates, stub_rule_, direction_, exist_stub);
}

std::vector<Date> frontDrivenDateSelect(const std::vector<Date>& driven_dates, StubRule stub_rule, DrivenDirection direction, bool exist_stub)
{
	//앞쪽의 driven date를 가져오는 것이므로 effective date 포함, termination date는 제외
	//->std::copy의 마지막 반복자는 최대 end() - 1까지만 가능.
	std::vector<Date> ret;
	auto size = driven_dates.size();
	if (exist_stub) {
		//LongStub인 경우
		if (stub_rule == StubRule::LongStub) {
			ret.reserve(size - 2);
			if (direction == DrivenDirection::Forward) {
				//Forward Generation인 경우
				//end() - 2에 해당하는 날짜만 빼고 복사.
				ret.assign(driven_dates.begin(), driven_dates.end() - 2);
			}
			else {
				//Backward Generation인 경우
				//begin() + 1에 해당하는 날짜만 빼고 복사.
				ret.emplace_back(driven_dates[0]);
				ret.insert(ret.end(), driven_dates.begin(), driven_dates.end() - 1);
			}
		}
		else {
			//ShortStub인 경우
				//전부 복사.
			ret.reserve(size - 1);
			ret.assign(driven_dates.begin(), driven_dates.end() - 1);
		}
	}
	else {
		//Stub 없는 경우
		//전부 복사.
		ret.reserve(size - 1);
		ret.assign(driven_dates.begin(), driven_dates.end() - 1);
	}

	return ret;
}

std::vector<Date> backDrivenDateSelect(const std::vector<Date>& driven_dates, StubRule stub_rule, DrivenDirection direction, bool exist_stub)
{
	//뒤쪽의 driven date를 가져오는 것이므로 effective date 제외, termination date는 포함
	//-> std::copy의 마지막 반복자는 최소 begin() + 1부터만 가능.
	std::vector<Date> ret;
	auto size = driven_dates.size();
	if (exist_stub) {
		//LongStub인 경우
		if (stub_rule == StubRule::LongStub) {
			ret.reserve(size - 2);
			if (direction == DrivenDirection::Forward) {
				//Forward Generation인 경우
				//end() - 2에 해당하는 날짜만 빼고 복사.
				ret.assign(driven_dates.begin() + 1, driven_dates.end() - 2);
				ret.emplace_back(driven_dates.back());
			}
			else {
				//Backward Generation인 경우
				//begin() + 1에 해당하는 날짜만 빼고 복사.
				ret.assign(driven_dates.begin() + 2, driven_dates.end());
			}
		}
		else {
			//ShortStub인 경우
				//전부 복사.
			ret.reserve(size - 1);
			ret.assign(driven_dates.begin() + 1, driven_dates.end());
		}
	}
	else {
		//Stub 없는 경우
		//전부 복사.
		ret.reserve(size - 1);
		ret.assign(driven_dates.begin() + 1, driven_dates.end());
	}

	return ret;
}

std::shared_ptr<DrivenDateSelector> generateDrivenDateSelector(KindsOfDate kinds_of_date, StubRule stub_rule, DrivenDirection direction, bool isFixedInArrears)
{
	switch (kinds_of_date) {
	case (KindsOfDate::CalculationStartDate):
		return std::make_shared<CalculationStartDateSelector>(stub_rule, direction);
	case (KindsOfDate::CalculationEndDate):
		return std::make_shared<CalculationEndDateSelector>(stub_rule, direction);
	case (KindsOfDate::FixingDate):
		return std::make_shared<FixingDateSelector>(stub_rule, direction, isFixedInArrears);
	case (KindsOfDate::PaymentDate):
		return std::make_shared<PaymentDateSelector>(stub_rule, direction);
	default:
		throw std::runtime_error("Unknown kind of date");
	}
}