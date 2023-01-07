#pragma once
#include <Source/PricingEngine/BlackCalculator.h>

//Vega, Rho�� ������ �ٲپ�� �ϰ�, �� �� BlackCalculator�� ��� ������ ����ؾ� �ϹǷ� private ������� ������.
//��ġ�� �κ��� ���Ƽ� public ����� �ް�, �ٲٴ� �Ϻ� �޼ҵ忡 ���ؼ��� 
//using ������ ���� BlackCalculator�� �޼ҵ带 privateȭ��Ű�� ������ ������ ���� ������,
//�̷��� ���� �� �������� ������ BlackCalculator �����͸� ���� 
//QuantoBlackCalculator�� �޼ҵ� ȣ�� �� BlackCalculator �޼ҵ尡 ȣ��Ǵ� �� ���� ����� ����.

class QuantoBlackCalculator : private BlackCalculator {
public:
	QuantoBlackCalculator() = default;
	QuantoBlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff, double forward, double discount, double stddev, double variance);

	double value() const;
	double delta(double S) const;
	double gamma(double S) const;
	double vega(double ttm, double sigma_fx, double rho_fx) const;
	double theta(double spot, double ttm) const;
	double discountrho(double ttm) const;
	double driftrho(double ttm) const;
	double epsilon(double ttm) const;
	double fxvega(double ttm, double rho_fx) const;
	double quantocega(double ttm, double sigma_fx) const;

	double forward() const;
	double strike() const;
	double discount() const;
	double stddev() const;
	double variance() const;
};