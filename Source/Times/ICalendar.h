#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/BusinessDayConvention.h>

//Calendar 인터페이스
//해당 날짜에 대해서 bool 변수로 반환해야 되는 것들만 인터페이스로 구성.
//날짜 연산은 비멤버 함수로 DateHandling 파일에 존재.
//필수적으로 멤버 함수가 아니어도 되는 함수도 있지만, 인터페이스 일관성 목적으로 bool 변수로 반환하는 것들만 넣어둠.

class ICalendar {
public:
	virtual ~ICalendar() = default;

	virtual bool isBusinessDay(const Date&) const = 0;
	virtual bool isHoliday(const Date&) const = 0;
	virtual bool isWeekEnd(const Date&) const = 0;
	virtual bool isEndOfMonth(const Date&) const = 0;
};