#include "Test.h"

//FDM / MC ���� ���� �� PlainVanilla�� �ϴ� �����ϵ���?
//������ Exotic�� FDM or MC �ۿ� �� ���ϱ�... ���� �����ؾ� �� ��?
//Rate TermStructure / Vol Surface
//FDM Implicit Explicit Crank-Nicolson Operator-Spliting
//MC RNG Sobol sequence Brownian Bridge

//Parameter ���� ��ȭ��Ų �Ŷ� ���ؼ� �����սô�.

int main() {
	//TestDate();
	//TestBoostGregorian();
	//TestCalendar();
	//TestJointCalendar();
	//TestDayCounter();
	//TestNormalDistribution();
	//TestBlackCalculator();
	//TestQuantoBlackEuropeanOptionEngine();
	TestQuantoBlackEuropeanOptionEngineGreeks();

	return 0;
}