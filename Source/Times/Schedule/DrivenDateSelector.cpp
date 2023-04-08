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
	//������ driven date�� �������� ���̹Ƿ� effective date ����, termination date�� ����
	//->std::copy�� ������ �ݺ��ڴ� �ִ� end() - 1������ ����.
	std::vector<Date> ret;
	auto size = driven_dates.size();
	if (exist_stub) {
		//LongStub�� ���
		if (stub_rule == StubRule::LongStub) {
			ret.reserve(size - 2);
			if (direction == DrivenDirection::Forward) {
				//Forward Generation�� ���
				//end() - 2�� �ش��ϴ� ��¥�� ���� ����.
				ret.assign(driven_dates.begin(), driven_dates.end() - 2);
			}
			else {
				//Backward Generation�� ���
				//begin() + 1�� �ش��ϴ� ��¥�� ���� ����.
				ret.emplace_back(driven_dates[0]);
				ret.insert(ret.end(), driven_dates.begin(), driven_dates.end() - 1);
			}
		}
		else {
			//ShortStub�� ���
				//���� ����.
			ret.reserve(size - 1);
			ret.assign(driven_dates.begin(), driven_dates.end() - 1);
		}
	}
	else {
		//Stub ���� ���
		//���� ����.
		ret.reserve(size - 1);
		ret.assign(driven_dates.begin(), driven_dates.end() - 1);
	}

	return ret;
}

std::vector<Date> backDrivenDateSelect(const std::vector<Date>& driven_dates, StubRule stub_rule, DrivenDirection direction, bool exist_stub)
{
	//������ driven date�� �������� ���̹Ƿ� effective date ����, termination date�� ����
	//-> std::copy�� ������ �ݺ��ڴ� �ּ� begin() + 1���͸� ����.
	std::vector<Date> ret;
	auto size = driven_dates.size();
	if (exist_stub) {
		//LongStub�� ���
		if (stub_rule == StubRule::LongStub) {
			ret.reserve(size - 2);
			if (direction == DrivenDirection::Forward) {
				//Forward Generation�� ���
				//end() - 2�� �ش��ϴ� ��¥�� ���� ����.
				ret.assign(driven_dates.begin() + 1, driven_dates.end() - 2);
				ret.emplace_back(driven_dates.back());
			}
			else {
				//Backward Generation�� ���
				//begin() + 1�� �ش��ϴ� ��¥�� ���� ����.
				ret.assign(driven_dates.begin() + 2, driven_dates.end());
			}
		}
		else {
			//ShortStub�� ���
				//���� ����.
			ret.reserve(size - 1);
			ret.assign(driven_dates.begin() + 1, driven_dates.end());
		}
	}
	else {
		//Stub ���� ���
		//���� ����.
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