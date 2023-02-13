#pragma once
#include <memory>
#include <Source/Times/Date.h>

class DayCounter;

//Rate vs Vol
//Rate -> dcf / dcft / forward rate / annuity ��� �ʿ�
//Rate�� Bootstrap �̿��� fitting. Bootstrap�� Interpolation ��ü �ʿ�� ��
//Bootstrap ��ü�� ��ȯ�� Interpolation ��������� Interpolation�ϵ��� �ؾ� �ϹǷ�, Bootstrap ��ü�� has - a ����� ����.
//Vol -> variance / stddev / forward vol ��� �ʿ�.
//Vol�� TermStructure�� ���� �� ���� Surface�� �ַ� ����ϰ� fitting ����е� Surface ������.
//���� �� �� ���̹Ƿ� �ܼ��� Estimator�� has - a ����� ������ �� �����͸� ������ interpolation�ؼ� value ���ϵ���.

class TermStructure {
public:
	TermStructure() = default;
	virtual ~TermStructure() = default;
	TermStructure(const Date& curve_date, const std::shared_ptr<DayCounter>& day_counter);

protected:
	Date curve_date_;
	std::shared_ptr<DayCounter> day_counter_;
};