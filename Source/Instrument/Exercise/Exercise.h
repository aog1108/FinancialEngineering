#pragma once
#include <Source/Times/Date.h>

class Exercise {
public:
	enum Type { European, American, Bermudan };
	
	Exercise() { }
	virtual ~Exercise() { }

	explicit Exercise(Type type) : type_(type) { }
	Type type() const { return type_; }
	Date date(std::size_t index) const { return dates_[index]; }
	Date dateat(std::size_t index) const { return dates_.at(index); }
	const std::vector<Date>& dates() const { return dates_; }

protected:
	Type type_;
	std::vector<Date> dates_;
};

class EuropeanExercise : public Exercise {
public:
	EuropeanExercise(const Date& date) : Exercise(European) { dates_.push_back(date); }
};