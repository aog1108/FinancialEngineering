#include "Test.h"
#include <vector>
#include <string>
#include "Times/Date.h"
#include "Times/Calendar.h"
#include "Times/DayCounter.h"
#include "Times/DateHandling.h"
#include "Times/JointCalendar.h"
#include "Math/NormalDistribution.h"
#include "PricingEngine/BlackCalculator.h"
#include "Settings.h"
#include "Instrument/EuropeanOption.h"
#include "PricingEngine/BlackEuropeanOptionEngine.h"
#include "PricingEngine/QuantoBlackEuropeanOptionEngine.h"

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

void TestNormalDistribution()
{
	NormalDistribution dist(0.0, 1.0);
	
	std::cout << dist.cdf(0.0) << std::endl;
	std::cout << dist.pdf(0.0) << std::endl;
	std::cout << dist.cdf(1.96) << std::endl;
	std::cout << dist.pdf(1.96) << std::endl;
}

void TestBlackCalculator()
{
	DayCounter dc(ACT365Fixed);
	double ttm = dc.Getcvg(Date(2020, 6, 14), Date(2023, 3, 2));
	double K = 2452;
	std::shared_ptr<PlainVanillaPayoff> payoff1(new PlainVanillaPayoff(Call, K));
	double S = 2553;
	double F = 2553 * std::exp((0.02 - 0.015) * ttm);
	double discount = std::exp(-0.02 * ttm);
	double stddev = 0.3543 * std::sqrt(ttm);
	double variance = stddev * stddev;
	BlackCalculator calculator(payoff1, F, discount, stddev, variance);
	
	std::cout << std::fixed;
	std::cout.precision(13);
	std::cout << "PlainVanilla Call Value: " << calculator.value() << std::endl;
	std::cout << "PlainVanilla Call Delta: " << calculator.delta(S) << std::endl;
	std::cout << "PlainVanilla Call Gamma: " << calculator.gamma(S) << std::endl;
	std::cout << "PlainVanilla Call Vega: " << calculator.vega(ttm) << std::endl;
	std::cout << "PlainVanilla Call Theta: " << calculator.theta(S, ttm) << std::endl;
	std::cout << "PlainVanilla Call Rho: " << calculator.rho(ttm) << std::endl;
	std::cout << "PlainVanilla Call Epsilon: " << calculator.epsilon(ttm) << std::endl;

	std::shared_ptr<CashOrNothingPayoff> payoff2(new CashOrNothingPayoff(Call, K, 1.0));
	BlackCalculator calculator2(payoff2, F, discount, stddev, variance);

	std::cout << "CashOrNothing Call Value: " << calculator2.value() << std::endl;
	std::cout << "CashOrNothing Call Delta: " << calculator2.delta(S) << std::endl;
	std::cout << "CashOrNothing Call Gamma: " << calculator2.gamma(S) << std::endl;
	std::cout << "CashOrNothing Call Vega: " << calculator2.vega(ttm) << std::endl;
	std::cout << "CashOrNothing Call Theta: " << calculator2.theta(S, ttm) << std::endl;
	std::cout << "CashOrNothing Call Rho: " << calculator2.rho(ttm) << std::endl;
	std::cout << "CashOrNothing Call Epsilon: " << calculator2.epsilon(ttm) << std::endl;

	std::shared_ptr<AssetOrNothingPayoff> payoff3(new AssetOrNothingPayoff(Call, K));
	BlackCalculator calculator3(payoff3, F, discount, stddev, variance);

	std::cout << "AssetOrNothing Call Value: " << calculator3.value() << std::endl;
	std::cout << "AssetOrNothing Call Delta: " << calculator3.delta(S) << std::endl;
	std::cout << "AssetOrNothing Call Gamma: " << calculator3.gamma(S) << std::endl;
	std::cout << "AssetOrNothing Call Vega: " << calculator3.vega(ttm) << std::endl;
	std::cout << "AssetOrNothing Call Theta: " << calculator3.theta(S, ttm) << std::endl;
	std::cout << "AssetOrNothing Call Rho: " << calculator3.rho(ttm) << std::endl;
	std::cout << "AssetOrNothing Call Epsilon: " << calculator3.epsilon(ttm) << std::endl;

	payoff1 = std::make_shared<PlainVanillaPayoff>(Put,2452);
	payoff2 = std::make_shared<CashOrNothingPayoff>(Put, 2452, 1.0);
	payoff3 = std::make_shared<AssetOrNothingPayoff>(Put, 2452);
	calculator = BlackCalculator(payoff1, F, discount, stddev, variance);
	calculator2 = BlackCalculator(payoff2, F, discount, stddev, variance);
	calculator3 = BlackCalculator(payoff3, F, discount, stddev, variance);

	std::cout << "PlainVanilla Put Value: " << calculator.value() << std::endl;
	std::cout << "PlainVanilla Put Delta: " << calculator.delta(S) << std::endl;
	std::cout << "PlainVanilla Put Gamma: " << calculator.gamma(S) << std::endl;
	std::cout << "PlainVanilla Put Vega: " << calculator.vega(ttm) << std::endl;
	std::cout << "PlainVanilla Put Theta: " << calculator.theta(S, ttm) << std::endl;
	std::cout << "PlainVanilla Put Rho: " << calculator.rho(ttm) << std::endl;
	std::cout << "PlainVanilla Put Epsilon: " << calculator.epsilon(ttm) << std::endl;

	std::cout << "CashOrNothing Put Value: " << calculator2.value() << std::endl;
	std::cout << "CashOrNothing Put Delta: " << calculator2.delta(S) << std::endl;
	std::cout << "CashOrNothing Put Gamma: " << calculator2.gamma(S) << std::endl;
	std::cout << "CashOrNothing Put Vega: " << calculator2.vega(ttm) << std::endl;
	std::cout << "CashOrNothing Put Theta: " << calculator2.theta(S, ttm) << std::endl;
	std::cout << "CashOrNothing Put Rho: " << calculator2.rho(ttm) << std::endl;
	std::cout << "CashOrNothing Put Epsilon: " << calculator2.epsilon(ttm) << std::endl;

	std::cout << "AssetOrNothing Put Value: " << calculator3.value() << std::endl;
	std::cout << "AssetOrNothing Put Delta: " << calculator3.delta(S) << std::endl;
	std::cout << "AssetOrNothing Put Gamma: " << calculator3.gamma(S) << std::endl;
	std::cout << "AssetOrNothing Put Vega: " << calculator3.vega(ttm) << std::endl;
	std::cout << "AssetOrNothing Put Theta: " << calculator3.theta(S, ttm) << std::endl;
	std::cout << "AssetOrNothing Put Rho: " << calculator3.rho(ttm) << std::endl;
	std::cout << "AssetOrNothing Put Epsilon: " << calculator3.epsilon(ttm) << std::endl;
}

