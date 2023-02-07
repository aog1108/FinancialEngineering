#include <Source/Math/Estimator1D.h>
#include <Source/Math/Interpolation/Interpolation1D.h>
#include <Source/Math/Extrapolation/Extrapolation1D.h>
#include <stdexcept>

Estimator1D::Estimator1D(const std::shared_ptr<Extrapolation1D>& front_extrapolation,
	const std::shared_ptr<Interpolation1D>& interpolation,
	const std::shared_ptr<Extrapolation1D>& end_extrapolation)
	: front_extrapolation_(front_extrapolation), interpolation_(interpolation), end_extrapolation_(end_extrapolation)
{
	if ((front_extrapolation_->getLocation() != Extrapolation1D::Location::Front) ||
		(end_extrapolation_->getLocation() != Extrapolation1D::Location::End))
		throw std::runtime_error("Invalid extrapolation location type");
}

double Estimator1D::value(double x) const
{
	if (interpolation_->isValidRange(x)) {
		return interpolation_->value(x);
	}
	else if (front_extrapolation_->isValidRange(x))
		return front_extrapolation_->value(x);
	else
		return end_extrapolation_->value(x);
}

double Estimator1D::derivative(double x) const
{
	if (interpolation_->isValidRange(x))
		return interpolation_->derivative(x);
	else
		throw std::range_error("Invalid range for derivative");
}

double Estimator1D::second_derivative(double x) const
{
	if (interpolation_->isValidRange(x))
		return interpolation_->second_derivative(x);
	else
		throw std::range_error("Invalid range for derivative");
}

double Estimator1D::primitive(double x) const
{
	if (interpolation_->isValidRange(x))
		return interpolation_->primitive(x);
	else
		throw std::range_error("Invalid range for primitive");
}
