#include "Test.h"

//FDM / MC ���� ���� �� PlainVanilla�� �ϴ� �����ϵ���.
//������ Exotic�� FDM or MC �ۿ� �� ���ϱ�... FDM MC ���� ����.
//Rate TermStructure / Vol Surface
//FDM Implicit Explicit Crank-Nicolson Operator-Spliting
//MC RNG Sobol sequence Brownian Bridge

//Parameter ���� ��ȭ��Ų �Ŷ� ���ؼ� ����.

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