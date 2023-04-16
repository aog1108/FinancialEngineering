#include "Test.h"

//FDM / MC 엔진 개발 후 PlainVanilla만 일단 가능하도록.
//어차피 Exotic은 FDM or MC 밖에 못 쓰니까... FDM MC 먼저 개발.
//Rate TermStructure / Vol Surface
//FDM Implicit Explicit Crank-Nicolson Operator-Spliting
//MC RNG Sobol sequence Brownian Bridge

//Parameter 조금 변화시킨 거랑 비교해서 검증.

int main() {
	//TestDate();
	//TestBoostGregorian();
	//TestCalendar();
	//TestJointCalendar();
	//TestDayCounter();
	//TestNormalDistribution();
	//TestBlackCalculator();
	//TestQuantoBlackEuropeanOptionEngine();
	//TestQuantoBlackEuropeanOptionEngineGreeks();
	//TestEstimator1D();	
	//TestSchedule();
	TestDateScheduleConverter();

	return 0;
}