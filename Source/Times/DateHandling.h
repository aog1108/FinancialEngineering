#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/BusinessDayConvention.h>
#include <Source/Times/ICalendar.h>

//��¥ ���� �Լ��� ���� ��� �Լ��� �������� �ʾƵ� ���� �����ϹǷ� ���� �Լ��� ������.

const Date GetGoodDay(const ICalendar&, const Date&, BusinessDayConvention);
const Date DayAdd(const ICalendar&, const Date&, int, BusinessDayConvention);
const Date MonthAdd(const ICalendar&, const Date&, int, BusinessDayConvention, bool eom = false);
const Date YearAdd(const ICalendar&, const Date&, int, BusinessDayConvention, bool eom = false);
const Date EndOfMonth(const ICalendar&, const Date&);