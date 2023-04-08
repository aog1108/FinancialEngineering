#include <Source/Times/Schedule/Relation.h>
#include <Source/Times/Schedule/GenerationRule.h>

std::vector<Date> Equalto::applyRelation(const ICalendar& calendar, const std::vector<Date>& dates, BusinessDayConvention dc, bool eom) const
{
	//연관된 date가 backDrivenDateSelect에 가까운 로직으로 생성된 것인지, frontDrivenDateSelect에 가까운 로직으로 생성된 것인지 판단.
	//Calculation start date : frontDrivenDateSelect와 비슷한 로직으로 생성되어야 함.
	//Calculation end date, Payment date : backDrivenDateSelect와 비슷한 로직으로 생성되어야 함.
	//Fixing date는 FixedInArrears 값에 따라 둘 다 가능, 하지만 여기선 FixedInArrears값은 무시하고 순수한 의미의 Equalto가 적용됨.
	//Fixing date 생성에서 FixedInArrears가 기능할 때는 fixing date가 driven date일 때 뿐.

	//Equalto로 생성하게 되면 생성되는 날짜는 effective date ~ termination date 범위 안에 존재하게 됨.
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