void TestBlackEuropeanEngine()
{
	Settings::get().ValueDate() = Date(2020, 6, 4);
	std::shared_ptr<DayCounter> dc = std::make_shared<DayCounter>(ACT365Fixed);
	std::shared_ptr<ICalendar> cal = std::make_shared<Calendar>();
	double K = 2452;
	std::shared_ptr<PlainVanillaPayoff> payoff1 = std::make_shared<PlainVanillaPayoff>(PlainVanillaPayoff(Call, K));
	std::shared_ptr<EuropeanExercise> exercise = std::make_shared<EuropeanExercise>(EuropeanExercise(Date(2023, 3, 2)));
	std::shared_ptr<EuropeanOption> option = std::make_shared<EuropeanOption>(EuropeanOption(cal, dc, payoff1, exercise));

	BlackEuropeanOptionEngine engine(option, 2553, 0.02, 0.015, 0.3543);
	engine.calculate();
	Results results = engine.GetResults();

	std::cout << std::fixed;
	std::cout.precision(13);
	std::cout << "PlainVanilla Call Value: " << results.value_ << std::endl;
	std::cout << "PlainVanilla Call Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "PlainVanilla Call Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "PlainVanilla Call Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "PlainVanilla Call Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "PlainVanilla Call Rho: " << boost::any_cast<double>(results.additionalResults["rho"]) << std::endl;
	std::cout << "PlainVanilla Call Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;

	std::shared_ptr<CashOrNothingPayoff> payoff2 = std::make_shared<CashOrNothingPayoff>(CashOrNothingPayoff(Call, K, 1.0));
	std::shared_ptr<EuropeanOption> option2 = std::make_shared<EuropeanOption>(EuropeanOption(cal, dc, payoff2, exercise));
	engine.setupInstruments(option2);
	engine.calculate();
	results = engine.GetResults();

	std::cout << "CashOrNothing Call Value: " << results.value_ << std::endl;
	std::cout << "CashOrNothing Call Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "CashOrNothing Call Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "CashOrNothing Call Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "CashOrNothing Call Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "CashOrNothing Call Rho: " << boost::any_cast<double>(results.additionalResults["rho"]) << std::endl;
	std::cout << "CashOrNothing Call Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;

	std::shared_ptr<AssetOrNothingPayoff> payoff3 = std::make_shared<AssetOrNothingPayoff>(AssetOrNothingPayoff(Call, K));
	std::shared_ptr<EuropeanOption> option3 = std::make_shared<EuropeanOption>(EuropeanOption(cal, dc, payoff3, exercise));
	engine.setupInstruments(option3);
	engine.calculate();
	results = engine.GetResults();

	std::cout << "AssetOrNothing Call Value: " << results.value_ << std::endl;
	std::cout << "AssetOrNothing Call Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "AssetOrNothing Call Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "AssetOrNothing Call Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "AssetOrNothing Call Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "AssetOrNothing Call Rho: " << boost::any_cast<double>(results.additionalResults["rho"]) << std::endl;
	std::cout << "AssetOrNothing Call Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;

	option->payoff_ = std::make_shared<PlainVanillaPayoff>(PlainVanillaPayoff(Put, K));
	engine.setupInstruments(option);
	engine.calculate();
	results = engine.GetResults();

	std::cout << "PlainVanilla Put Value: " << results.value_ << std::endl;
	std::cout << "PlainVanilla Put Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "PlainVanilla Put Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "PlainVanilla Put Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "PlainVanilla Put Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "PlainVanilla Put Rho: " << boost::any_cast<double>(results.additionalResults["rho"]) << std::endl;
	std::cout << "PlainVanilla Put Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;

	option2->payoff_ = std::make_shared<CashOrNothingPayoff>(CashOrNothingPayoff(Put, K, 1.0));
	engine.setupInstruments(option2);
	engine.calculate();
	results = engine.GetResults();

	std::cout << "CashOrNothing Put Value: " << results.value_ << std::endl;
	std::cout << "CashOrNothing Put Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "CashOrNothing Put Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "CashOrNothing Put Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "CashOrNothing Put Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "CashOrNothing Put Rho: " << boost::any_cast<double>(results.additionalResults["rho"]) << std::endl;
	std::cout << "CashOrNothing Put Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;

	option3->payoff_ = std::make_shared<AssetOrNothingPayoff>(AssetOrNothingPayoff(Put, K));
	engine.setupInstruments(option3);
	engine.calculate();
	results = engine.GetResults();

	std::cout << "AssetOrNothing Put Value: " << results.value_ << std::endl;
	std::cout << "AssetOrNothing Put Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "AssetOrNothing Put Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "AssetOrNothing Put Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "AssetOrNothing Put Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "AssetOrNothing Put Rho: " << boost::any_cast<double>(results.additionalResults["rho"]) << std::endl;
	std::cout << "AssetOrNothing Put Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;
}

