#pragma once
#include <memory>
#include <Source/Times/Date.h>
#include <Source/Times/Schedule/Period.h>

//������ ��¥�� '������'�� �޾Ƽ�, ��¥�� �ٲ� ������, 
//next_date�� ȣ���ϴ� ������ �����쿡 �´� ��¥�� ����

class Relation {
public:
	Relation() = default;
	virtual ~Relation() = default;

	Relation(const std::shared_ptr<Date>& related_date) : related_date_(related_date) { }

	virtual Date next_date(const ICalendar&, BusinessDayConvention, bool eom = false) const = 0;

protected:
	std::shared_ptr<Date> related_date_;
};

class Equalto : public Relation {
public:
	Equalto(const std::shared_ptr<Date>& related_date) : Relation(related_date) { }

	Date next_date(const ICalendar&, BusinessDayConvention, bool eom = false) const override;
};

class DeducedFrom : public Relation {
public:
	DeducedFrom(const std::shared_ptr<Date>& related_date, const std::shared_ptr<Period>& period) : Relation(related_date), period_(period) { }

	Date next_date(const ICalendar&, BusinessDayConvention, bool eom = false) const override;

private:
	std::shared_ptr<Period> period_;
};