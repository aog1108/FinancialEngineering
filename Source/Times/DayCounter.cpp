#include <Source/Times/DayCounter.h>
#include <Source/Times/Date.h>

const std::map<DayCountConvention, std::string> DayCounter::name_mapping_
= { {ACT365Fixed, "ACT365Fixed"}, {ACTACTISDA, "ACTACTISDA"}, {ACT360, "ACT360"},
{Thirty360B, "30/360B"}, {Thirty360US, "30/360US"}, {ThirtyE360, "30E/360"} };

class DayCounter::Impl {
public:
	virtual ~Impl() { }

	virtual int DayCount(const Date& d1, const Date& d2) const { return (d2 - d1).days(); }
	virtual double Getcvg(const Date&, const Date&) const = 0;
};

class DayCounter::ACT365FixedImpl : public DayCounter::Impl {
public:
	double Getcvg(const Date& d1, const Date& d2) const { return DayCount(d1, d2) / 365.0; }
};

class DayCounter::ACTACTISDAImpl : public DayCounter::Impl {
public:
	double Getcvg(const Date&, const Date&) const;
};

class DayCounter::ACT360Impl : public DayCounter::Impl {
public:
	double Getcvg(const Date& d1, const Date& d2) const { return DayCount(d1, d2) / 360.0; }
	ACT360Impl() { }
};

class DayCounter::Thirty360BImpl : public DayCounter::Impl{
public:
	int DayCount(const Date&, const Date&) const;
	double Getcvg(const Date& d1, const Date& d2) const { return DayCount(d1, d2) / 360.0; }
};

class DayCounter::Thirty360USImpl : public DayCounter::Impl {
public:
	int DayCount(const Date&, const Date&) const;
	double Getcvg(const Date& d1, const Date& d2) const { return DayCount(d1, d2) / 360.0; }
};

class DayCounter::ThirtyE360Impl : public DayCounter::Impl {
public:
	int DayCount(const Date&, const Date&) const;
	double Getcvg(const Date& d1, const Date& d2) const { return DayCount(d1, d2) / 360.0; }
};

int DayCounter::DayCount(const Date& d1, const Date& d2) const { return impl_->DayCount(d1, d2); }
double DayCounter::Getcvg(const Date& d1, const Date& d2) const { return impl_->Getcvg(d1, d2); }

std::shared_ptr<DayCounter::Impl> DayCounter::implementation(DayCountConvention dc)
{
	std::shared_ptr<DayCounter::Impl> impl;
	switch (dc) {
	case ACT365Fixed:
		impl.reset(new DayCounter::ACT365FixedImpl);
		break;
	case ACTACTISDA:
		impl.reset(new DayCounter::ACTACTISDAImpl);
		break;
	case ACT360:
		impl.reset(new DayCounter::ACT360Impl);
		break;
	case Thirty360B:
		impl.reset(new DayCounter::Thirty360BImpl);
		break;
	case Thirty360US:
		impl.reset(new DayCounter::Thirty360USImpl);
		break;
	case ThirtyE360:
		impl.reset(new DayCounter::ThirtyE360Impl);
		break;
	default:
		throw std::domain_error("Unknown Day Count Convention.");
		break;
	}
	return impl;
}

double DayCounter::ACTACTISDAImpl::Getcvg(const Date& d1, const Date& d2) const
{
	if (d1.year() == d2.year()) {
		if (isLeapYear(d1.year()))
			return (d2 - d1).days() / 366.0;
		else
			return (d2 - d1).days() / 365.0;
	}
	else if (d1.year() > d2.year())
		return -Getcvg(d2, d1);
	else {
		int start_year = int(d1.year());
		int end_year = int(d2.year());
		double ret = 0;

		//start_year인 경우, end_year인 경우, 그 이외의 경우로 구분하여서 계산.
		//start_year인 경우 start_year의 12월 31일도 포함하여야 하므로, 분자에 1을 더함.
		if (isLeapYear(start_year))
			ret += ((Date(start_year, 12, 31) - d1).days() + 1.0) / 366.0;
		else
			ret += ((Date(start_year, 12, 31) - d1).days() + 1.0) / 365.0;

		if (isLeapYear(end_year))
			ret += (d2 - Date(end_year, 1, 1)).days() / 366.0;
		else
			ret += (d2 - Date(end_year, 1, 1)).days() / 365.0;

		ret += end_year - start_year - 1.0;

		return ret;
	}
}

int DayCounter::Thirty360BImpl::DayCount(const Date& d1, const Date& d2) const
{
	int day1 = d1.day() == 31 ? 30 : int(d1.day());
	int day2 = d2.day() == 31 && day1 > 29 ? 30 : int(d2.day());
	return 360 * (d2.year() - d1.year()) + 30 * (d2.month() - d1.month()) + (day2 - day1);
}

int DayCounter::Thirty360USImpl::DayCount(const Date& d1, const Date& d2) const
{
	int day1 = d1.month() == 2 && d1.end_of_month() == d1 ? 30 : int(d1.day());
	int day2 = (d1.month() == 2 && d1.end_of_month() == d1) && (d2.month() == 2 && d2.end_of_month() == d2) ? 30 : int(d2.day());
	day2 = day1 > 29 && day2 == 31 ? 30 : day2;
	day1 = day1 == 31 ? 30 : day1;
	return 360 * (d2.year() - d1.year()) + 30 * (d2.month() - d1.month()) + (day2 - day1);
}

int DayCounter::ThirtyE360Impl::DayCount(const Date& d1, const Date& d2) const
{
	int day1 = (d1.day() == 31) ? 30 : int(d1.day());
	int day2 = (d2.day() == 31) ? 30 : int(d2.day());
	return 360 * (d2.year() - d1.year()) + 30 * (d2.month() - d1.month()) + (day2 - day1);
}