#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include <Source/Times/Date.h>

//DayCountConvention�� ���� ���� ����� �����ؼ� �ø��� �ͺ��� �ϳ��� ����� �� ����ϴ� ���� ������.
//�긴���������� ����.
//DayCountConvention �߰� �ʿ� �� ��� ���Ͽ� ���ο� Impl ���� �� cpp�� ���� �߰��ϸ� ��.
//�����ϴ� ���� ���� ���� �� ��� ��.
enum DayCountConvention {
	ACT365Fixed = 1,
	ACTACTISDA = 2,
	ACT360 = 3,
	Thirty360B = 4,
	Thirty360US = 5,
	ThirtyE360 = 6
};

class DayCounter {
public:
	DayCounter() { }
	explicit DayCounter(DayCountConvention dc) : impl_(std::move(implementation(dc))), name_(name_mapping_.at(dc)) { }

	const std::string& GetName() const { return name_; }
	int DayCount(const Date& d1, const Date& d2) const;
	double Getcvg(const Date& d1, const Date& d2) const;

private:
	class Impl;
	class ACT365FixedImpl;
	class ACTACTISDAImpl;
	class ACT360Impl;
	class Thirty360BImpl;
	class Thirty360USImpl;
	class ThirtyE360Impl;

	std::shared_ptr<DayCounter::Impl> implementation(DayCountConvention dc);
	
	std::shared_ptr<DayCounter::Impl> impl_;
	std::string name_;
	const static std::map<DayCountConvention, std::string> name_mapping_;
};