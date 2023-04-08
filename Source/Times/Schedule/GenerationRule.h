#pragma once
#include <map>
#include <Source/Times/Schedule/Period.h>

enum class StubRule {
	LongStub,
	ShortStub
};

enum class DrivenDirection {
	Forward,
	Backward
};

enum class DateAdjust {
	Adjusted,
	UnAdjusted
};

//���ο� Date ������ �߰��� ������ Count ������ �߰�
enum class KindsOfDate {
	CalculationStartDate,
	CalculationEndDate,
	FixingDate,
	PaymentDate,
	Count
};