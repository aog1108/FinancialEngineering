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

//새로운 Date 종류가 추가될 때마다 Count 이전에 추가
enum class KindsOfDate {
	CalculationStartDate,
	CalculationEndDate,
	FixingDate,
	PaymentDate,
	Count
};