#include "Test.h"
#include <vector>
#include <string>
#include "Times/Date.h"
#include "Times/Calendar.h"
#include "Times/DayCounter.h"
#include "Times/DateHandling.h"
#include "Times/JointCalendar.h"

void TestDate()
{
	int y1, m1, d1, y2, m2, d2;
	std::cin >> y1 >> m1 >> d1 >> y2 >> m2 >> d2;

	Date D1(y1, m1, d1), D2(y2, m2, d2);
	std::vector<bool> b;
	std::vector<std::string> s;

	s.emplace_back("==");
	s.emplace_back("!=");
	s.emplace_back("<");
	s.emplace_back(">");
	s.emplace_back("<=");
	s.emplace_back(">=");
	b.emplace_back(D1 == D2);
	b.emplace_back(D1 != D2);
	b.emplace_back(D1 < D2);
	b.emplace_back(D1 > D2);
	b.emplace_back(D1 <= D2);
	b.emplace_back(D1 >= D2);

	std::cout << D1 - D2 << std::endl;
	for (std::vector<bool>::size_type i = 0; i != b.size(); ++i) {
		std::cout << s[i] << " " << b[i] << std::endl;
	}
}

void TestBoostGregorian()
{
	boost::gregorian::date d(2022, 9, 2);
	boost::gregorian::days s(5);

	std::cout << d + s << std::endl;
}

void TestCalendar()
{
	std::vector<Date> holidays;
	Date d1(2022, 9, 9), d2(2022, 9, 12), d3(2022, 10, 3), d4(2022, 10, 10);
	holidays.emplace_back(d1);
	holidays.emplace_back(d2);
	holidays.emplace_back(d3);
	holidays.emplace_back(d4);

	Date dd(2022, 10, 1), dd2(2022, 12, 31);

	Calendar cal(holidays);

	std::cout << d1 << ": isBusinessDay " << cal.isBusinessDay(d1) << std::endl;
	std::cout << d1 << ": isHoliDay " << cal.isHoliday(d1) << std::endl;
	std::cout << d1 << ": isWeekEnd " << cal.isWeekEnd(d1) << std::endl;
	std::cout << d1 << ": isEndOfMonth " << cal.isEndOfMonth(d1) << std::endl;

	std::cout << Date(2022, 9, 4) << ": isWeekEnd " << cal.isWeekEnd(Date(2022, 9, 4)) << std::endl;
	std::cout << Date(2022, 9, 30) << ": isEndOfMonth " << cal.isEndOfMonth(Date(2022, 9, 30)) << std::endl;

	std::cout << d1 << ": Unadjusted " << GetGoodDay(cal, d1, Unadjusted) << std::endl;
	std::cout << d1 << ": Following " << GetGoodDay(cal, d1, Following) << std::endl;
	std::cout << d1 << ": ModifiedFollowing " << GetGoodDay(cal, d1, ModifiedFollowing) << std::endl;
	std::cout << d1 << ": Preceding " << GetGoodDay(cal, d1, Preceding) << std::endl;
	std::cout << d1 << ": ModifiedPreceding " << GetGoodDay(cal, d1, ModifiedPreceding) << std::endl;

	std::cout << dd << ": Preceding " << GetGoodDay(cal, dd, Preceding) << std::endl;
	std::cout << dd << ": ModifiedPreceding " << GetGoodDay(cal, dd, ModifiedPreceding) << std::endl;
	std::cout << dd2 << ": Following " << GetGoodDay(cal, dd2, Following) << std::endl;
	std::cout << dd2 << ": ModifiedFollowing " << GetGoodDay(cal, dd2, ModifiedFollowing) << std::endl;

	Date di(2022, 9, 5);

	std::cout << di << ": DayAdd 5 Unadjusted " << DayAdd(cal, di, 5, Unadjusted) << std::endl;
	std::cout << di << ": DayAdd 5 with holidays " << DayAdd(cal, di, 5, Following) << std::endl;
	std::cout << di << ": DayAdd -8 Unadjusted " << DayAdd(cal, di, -8, Unadjusted) << std::endl;
	std::cout << di << ": DayAdd -8 " << DayAdd(cal, di, -8, Following) << std::endl;
	std::cout << di << ": MonthAdd 2 with Following " << MonthAdd(cal, di, 2, Following) << std::endl;
	std::cout << di << ": MonthAdd 2 with Preceding " << MonthAdd(cal, di, 2, Preceding) << std::endl;
	std::cout << di << ": MonthAdd -3 with Following " << MonthAdd(cal, di, -3, Following) << std::endl;
	std::cout << di << ": MonthAdd -3 with Preceding " << MonthAdd(cal, di, -3, Preceding) << std::endl;
	std::cout << di << ": YearAdd 4 with Following " << YearAdd(cal, di, 4, Following) << std::endl;
	std::cout << di << ": YearAdd 4 with Preceding " << YearAdd(cal, di, 4, Preceding) << std::endl;
	std::cout << di << ": YearAdd -1 with Following " << YearAdd(cal, di, -1, Following) << std::endl;
	std::cout << di << ": YearAdd -1 with Preceding " << YearAdd(cal, di, -1, Preceding) << std::endl;
	std::cout << di << ": EndOfMonth " << EndOfMonth(cal, di) << std::endl;

	Date di2(2022, 1, 31);

	std::cout << di2 << ": MonthAdd 3 Following " << MonthAdd(cal, di2, 3, Following) << std::endl;
	std::cout << di2 << ": MonthAdd 3 Following with eom true " << MonthAdd(cal, di2, 3, Following, true) << std::endl;
	std::cout << Date(2022, 4, 29) << ": isEndOfMonth " << cal.isEndOfMonth(Date(2022, 4, 29)) << std::endl;
}

