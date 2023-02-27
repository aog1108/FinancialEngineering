#pragma once
#include <map>
#include <Source/Times/Schedule/Period.h>

class Relation;

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

enum class KindsOfDate {
	PaymentDate,
	FixingDate,
	CalculationStartDate,
	CalculationEndDate
};

struct DateRelations {
	using RelationMapping = std::map<KindsOfDate, std::map<KindsOfDate, Relation>>;
	DateRelations(KindsOfDate driven_date, const std::shared_ptr<Period> driven_period, RelationMapping relations)
		: driven_date_(driven_date), driven_period_(driven_period), relations_(relations) { }

	KindsOfDate driven_date_;
	std::shared_ptr<Period> driven_period_;
	RelationMapping relations_;
};