void TestQuantoBlackEuropeanOptionEngine()
{
	Settings::get().ValueDate() = Date(2020, 6, 4);
	std::shared_ptr<DayCounter> dc = std::make_shared<DayCounter>(ACT365Fixed);
	std::shared_ptr<ICalendar> cal = std::make_shared<Calendar>();
	double K = 2452;
	double sigma_fx = 0.15, rho_fx = -0.3;
	std::shared_ptr<PlainVanillaPayoff> payoff1 = std::make_shared<PlainVanillaPayoff>(PlainVanillaPayoff(Call, K));
	std::shared_ptr<EuropeanExercise> exercise = std::make_shared<EuropeanExercise>(EuropeanExercise(Date(2023, 3, 2)));
	std::shared_ptr<EuropeanOption> option = std::make_shared<EuropeanOption>(EuropeanOption(cal, dc, payoff1, exercise));

	QuantoBlackEuropeanOptionEngine engine(option, 2553, 0.02, 0.02, 0.015, 0.3543, sigma_fx, rho_fx);
	engine.calculate();
	Results results = engine.GetResults();

	std::cout << std::fixed;
	std::cout.precision(13);
	std::cout << "PlainVanilla Call Value: " << results.value_ << std::endl;
	std::cout << "PlainVanilla Call Delta: " << boost::any_cast<double>(results.additionalResults["delta"]) << std::endl;
	std::cout << "PlainVanilla Call Gamma: " << boost::any_cast<double>(results.additionalResults["gamma"]) << std::endl;
	std::cout << "PlainVanilla Call Vega: " << boost::any_cast<double>(results.additionalResults["vega"]) << std::endl;
	std::cout << "PlainVanilla Call Theta: " << boost::any_cast<double>(results.additionalResults["theta"]) << std::endl;
	std::cout << "PlainVanilla Call Discount Rho: " << boost::any_cast<double>(results.additionalResults["discount_rho"]) << std::endl;
	std::cout << "PlainVanilla Call Drift Rho: " << boost::any_cast<double>(results.additionalResults["drift_rho"]) << std::endl;
	std::cout << "PlainVanilla Call Epsilon: " << boost::any_cast<double>(results.additionalResults["epsilon"]) << std::endl;
	std::cout << "PlainVanilla Call Fx Vega: " << boost::any_cast<double>(results.additionalResults["fx_vega"]) << std::endl;
	std::cout << "PlainVanilla Call Quanto Cega: " << boost::any_cast<double>(results.additionalResults["quanto_cega"]) << std::endl;
}

