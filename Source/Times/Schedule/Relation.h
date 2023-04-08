#pragma once
#include <memory>
#include <Source/Times/Date.h>
#include <Source/Times/Schedule/Period.h>

enum class KindsOfDate;

class Relation {
public:
	//std::pair<A, B> : A는 B로부터 만들어진다는 의미
	using RelationPair = std::pair<KindsOfDate, KindsOfDate>;

	Relation() = default;
	virtual ~Relation() = default;
	Relation(const RelationPair& relation_pair) : relation_pair_(relation_pair) { }

	virtual std::vector<Date> applyRelation(const ICalendar&, const std::vector<Date>& dates, BusinessDayConvention, bool eom = false) const = 0;
	virtual std::string relationName() = 0; 

	const RelationPair& getRelationPair() { return relation_pair_; }

	void setDrivenDates(std::vector<Date>* driven_dates) { driven_dates_ = driven_dates; }

protected:
	RelationPair relation_pair_;
	std::vector<Date>* driven_dates_;
};

class Equalto : public Relation {
public:
	Equalto() = default;
	Equalto(const RelationPair& relation_pair) : Relation(relation_pair) { }

	std::vector<Date> applyRelation(const ICalendar&, const std::vector<Date>& dates, BusinessDayConvention, bool eom = false) const override;
	std::string relationName() { return "Equalto"; }
};

class DeducedFrom : public Relation {
public:
	DeducedFrom() = default;
	DeducedFrom(const RelationPair& relation_pair, const std::shared_ptr<Period> period) 
		: Relation(relation_pair), period_(period) { }

	std::vector<Date> applyRelation(const ICalendar&, const std::vector<Date>& dates, BusinessDayConvention, bool eom = false) const override;
	std::string relationName() { return "DeducedFrom"; }

private:
	std::shared_ptr<Period> period_;
};