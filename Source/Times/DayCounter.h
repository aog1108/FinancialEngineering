#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include <Source/Times/Date.h>

//DayCountConvention에 따라서 별도 헤더로 정의해서 늘리는 것보다 하나의 헤더로 다 사용하는 것을 선택함.
//브릿지패턴으로 구현.
//DayCountConvention 추가 필요 시 헤더 파일에 새로운 Impl 선언 후 cpp에 구현 추가하면 됨.
//빌드하는 것을 따로 빼줄 지 고민 중.
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