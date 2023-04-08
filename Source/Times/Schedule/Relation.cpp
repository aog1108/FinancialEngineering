#include <Source/Times/Schedule/Relation.h>
#include <Source/Times/Schedule/GenerationRule.h>

std::vector<Date> Equalto::applyRelation(const ICalendar& calendar, const std::vector<Date>& dates, BusinessDayConvention dc, bool eom) const
{
	//������ date�� backDrivenDateSelect�� ����� �������� ������ ������, frontDrivenDateSelect�� ����� �������� ������ ������ �Ǵ�.
	//Calculation start date : frontDrivenDateSelect�� ����� �������� �����Ǿ�� ��.
	//Calculation end date, Payment date : backDrivenDateSelect�� ����� �������� �����Ǿ�� ��.
	//Fixing date�� FixedInArrears ���� ���� �� �� ����, ������ ���⼱ FixedInArrears���� �����ϰ� ������ �ǹ��� Equalto�� �����.
	//Fixing date �������� FixedInArrears�� ����� ���� fixing date�� driven date�� �� ��.

	//Equalto�� �����ϰ� �Ǹ� �����Ǵ� ��¥�� effective date ~ termination date ���� �ȿ� �����ϰ� ��.
	int date_diff1 = std::abs(static_cast<int>((dates.front() - driven_dates_->front()).days()));
	int date_diff2 = std::abs(static_cast<int>((dates.back() - driven_dates_->back()).days()));

	if (relation_pair_.first == KindsOfDate::CalculationStartDate) {
		if (date_diff1 > date_diff2) {
			std::vector<Date> ret;
			ret.emplace_back(GetGoodDay(calendar, driven_dates_->front(), dc));
			std::transform(dates.begin(), dates.end() - 1, std::back_inserter(ret),
				[calendar = &calendar, dc = dc](const Date& date) { return GetGoodDay(*calendar, date, dc); });
			return ret;
		}
		else
			return dates;
	}
	else if (relation_pair_.first == KindsOfDate::CalculationEndDate || relation_pair_.first == KindsOfDate::PaymentDate) {
		if (date_diff1 < date_diff2) {
			std::vector<Date> ret;
			std::transform(dates.begin() + 1, dates.end(), std::back_inserter(ret),
				[calendar = &calendar, dc = dc](const Date& date) { return GetGoodDay(*calendar, date, dc); });
			ret.emplace_back(GetGoodDay(calendar, driven_dates_->back(), dc));
			return ret;
		}
		else
			return dates;
	}
	else {
		return dates;
	}
}

std::vector<Date> DeducedFrom::applyRelation(const ICalendar& calendar, const std::vector<Date>& dates, BusinessDayConvention dc, bool eom) const
{
	std::vector<Date> ret;
	std::transform(dates.begin(), dates.end(), std::back_inserter(ret),
		[period = period_.get(), calendar = &calendar, dc = dc, eom = eom](const auto& date) { return period->advance(*calendar, date, dc, eom); });
	return ret;
}