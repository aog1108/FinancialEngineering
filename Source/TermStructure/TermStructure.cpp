#include <Source/TermStructure/TermStructure.h>
#include <Source/Times/DayCounter.h>

TermStructure::TermStructure(const Date& curve_date, const std::shared_ptr<DayCounter>& daycounter)
	: curve_date_(curve_date), day_counter_(daycounter) { }