#pragma once
#include <boost/math/distributions/normal.hpp>
#include <Source/DesignPattern/Singleton.h>

class NormalDistribution : private Singleton<boost::math::normal_distribution<double> > {
public:
	static boost::math::normal_distribution<double>& get(double average, double sigma) {
		static boost::math::normal_distribution<double> instance(average, sigma);
		return instance;
	}
};

double normpdf(double x, double average = 0.0, double sigma = 1.0);
double normcdf(double x, double average = 0.0, double sigma = 1.0);