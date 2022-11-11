#include <Source/Math/NormalDistribution.h>

double normpdf(double x, double average, double sigma)
{
	return boost::math::pdf(NormalDistribution::get(average, sigma), x);
}

double normcdf(double x, double average, double sigma)
{
	return boost::math::cdf(NormalDistribution::get(average, sigma), x);
}