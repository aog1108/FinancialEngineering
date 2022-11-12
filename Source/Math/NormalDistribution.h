#pragma once
#include <boost/math/distributions/normal.hpp>

class NormalDistribution {
public:
	NormalDistribution() = default;
	explicit NormalDistribution(double average = 0.0, double sigma = 1.0) : dist_(boost::math::normal_distribution<double>(average, sigma)) { }
	explicit NormalDistribution(const boost::math::normal_distribution<double>& dist): dist_(dist) { }

	double get_average() { return dist_.mean(); }
	double get_sigma() { return dist_.standard_deviation(); }

	double pdf(double x) { return boost::math::pdf(dist_, x); }
	double cdf(double x) { return boost::math::cdf(dist_, x); }

private:
	boost::math::normal_distribution<double> dist_;
};