void TestJointCalendar()
{
	std::vector<Date> holidays1;
	Date d1(2022, 9, 9), d2(2022, 9, 12), d3(2022, 10, 3), d4(2022, 10, 10);
	holidays1.emplace_back(d1);
	holidays1.emplace_back(d2);
	holidays1.emplace_back(d3);
	holidays1.emplace_back(d4);

	Date dd(2022, 10, 1), dd2(2022, 12, 31);

	std::vector<Date> holidays2;
	holidays2.emplace_back(Date(2022, 9, 5));
	holidays2.emplace_back(Date(2022, 10, 10));

	JointCalendar cal;
	cal.addCalendar("SouthKorea",holidays1);
	cal.addCalendar("UnitedStates", holidays2);
	std::cout << d1 << ": isBusinessDay " << cal.isBusinessDay(d1) << std::endl;
	std::cout << d1 << ": isHoliDay " << cal.isHoliday(d1) << std::endl;
	std::cout << d1 << ": isWeekEnd " << cal.isWeekEnd(d1) << std::endl;
	std::cout << d1 << ": isEndOfMonth " << cal.isEndOfMonth(d1) << std::endl;

	std::cout << Date(2022, 9, 4) << ": isWeekEnd " << cal.isWeekEnd(Date(2022, 9, 4)) << std::endl;
	std::cout << Date(2022, 9, 30) << ": isEndOfMonth " << cal.isEndOfMonth(Date(2022, 9, 30)) << std::endl;

	std::cout << d1 << ": Unadjusted " << GetGoodDay(cal, d1, Unadjusted) << std::endl;
	std::cout << d1 << ": Following " << GetGoodDay(cal, d1, Following) << std::endl;
	std::cout << d1 << ": ModifiedFollowing " << GetGoodDay(cal, d1, ModifiedFollowing) << std::endl;
	std::cout << d1 << ": Preceding " << GetGoodDay(cal, d1, Preceding) << std::endl;
	std::cout << d1 << ": ModifiedPreceding " << GetGoodDay(cal, d1, ModifiedPreceding) << std::endl;

	std::cout << dd << ": Preceding " << GetGoodDay(cal, dd, Preceding) << std::endl;
	std::cout << dd << ": ModifiedPreceding " << GetGoodDay(cal, dd, ModifiedPreceding) << std::endl;
	std::cout << dd2 << ": Following " << GetGoodDay(cal, dd2, Following) << std::endl;
	std::cout << dd2 << ": ModifiedFollowing " << GetGoodDay(cal, dd2, ModifiedFollowing) << std::endl;

	Date di(2022, 9, 5);

	std::cout << di << ": DayAdd 5 Unadjusted " << DayAdd(cal, di, 5, Unadjusted) << std::endl;
	std::cout << di << ": DayAdd 5 with holidays " << DayAdd(cal, di, 5, Following) << std::endl;
	std::cout << di << ": DayAdd -8 Unadjusted " << DayAdd(cal, di, -8, Unadjusted) << std::endl;
	std::cout << di << ": DayAdd -8 " << DayAdd(cal, di, -8, Following) << std::endl;
	std::cout << di << ": MonthAdd 2 with Following " << MonthAdd(cal, di, 2, Following) << std::endl;
	std::cout << di << ": MonthAdd 2 with Preceding " << MonthAdd(cal, di, 2, Preceding) << std::endl;
	std::cout << di << ": MonthAdd -3 with Following " << MonthAdd(cal, di, -3, Following) << std::endl;
	std::cout << di << ": MonthAdd -3 with Preceding " << MonthAdd(cal, di, -3, Preceding) << std::endl;
	std::cout << di << ": YearAdd 4 with Following " << YearAdd(cal, di, 4, Following) << std::endl;
	std::cout << di << ": YearAdd 4 with Preceding " << YearAdd(cal, di, 4, Preceding) << std::endl;
	std::cout << di << ": YearAdd -1 with Following " << YearAdd(cal, di, -1, Following) << std::endl;
	std::cout << di << ": YearAdd -1 with Preceding " << YearAdd(cal, di, -1, Preceding) << std::endl;
	std::cout << di << ": EndOfMonth " << EndOfMonth(cal, di) << std::endl;

	Date di2(2022, 1, 31);

	std::cout << di2 << ": MonthAdd 3 Following " << MonthAdd(cal, di2, 3, Following) << std::endl;
	std::cout << di2 << ": MonthAdd 3 Following with eom true " << MonthAdd(cal, di2, 3, Following, true) << std::endl;
	std::cout << Date(2022, 4, 29) << ": isEndOfMonth " << cal.isEndOfMonth(Date(2022, 4, 29)) << std::endl;

	Date di3(2022, 9, 2);

	std::cout << di3 << ": DayAdd 2 Following " << DayAdd(cal, di3, 2, Following) << std::endl;
}

