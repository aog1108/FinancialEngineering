#pragma once
#include <vector>
#include <memory>
#include <Source/Times/Date.h>

enum class StubRule;
enum class DrivenDirection;
enum class KindsOfDate;

class DrivenDateSelector {
public:
	DrivenDateSelector() = default;
	virtual ~DrivenDateSelector() = default;
	
	DrivenDateSelector(StubRule stub_rule, DrivenDirection direction) : stub_rule_(stub_rule), direction_(direction) { }

	virtual std::vector<Date> select(const std::vector<Date>& driven_dates, bool exist_stub) const = 0;

protected:
	StubRule stub_rule_;
	DrivenDirection direction_;
};

class CalculationStartDateSelector : public DrivenDateSelector {
public:
	CalculationStartDateSelector() = default;
	CalculationStartDateSelector(StubRule stub_rule, DrivenDirection direction) : DrivenDateSelector(stub_rule, direction) { }

	std::vector<Date> select(const std::vector<Date>& driven_dates, bool exist_stub) const override;
};

class CalculationEndDateSelector : public DrivenDateSelector {
public:
	CalculationEndDateSelector() = default;
	CalculationEndDateSelector(StubRule stub_rule, DrivenDirection direction) : DrivenDateSelector(stub_rule, direction) { }

	std::vector<Date> select(const std::vector<Date>& driven_dates, bool exist_stub) const override;
};

//FixingDateDriven�̸鼭 fixed in arrears ������ ��츦 ó���ϱ� ���� isFixedInArrears ������ �ʿ�.
//Driven date�� fixing date�� �ƴ� ���, relation�� ������ �����Ͽ� fixed in arrears ���� ������ ���� ����.
class FixingDateSelector : public DrivenDateSelector {
public:
	FixingDateSelector() = default;
	FixingDateSelector(StubRule stub_rule, DrivenDirection direction, bool isFixedInArrears) 
		: DrivenDateSelector(stub_rule, direction), isFixedInArrears_(isFixedInArrears) { }

	std::vector<Date> select(const std::vector<Date>& driven_dates, bool exist_stub) const override;

private:
	bool isFixedInArrears_;
};

class PaymentDateSelector : public DrivenDateSelector {
public:
	PaymentDateSelector() = default;
	PaymentDateSelector(StubRule stub_rule, DrivenDirection direction) : DrivenDateSelector(stub_rule, direction) { }

	std::vector<Date> select(const std::vector<Date>& driven_dates, bool exist_stub) const override;
};

std::vector<Date> frontDrivenDateSelect(const std::vector<Date>& driven_dates, StubRule stub_rule, DrivenDirection direction, bool exist_stub);

std::vector<Date> backDrivenDateSelect(const std::vector<Date>& driven_dates, StubRule stub_rule, DrivenDirection direction, bool exist_stub);

std::shared_ptr<DrivenDateSelector> generateDrivenDateSelector(KindsOfDate kinds_of_date, StubRule stub_rule, DrivenDirection direction, bool isFixedInArrears = false);