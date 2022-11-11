#pragma once
#include <Source/Times/Date.h>
#include <Source/Times/BusinessDayConvention.h>

//Calendar �������̽�
//�ش� ��¥�� ���ؼ� bool ������ ��ȯ�ؾ� �Ǵ� �͵鸸 �������̽��� ����.
//��¥ ������ ���� �Լ��� DateHandling ���Ͽ� ����.
//�ʼ������� ��� �Լ��� �ƴϾ �Ǵ� �Լ��� ������, �������̽� �ϰ��� �������� bool ������ ��ȯ�ϴ� �͵鸸 �־��.

class ICalendar {
public:
	virtual ~ICalendar() = default;

	virtual bool isBusinessDay(const Date&) const = 0;
	virtual bool isHoliday(const Date&) const = 0;
	virtual bool isWeekEnd(const Date&) const = 0;
	virtual bool isEndOfMonth(const Date&) const = 0;
};