#pragma once
#include <memory>
#include <Source/Times/Date.h>

class DayCounter;

//Rate vs Vol
//Rate -> dcf / dcft / forward rate / annuity 기능 필요
//Rate는 Bootstrap 이용한 fitting. Bootstrap은 Interpolation 객체 필요로 함
//Bootstrap 객체가 반환한 Interpolation 방법론으로 Interpolation하도록 해야 하므로, Bootstrap 객체를 has - a 관계로 구현.
//Vol -> variance / stddev / forward vol 기능 필요.
//Vol은 TermStructure는 거의 안 쓰고 Surface를 주로 사용하고 fitting 방법론도 Surface 위주임.
//거의 안 쓸 것이므로 단순히 Estimator를 has - a 관계로 가지게 해 데이터를 받으면 interpolation해서 value 구하도록.

class TermStructure {
public:
	TermStructure() = default;
	virtual ~TermStructure() = default;
	TermStructure(const Date& curve_date, const std::shared_ptr<DayCounter>& day_counter);

protected:
	Date curve_date_;
	std::shared_ptr<DayCounter> day_counter_;
};