void TestQuantoBlackEuropeanOptionEngineGreeks()
{
	Settings::get().ValueDate() = Date(2020, 6, 4);
	std::shared_ptr<DayCounter> dc = std::make_shared<DayCounter>(ACT365Fixed);
	std::shared_ptr<ICalendar> cal = std::make_shared<Calendar>();
	double K = 2452;
	double sigma_fx = 0.15, rho_fx = -0.3;
	std::shared_ptr<PlainVanillaPayoff> payoff1 = std::make_shared<PlainVanillaPayoff>(PlainVanillaPayoff(Call, K));
	std::shared_ptr<EuropeanExercise> exercise = std::make_shared<EuropeanExercise>(EuropeanExercise(Date(2023, 3, 2)));
	std::shared_ptr<EuropeanOption> option = std::make_shared<EuropeanOption>(EuropeanOption(cal, dc, payoff1, exercise));

	double S = 2553, r = 0.02, rf = 0.02, q = 0.015, sigma = 0.3543;
	QuantoBlackEuropeanOptionEngine engine(option, S, r, rf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	Results original_results = engine.GetResults();

	double up_result, down_result;
	//delta, gamma
	double dS = 0.01;
	engine.setupParameter(S + dS, r, rf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S - dS, r, rf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Delta: " << boost::any_cast<double>(original_results.additionalResults["delta"]);
	std::cout << ", Estimated Delta: " << (up_result - down_result) / (2 * dS) << std::endl;

	std::cout << "Analytic Gamma: " << boost::any_cast<double>(original_results.additionalResults["gamma"]);
	std::cout << ", Estimated Gamma: " << (up_result - 2 * original_results.value_ + down_result) / (dS * dS) << std::endl;

	//vega
	double dsigma = 0.01;
	engine.setupParameter(S, r, rf, q, sigma + dsigma, sigma_fx, rho_fx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S , r, rf, q, sigma - dsigma, sigma_fx, rho_fx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Vega: " << boost::any_cast<double>(original_results.additionalResults["vega"]);
	std::cout << ", Estimated Vega: " << (up_result - down_result) / (2 * dsigma) << std::endl;

	//discount rho
	double dr = 0.0001;
	engine.setupParameter(S, r + dr, rf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S, r - dr, rf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Discount Rho: " << boost::any_cast<double>(original_results.additionalResults["discount_rho"]);
	std::cout << ", Estimated Discount Rho: " << (up_result - down_result) / (2 * dr) << std::endl;

	//drift rho
	double drf = 0.0001;
	engine.setupParameter(S, r, rf + drf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S, r, rf - drf, q, sigma, sigma_fx, rho_fx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Drift Rho: " << boost::any_cast<double>(original_results.additionalResults["drift_rho"]);
	std::cout << ", Estimated Drift Rho: " << (up_result - down_result) / (2 * drf) << std::endl;

	//epsilon
	double dq = 0.0001;
	engine.setupParameter(S, r, rf, q + dq, sigma, sigma_fx, rho_fx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S, r, rf, q - dq, sigma, sigma_fx, rho_fx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Epsilon: " << boost::any_cast<double>(original_results.additionalResults["epsilon"]);
	std::cout << ", Estimated Epsilon: " << (up_result - down_result) / (2 * drf) << std::endl;

	//fx vega
	double dsigmafx = 0.01;
	engine.setupParameter(S, r, rf, q, sigma, sigma_fx + dsigmafx, rho_fx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S, r, rf, q, sigma, sigma_fx - dsigmafx, rho_fx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Fx Vega: " << boost::any_cast<double>(original_results.additionalResults["fx_vega"]);
	std::cout << ", Estimated Fx Vega: " << (up_result - down_result) / (2 * dsigmafx) << std::endl;

	//quanto cega
	double drhofx = 0.0001;
	engine.setupParameter(S, r, rf, q, sigma, sigma_fx, rho_fx + drhofx);
	engine.calculate();
	up_result = engine.GetResults().value_;

	engine.setupParameter(S, r, rf, q, sigma, sigma_fx, rho_fx - drhofx);
	engine.calculate();
	down_result = engine.GetResults().value_;

	std::cout << "Analytic Quanto Cega: " << boost::any_cast<double>(original_results.additionalResults["quanto_cega"]);
	std::cout << ", Estimated Quanto Cega: " << (up_result - down_result) / (2 * drhofx) << std::endl;
}