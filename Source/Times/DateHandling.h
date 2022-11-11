#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/BusinessDayConvention.h>
#include <Source/Times/ICalendar.h>

//날짜 연산 함수는 굳이 멤버 함수로 구현하지 않아도 구현 가능하므로 비멤버 함수로 구현함.

const Date GetGoodDay(const ICalendar&, const Date&, BusinessDayConvention);
const Date DayAdd(const ICalendar&, const Date&, int, BusinessDayConvention);
const Date MonthAdd(const ICalendar&, const Date&, int, BusinessDayConvention, bool eom = false);
const Date YearAdd(const ICalendar&, const Date&, int, BusinessDayConvention, bool eom = false);
const Date EndOfMonth(const ICalendar&, const Date&);