void TestDayCounter()
{
	DayCounter dc(ACT365Fixed);
	
	Date d1(2020, 3, 5), d2(2022, 9, 13);

	std::cout << "ACT365Fixed Name: " << dc.GetName() << std::endl;
	std::cout << "ACT365Fixed DayCount: " << dc.DayCount(d1, d2) << std::endl;
	std::cout << "ACT365Fixed CVG: " << dc.Getcvg(d1, d2) << std::endl;
	std::cout << "ACT365Fixed CVG: " << dc.Getcvg(d2, d1) << std::endl;

	std::cout << std::endl;
	dc = DayCounter(ACT360);
	std::cout << "ACT360 Name: " << dc.GetName() << std::endl;
	std::cout << "ACT360 DayCount: " << dc.DayCount(d1, d2) << std::endl;
	std::cout << "ACT360 CVG: " << dc.Getcvg(d1, d2) << std::endl;
	std::cout << "ACT360 CVG: " << dc.Getcvg(d2, d1) << std::endl;

	std::cout << std::endl;
	dc = DayCounter(ACTACTISDA);
	std::cout << "ACTACTISDA Name: " << dc.GetName() << std::endl;
	std::cout << "ACTACTISDA DayCount: " << dc.DayCount(d1, d2) << std::endl;
	std::cout << "ACTACTISDA CVG: " << dc.Getcvg(d1, d2) << std::endl;
	std::cout << "ACTACTISDA CVG: " << dc.Getcvg(d2, d1) << std::endl;

	std::cout << std::endl;
	dc = DayCounter(Thirty360B);
	std::cout << "30/360B Name: " << dc.GetName() << std::endl;
	std::cout << "30/360B DayCount: " << dc.DayCount(d1, d2) << std::endl;
	std::cout << "30/360B CVG: " << dc.Getcvg(d1, d2) << std::endl;
	std::cout << "30/360B CVG: " << dc.Getcvg(d2, d1) << std::endl;
}